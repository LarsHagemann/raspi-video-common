#ifndef RASPI_VIDEO_COMMON_MESSAGES_HPP
#define RASPI_VIDEO_COMMON_MESSAGES_HPP

#include <cstdint>
#include <cstddef>

using image_id_t = uint64_t;
using image_chunk_id_t = uint16_t;

enum class EMessageType : uint16_t {
  MC_IMAGE_METADATA,
  MC_IMAGE_CHUNK,
};

struct SImageMetadataMessage {
  EMessageType     type;
  image_id_t       imageId;
  uint16_t         width;
  uint16_t         height;
  uint32_t         imageByteLength;
  uint16_t         numChunks;
};

template<size_t ChunkSize>
struct SImageChunkMessage {
  static constexpr auto ChunkMessageHeaderLength = 18u;

  EMessageType     type;
  image_id_t       imageId;
  image_chunk_id_t imageChunkId;
  uint16_t         chunkLength;
  uint32_t         imageByteLength;

  char             chunk[ChunkSize];
};

#endif
