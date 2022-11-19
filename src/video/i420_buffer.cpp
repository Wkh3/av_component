#include "video/i420_buffer.hpp"

#include <string.h>

#include <algorithm>

#include "base/make_ref_counted.hpp"
#include "libyuv/convert.h"
#include "libyuv/planar_functions.h"
#include "libyuv/scale.h"

static constexpr const int kBufferAlignment = 64;

namespace component::video {

int I420DataSize(int height, int stride_y, int stride_u, int stride_v) {
    return stride_y * height + (stride_u + stride_v) * ((height + 1) / 2);
}

I420Buffer::I420Buffer(int width, int height)
        : I420Buffer(width, height, width, (width + 1) / 2, (width + 1) / 2) {
}

I420Buffer::I420Buffer(int width, int height, int stride_y, int stride_u, int stride_v)
        : width_(width),
          height_(height),
          stride_y_(stride_y),
          stride_u_(stride_u),
          stride_v_(stride_v),
          data_(base::AlignedMalloc<uint8_t>(
              I420DataSize(height, stride_y, stride_u, stride_v),
              kBufferAlignment)) {
}

I420Buffer::~I420Buffer() {
}

void I420Buffer::InitializeData() {
    ::memset(data_.get(), 0, I420DataSize(height_, stride_y_, stride_u_, stride_v_));
}

base::scoped_refptr<I420Buffer> I420Buffer::Create(int width, int height) {
    return base::make_ref_counted<I420Buffer>(width, height);
}

base::scoped_refptr<I420Buffer> I420Buffer::Create(int width,
                                                   int height,
                                                   int stride_y,
                                                   int stride_u,
                                                   int stride_v) {
    return base::make_ref_counted<I420Buffer>(width, height, stride_y, stride_u, stride_v);
}

int I420Buffer::width() const {
    return width_;
}

int I420Buffer::height() const {
    return height_;
}

const uint8_t* I420Buffer::DataY() const {
    return data_.get();
}

const uint8_t* I420Buffer::DataU() const {
    return data_.get() + stride_y_ * height_;
}

const uint8_t* I420Buffer::DataV() const {
    return data_.get() + stride_y_ * height_ + stride_u_ * ((height_ + 1) / 2);
}

int I420Buffer::StrideY() const {
    return stride_y_;
}

int I420Buffer::StrideU() const {
    return stride_u_;
}

int I420Buffer::StrideV() const {
    return stride_v_;
}

uint8_t* I420Buffer::MutableDataY() {
    return const_cast<uint8_t*>(DataY());
}

uint8_t* I420Buffer::MutableDataU() {
    return const_cast<uint8_t*>(DataU());
}

uint8_t* I420Buffer::MutableDataV() {
    return const_cast<uint8_t*>(DataV());
}

base::scoped_refptr<I420Buffer> I420Buffer::Copy(const I420BufferInterface& buffer) {
    return Copy(buffer.width(), buffer.height(), buffer.DataY(), buffer.StrideV(),
                buffer.DataU(), buffer.StrideU(), buffer.DataV(), buffer.StrideV());
}
base::scoped_refptr<I420Buffer> I420Buffer::Copy(int width,
                                                 int height,
                                                 const uint8_t* data_y,
                                                 int stride_y,
                                                 const uint8_t* data_u,
                                                 int stride_u,
                                                 const uint8_t* data_v,
                                                 int stride_v) {
    auto buffer = Create(width, height);
    libyuv::I420Copy(data_y, stride_y, data_u, stride_u, data_v, stride_v,
                     buffer->MutableDataY(), buffer->StrideY(),
                     buffer->MutableDataU(), buffer->StrideU(),
                     buffer->MutableDataV(), buffer->StrideV(),
                     width, height);
    return buffer;
}

base::scoped_refptr<I420Buffer> I420Buffer::Rotate(const I420BufferInterface& src, VideoRotation rotation) {
    int rotated_width  = src.width();
    int rotated_height = src.height();

    if (rotation == VideoRotation::kVideoRotation_90 ||
        rotation == VideoRotation::kVideoRotation_270) {
        std::swap(rotated_width, rotated_height);
    }

    auto buffer = I420Buffer::Create(rotated_width, rotated_height);
    libyuv::I420Rotate(src.DataY(), src.StrideY(), src.DataU(), src.StrideU(), src.DataV(), src.StrideV(),
                       buffer->MutableDataY(), buffer->StrideY(), buffer->MutableDataU(), buffer->StrideU(),
                       buffer->MutableDataV(), buffer->StrideV(), src.width(), src.height(),
                       static_cast<libyuv::RotationMode>(rotation));

    return buffer;
}

void I420Buffer::SetBlack(I420Buffer* buffer) {
    libyuv::I420Rect(buffer->MutableDataY(), buffer->StrideY(),
                     buffer->MutableDataU(), buffer->StrideU(),
                     buffer->MutableDataV(), buffer->StrideV(),
                     0, 0, buffer->width(), buffer->height(), 0, 128, 128);
}

void I420Buffer::CropAndScaleFrom(const I420BufferInterface& src, int offset_x, int offset_y, int crop_width, int crop_height) {
    //Make sure offset is even so that u/v plane becomes aligned.
    const int uv_offset_x = offset_x / 2;
    const int uv_offset_y = offset_y / 2;
    offset_x              = uv_offset_x * 2;
    offset_y              = uv_offset_y * 2;

    const uint8_t* y_plane = src.DataY() + src.StrideY() * offset_y + offset_x;
    const uint8_t* u_plane = src.DataU() + src.StrideU() * uv_offset_y + uv_offset_x;
    const uint8_t* v_plane = src.DataV() + src.StrideV() * uv_offset_y + uv_offset_x;

    libyuv::I420Scale(y_plane, src.StrideY(), u_plane, src.StrideU(), v_plane, src.StrideV(),
                      crop_width, crop_height,
                      MutableDataY(), StrideY(), MutableDataU(), StrideU(), MutableDataV(), StrideV(),
                      width(), height(), libyuv::kFilterBox);
}

void I420Buffer::CropAndScaleFrom(const I420BufferInterface& src) {
    const int crop_width  = height() > 0 ? std::min(src.width(), width() * src.height() / height()) : src.width();
    const int crop_height = width() > 0 ? std::min(src.height(), height() * src.width() / width()) : src.height();
    CropAndScaleFrom(src, (src.width() - crop_width) / 2, (src.height() - crop_height) / 2, crop_width, crop_height);
}

void I420Buffer::ScaleFrom(const I420BufferInterface& src) {
    CropAndScaleFrom(src, 0, 0, src.width(), src.height());
}

} // namespace component::video
