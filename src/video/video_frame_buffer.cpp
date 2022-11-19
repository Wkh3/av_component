#include "video/video_frame_buffer.hpp"

#include "video/i420_buffer.hpp"
namespace component::video {

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

base::scoped_refptr<VideoFrameBuffer> VideoFrameBuffer::CropAndScale(int offset_x,
                                                                     int offset_y,
                                                                     int crop_width,
                                                                     int crop_height,
                                                                     int scaled_width,
                                                                     int scaled_height) {
    auto buffer = I420Buffer::Create(scaled_width, scaled_height);
    buffer->CropAndScaleFrom(*this->ToI420(), offset_x, offset_y, crop_width, crop_height);
    return buffer;
}

const I420BufferInterface* VideoFrameBuffer::GetI420() const {
    return nullptr;
}

const NV12BufferInterface* VideoFrameBuffer::GetNV12() const {
    return static_cast<const NV12BufferInterface*>(this);
}

VideoFrameBuffer::Type I420BufferInterface::type() const {
    return Type::kI420;
}

int I420BufferInterface::ChromaHeight() const {
    return (height() + 1) / 2;
}

int I420BufferInterface::ChromaWidth() const {
    return (width() + 1) / 2;
}

base::scoped_refptr<I420BufferInterface> I420BufferInterface::ToI420() {
    return base::scoped_refptr<I420BufferInterface>(this);
}

const I420BufferInterface* I420BufferInterface::GetI420() const {
    return this;
}

} // namespace component::video