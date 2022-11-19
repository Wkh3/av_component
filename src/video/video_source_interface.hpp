#pragma once
#include "video/video_sink_interface.hpp"
namespace component::video {

class VideoSinkWants;

template <typename VideoFrameT>
class VideoSourceInterface {
public:
    virtual ~VideoSourceInterface();

    virtual void AddOrUpdateSink(VideoSinkInterface<VideoFrameT>* sink, const VideoSinkWants& wants) = 0;

    virtual void RemoveSink(VideoSinkInterface<VideoFrameT>* sink) = 0;

    virtual void RequestRefreshFrame() {}
};

} // namespace component::video