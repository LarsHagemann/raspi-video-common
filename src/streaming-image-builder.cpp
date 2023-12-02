#include "raspi-video-common/streaming-image-builder.hpp"
#include "raspi-video-common/messages.hpp"

#include <cstring>
#include <cassert>

CStreamingImageBuilder::CStreamingImageBuilder(
    CMulticastClient* client,
    CLoggingService*  logger,
    uint8_t           parallelBuilds)
    : m_client(client) 
    , m_logger(logger)
    , m_maxParallelBuilds(parallelBuilds) {
  m_buffer = std::make_unique<char[]>(CMulticastClient::max_packet_size);
}

bool CStreamingImageBuilder::updateBuilder(image_id_t id) {
  auto it = m_builderData.find(id);
  auto now = std::chrono::steady_clock::now();
  if (it == m_builderData.end()) {
    if (m_droppedImages.find(id) != m_droppedImages.end()) {
      // image was dropped in the past
      return false;
    }

    m_logger->debug(
        "CStreamingImageBuilder: New image %zd",
        id);
    
    m_builderData.emplace(
        id,
        SImageBuilderData{
          .imageId = id,
          .receivedChunks = 0,
          .numChunks = 0,
          .firstImageDataTs = now,
          .lastImageUpdateTs = now,
          .width = 0,
          .height = 0,
          .dataLength = 0,
          .data = nullptr,
        }
    );
    if (m_builderData.size() > m_maxParallelBuilds) {
      // remove image with oldest update ts
      auto erase = m_builderData.begin();
      for (auto it = m_builderData.begin(); it != m_builderData.end(); ++it) {
        if (it->second.lastImageUpdateTs < erase->second.lastImageUpdateTs) {
          erase = it;
        }
      }
      if (!validateBuilder(erase->second.imageId)) {
        const auto age = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::steady_clock::now() - erase->second.lastImageUpdateTs
        );
        m_logger->warning(
            "CStreamingImageBuilder: Dropping image %zd; "
            "age=%zdms; "
            "chunks=%d/%d; "
            "reason: num_builder > %d",
            erase->second.imageId,
            age.count(),
            erase->second.receivedChunks,
            erase->second.numChunks,
            static_cast<uint32_t>(m_maxParallelBuilds));

        m_builderData.erase(erase);
      }
    }
    return true;
  }
  else {
    it->second.lastImageUpdateTs = now;
    return true;
  }
}

bool CStreamingImageBuilder::validateBuilder(image_id_t id) {
  auto it = m_builderData.find(id);
  if (it->second.receivedChunks == it->second.numChunks) {
    
    auto sharedObj = std::make_shared<SImageBuilderData>(SImageBuilderData{
        .imageId = it->second.imageId,
        .receivedChunks = it->second.receivedChunks,
        .numChunks = it->second.numChunks,
        .firstImageDataTs = it->second.firstImageDataTs,
        .lastImageUpdateTs = it->second.lastImageUpdateTs,
        .width = it->second.width,
        .height = it->second.height,
        .dataLength = it->second.dataLength,
        .data = std::move(it->second.data)
    });

    m_logger->debug("CStreamingImageBuilder: Processing %zd", it->second.imageId);

    for (auto processor : m_processors) {
      processor->enqueue(sharedObj);
    }

    m_builderData.erase(it);
    return true;
  }
  return false;
}

void CStreamingImageBuilder::handleChunkMessage() {
  auto chunk = (TSImageChunkMessage<>*)(m_buffer.get());
  m_logger->debug(
      "CStreamingImageBuilder: Chunk for %zd",
      chunk->imageId);
  if (updateBuilder(chunk->imageId)) {
    auto it = m_builderData.find(chunk->imageId);
    const auto offset = chunk->chunkOffset;
    const auto length = chunk->chunkLength;
    
    it->second.receivedChunks++;
    if (!it->second.data) {
      it->second.dataLength = chunk->imageByteLength;
      it->second.data = std::make_unique<char[]>(chunk->imageByteLength);
    }
    assert(it->second.numChunks == 0 || it->second.numChunks == chunk->numChunks);
    assert(it->second.dataLength == chunk->imageByteLength);
    assert(offset + length <= it->second.dataLength);

    it->second.numChunks = chunk->numChunks;

    std::memcpy(
        it->second.data.get() + offset,
        chunk->chunk,
        length
    );
  }

  validateBuilder(chunk->imageId);
}

void CStreamingImageBuilder::addProcessor(FImageProcessor* processor) {

  m_processors.emplace_back(processor);
}

void CStreamingImageBuilder::run() {  
  const auto imageTimeout = std::chrono::milliseconds(800);
  while (m_running) {
    auto now = std::chrono::steady_clock::now();
    // cleanup 
    std::erase_if(
        m_builderData,
        [this,now,imageTimeout](const auto& item) {
      const auto& [key, value] = item;
      const auto elapsed = now - value.lastImageUpdateTs;
      const auto result = elapsed >= imageTimeout;

      if (result) {
        m_logger->warning(
            "CStreamingImageBuilder: Dropping image %zd; "
            "age=%zdms; "
            "chunks=%d/%d; "
            "reason: timeout > %zdms",
            value.imageId,
            std::chrono::duration_cast<
              std::chrono::milliseconds>(elapsed).count(),
            value.receivedChunks,
            value.numChunks,
            imageTimeout.count());

      }
    
      return result;
    });

    const auto length = m_client->receive(
        m_buffer.get(),
        CMulticastClient::max_packet_size
    );
    if (length > 0) {
      const auto type = (EMessageType)(m_buffer.get())[0];
      if (type >= EMessageType::COUNT) {
        m_logger->warning(
            "CStreamingImageBuilder: received invalid message type %d",
            static_cast<uint32_t>(type)
        );
        continue;
      }
      
      switch (type) {
        case EMessageType::MC_IMAGE_CHUNK:
          handleChunkMessage();
          break;

        case EMessageType::COUNT:
        default:
          break;
      }
    }
  }
}

void CStreamingImageBuilder::stop() {
  m_running = false;
}
