/**
 * PriorityQueue
 */

#pragma once

#include <atomic>
#include <mutex>
#include "Base/msg.h"

 /**
 * Channel interface declaration
 */

struct MsgObserver
{
	virtual void onMsg(const Msg& msg) = 0;
    virtual ~MsgObserver() = default;
};


struct Channel
{
    virtual void pushMsg(const Msg& msg) = 0; 
    virtual void pushMsg(Msg&& msg) = 0;
    virtual bool popMsg(MsgObserver* observ) = 0;
    virtual bool isEmpty() = 0;
    virtual bool waitForMsg(const std::chrono::duration<double, std::milli>& timeout) = 0;
    virtual ~Channel() = default;
};

class ChannelFactory
{
    inline static Channel* priorityQueueInstance_ = nullptr;
    inline static std::mutex mtx_;

  public:
    static Channel* getPriorityQueueInstance();
    
};

