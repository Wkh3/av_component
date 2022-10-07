#include "video/video_frame_buffer.hpp"

namespace component::video{

const char* VideoFrameBufferTypeToString(VideoFrameBuffer::Type type) {
  switch (type) {
    case VideoFrameBuffer::Type::kNative:
      return "kNative";
    case VideoFrameBuffer::Type::kI420:
      return "kI420";
    case VideoFrameBuffer::Type::kI420A:
      return "kI420A";
    case VideoFrameBuffer::Type::kI444:
      return "kI444";
    case VideoFrameBuffer::Type::kI010:
      return "kI010";
    case VideoFrameBuffer::Type::kNV12:
      return "kNV12";
    default:
      return "unknown-videoframe-type";
  }
}


}