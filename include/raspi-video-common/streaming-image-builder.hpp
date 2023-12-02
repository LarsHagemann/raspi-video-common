#ifndef RASPI_VIDEO_COMMON_STREAMING_IMAGE_BUILDER_HPP
#define RASPI_VIDEO_COMMON_STREAMING_IMAGE_BUILDER_HPP

#include "raspi-video-common/multicast-client.hpp"
#include "raspi-video-common/logging-service.hpp"
#include "raspi-video-common/messages.hpp"

#include <chrono>
#include <memory>
#include <unordered_map>
#include <unordered_set>

using timestamp_t = std::chrono::steady_clock::time_point;

struct SImageBuilderData {
  image_id_t              imageId;
  uint16_t                receivedChunks;
  uint16_t                numChunks;
  timestamp_t             firstImageDataTs;
  timestamp_t             lastImageUpdateTs;
  uint16_t                width;
  uint16_t                height;
  size_t                  dataLength;
  std::unique_ptr<char[]> data;
};


class FImageProcessor {
public:
  FImageProcessor() = default;
  virtual ~FImageProcessor() = default;

  virtual void enqueue(std::shared_ptr<SImageBuilderData> data) = 0;
};

class CStreamingImageBuilder {
private:
  CMulticastClient*                 m_client;
  CLoggingService*                  m_logger;
  std::unique_ptr<char[]>           m_buffer;

  std::unordered_map<
    image_id_t,
    SImageBuilderData>              m_builderData;
  std::unordered_set<image_id_t>    m_droppedImages;
  uint8_t                           m_maxParallelBuilds;

  std::vector<FImageProcessor*>     m_processors;

  bool                              m_running = true;
private:
  bool updateBuilder(image_id_t id);
  bool validateBuilder(image_id_t id);

  void handleMetadataMessage();
  void handleChunkMessage();
public:
  CStreamingImageBuilder(
      CMulticastClient* client,
      CLoggingService*  logger,
      uint8_t           parallelBuilds);

  void addProcessor(FImageProcessor* processor);
  void run();
  void stop();
};

#endif
