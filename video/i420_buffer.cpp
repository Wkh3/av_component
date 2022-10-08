#include "video/i420_buffer.hpp"
#include "base/make_ref_counted.hpp"
static constexpr const int kBufferAlignment = 64;

namespace component::video{

int I420DataSize(int height,int stride_y,int stride_u,int stride_v){
    return stride_y * height + (stride_u + stride_v) * ((height + 1) / 2);
}

I420Buffer::I420Buffer(int width,int height) : I420Buffer(width,height,width,(width + 1) / 2,(width + 1) / 2){

}

I420Buffer::I420Buffer(int width,int height,int stride_y,int stride_u,int stride_v) : width_(width),
                                                                                      height_(height),
                                                                                      stride_y_(stride_y),
                                                                                      stride_u_(stride_u),
                                                                                      stride_v_(stride_v),
                                                                                      data_(base::AlignedMalloc<uint8_t>(
                                                                                        I420DataSize(height,stride_y,stride_u,stride_v),
                                                                                        kBufferAlignment
                                                                                      ))
{

}

I420Buffer::~I420Buffer(){

}

void I420Buffer::InitializeData(){
    ::memset(data_.get(),0,I420DataSize(height_,stride_y_,stride_u_,stride_v_));
}

base::scoped_refptr<I420Buffer> I420Buffer::Create(int width,int height){
    return base::make_ref_counted<I420Buffer>(width,height);
}

base::scoped_refptr<I420Buffer> I420Buffer::Create(int width,
                                                   int height,
                                                   int stride_y,
                                                   int stride_u,
                                                   int stride_v){
    return base::make_ref_counted<I420Buffer>(width,height,stride_y,stride_u,stride_v);
}

int I420Buffer::width() const{
    return width_;
}

int I420Buffer::height() const{
    return height_;
}

const uint8_t* I420Buffer::DataY() const{
    return data_.get();
}

const uint8_t* I420Buffer::DataU() const{
    return data_.get() + stride_y_ * height_;
}

const uint8_t* I420Buffer::DataV() const{
    return data_.get() + stride_y_ * height_ + stride_u_ * ((height_ + 1) / 2);
}

int I420Buffer::StrideY() const{
    return stride_y_;
}

int I420Buffer::StrideU() const{
    return stride_u_;
}

int I420Buffer::StrideV() const{
    return stride_v_;
}

uint8_t* I420Buffer::MutableDataY(){
    return const_cast<uint8_t*>(DataY());
}

uint8_t* I420Buffer::MutableDataU(){
    return const_cast<uint8_t*>(DataU());
}

uint8_t* I420Buffer::MutableDataV(){
    return const_cast<uint8_t*>(DataV());
}

base::scoped_refptr<I420Buffer> I420Buffer::Copy(const I420BufferInterface &buffer){


}
base::scoped_refptr<I420Buffer> I420Buffer::Copy(int width,
                                     int height,
                                     const uint8_t *data_y,
                                     int stride_y,
                                     const uint8_t* data_u,
                                     int stride_u,
                                     const uint8_t* data_v,
                                     int stride_v){


}

base::scoped_refptr<I420Buffer> I420Buffer::Rotate(const I420BufferInterface &src,VideoRotation rotation){

}

}
