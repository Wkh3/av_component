#pragma once

namespace component::base{

    class NoCopyAble{
    protected:
        NoCopyAble() = default;
        ~NoCopyAble() = default;

        NoCopyAble(const NoCopyAble &) = delete;
        NoCopyAble& operator=(const NoCopyAble&) = delete;
    };
}