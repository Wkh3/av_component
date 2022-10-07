#pragma once

namespace component::base{

enum class RefCountReleaseStatus{
    kDroppedLastRef,
    kOtherRefsRemained,
};

class RefCountInterface{
public:
    virtual void AddRef() const = 0;
    virtual RefCountReleaseStatus Release() const = 0;
  
  // Non-public destructor, because Release() has exclusive responsibility for
  // destroying the object.
protected:
    virtual ~RefCountInterface();
};

}