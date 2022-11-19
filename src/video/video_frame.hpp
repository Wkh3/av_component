#pragma once
#include <optional>

#include "video/video_rotation.hpp"

namespace component::video {

class VideoFrame {
public:
    int width() const;

    int height() const;

    uint32_t size() const;

    uint16_t id() const {
        return id_;
    }

    void set_id(uint16_t id) {
        id_ = id;
    }

    uint32_t timestamp_rtp() const {
        return timestamp_rtp_;
    }

    void set_timestamp_rtp(uint32_t timestamp_rtp) {
        timestamp_rtp_ = timestamp_rtp;
    }

    int64_t ntp_time_ms() const {
        return ntp_time_ms_;
    }

    void set_ntp_time_ms(int64_t ntp_time_ms) {
        ntp_time_ms_ = ntp_time_ms;
    }

    int64_t timestamp_us() const {
        return timestamp_us_;
    }

    void set_timestamp_us(int64_t timestamp_us) {
        timestamp_us_ = timestamp_us;
    }

    VideoRotation rotation() const {
        return rotation_;
    }

    void set_rotation(VideoRotation rotation) {
        rotation_ = rotation;
    }

private:
    uint16_t id_;
    uint32_t timestamp_rtp_;
    int64_t ntp_time_ms_;
    int64_t timestamp_us_;
    VideoRotation rotation_;
};

} // namespace component::video