#pragma once
#include "video_frame_buffer.hpp"
#include "video_rotation.hpp"
#include "base/scoped_refptr.hpp"
#include "base/aligned_malloc.hpp"
#include <memory>
namespace component::video{

class I420Buffer : public I420BufferInterface{
public:
    static base::scoped_refptr<I420Buffer> Create(int width,int height);
    static base::scoped_refptr<I420Buffer> Create(int width,
                                                  int height,
                                                  int stride_y,
                                                  int stride_u,
                                                  int stride_v);

    static base::scoped_refptr<I420Buffer> Copy(const I420BufferInterface &buffer);

    void InitializeData();

    int width() const override;
    int height() const override;

    const uint8_t* DataY() const override;
    const uint8_t* DataU() const override;
    const uint8_t* DataV() const override;

    int StrideY() const override;
    int StrideU() const override;
    int StrideV() const override;

    uint8_t* MutableDataY();
    uint8_t* MutableDataU();
    uint8_t* MutableDataV();

protected:
    I420Buffer(int width,int height);
    I420Buffer(int width,int height,int stride_y,int stride_u,int stride_v);
    ~I420Buffer() override;
private:
    const int width_;
    const int height_;
    const int stride_y_;
    const int stride_u_;
    const int stride_v_;
    const std::unique_ptr<uint8_t,base::AlignedFreeDeleter> data_;
};


}