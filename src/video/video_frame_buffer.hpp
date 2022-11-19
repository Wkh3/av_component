#pragma once
#include <stdint.h>

#include <utility>

#include "base/ref_count.hpp"
#include "base/scoped_refptr.hpp"
namespace component::video {

class I420BufferInterface;
class I420ABufferInterface;
class I444BufferInterface;
class I010BufferInterface;
class NV12BufferInterface;

class VideoFrameBuffer : public base::RefCountInterface {
public:
    enum class Type {
        kNative,
        kI420,
        kI420A,
        kI444,
        kI010,
        kNV12,
    };

    virtual Type type() const = 0;

    virtual int width() const = 0;

    virtual int height() const = 0;

    virtual base::scoped_refptr<I420BufferInterface> ToI420() = 0;

    virtual const I420BufferInterface* GetI420() const;

    virtual base::scoped_refptr<VideoFrameBuffer> CropAndScale(int offset_x,
                                                               int offset_y,
                                                               int crop_width,
                                                               int crop_height,
                                                               int scaled_width,
                                                               int scaled_height);

    base::scoped_refptr<VideoFrameBuffer> Scale(int scaled_width, int scaled_height) {
        return CropAndScale(0, 0, width(), height(), scaled_width, scaled_height);
    }

    const I420ABufferInterface* GetI420A() const;
    const NV12BufferInterface* GetNV12() const;

protected:
    ~VideoFrameBuffer() override {}
};

const char* VideoFrameBufferTypeToString(VideoFrameBuffer::Type type);

// This interface represents planar formats.
class PlanarYuvBuffer : public VideoFrameBuffer {
public:
    virtual int ChromaWidth() const  = 0;
    virtual int ChromaHeight() const = 0;

    // Returns the number of steps(in terms of Data*() return type) between
    // successive rows for a given plane.
    virtual int StrideY() const = 0;
    virtual int StrideU() const = 0;
    virtual int StrideV() const = 0;

protected:
    ~PlanarYuvBuffer() override {}
};

// This interface represents 8-bit color depth formats: Type::kI420,
// Type::kI420A and Type::kI444.
class PlanarYuv8Buffer : public PlanarYuvBuffer {
public:
    // Returns pointer to the pixel data for a given plane. The memory is owned by
    // the VideoFrameBuffer object and must not be freed by the caller.
    virtual const uint8_t* DataY() const = 0;
    virtual const uint8_t* DataU() const = 0;
    virtual const uint8_t* DataV() const = 0;

protected:
    ~PlanarYuv8Buffer() override {}
};

class I420BufferInterface : public PlanarYuv8Buffer {
public:
    Type type() const override;

    int ChromaWidth() const final;
    int ChromaHeight() const final;

    base::scoped_refptr<I420BufferInterface> ToI420() final;
    const I420BufferInterface* GetI420() const final;

protected:
    ~I420BufferInterface() override {}
};

class BiplanarYuvBuffer : public VideoFrameBuffer {
public:
    virtual int ChromaWidth() const  = 0;
    virtual int ChromaHeight() const = 0;

    // Returns the number of steps(in terms of Data*() return type) between
    // successive rows for a given plane.
    virtual int StrideY() const  = 0;
    virtual int StrideUV() const = 0;

protected:
    ~BiplanarYuvBuffer() override {}
};

class BiplanarYuv8Buffer : public BiplanarYuvBuffer {
public:
    virtual const uint8_t* DataY() const  = 0;
    virtual const uint8_t* DataUV() const = 0;

protected:
    ~BiplanarYuv8Buffer() override {}
};

class NV12BufferInterface : public BiplanarYuv8Buffer {
public:
    Type type() const override;

    int ChromaWidth() const final;
    int ChromaHeight() const final;

    base::scoped_refptr<VideoFrameBuffer> CropAndScale(int offset_x,
                                                       int offset_y,
                                                       int crop_width,
                                                       int crop_height,
                                                       int scaled_width,
                                                       int scaled_height) override;

protected:
    ~NV12BufferInterface() override {}
};

} // namespace component::video