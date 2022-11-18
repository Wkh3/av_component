#pragma once

namespace component::video{

template<typename VideoFrameT>
class VideoSinkInterface{
public:
    virtual ~VideoSinkInterface();

    virtual void OnFrame(const VideoFrameT &frame) = 0;
    //should be called by the source when it discards the frame due to rate
    virtual void OnDiscardedFrame(){}
};

}