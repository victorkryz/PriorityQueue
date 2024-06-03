/**
 * PriorityQueue
 */


#ifndef PRIORITYQUEUE_CHANNEL_H
#define PRIORITYQUEUE_CHANNEL_H

#include <atomic>
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
    inline static std::atomic<Channel*> priorityQueueInstance_ = nullptr;

  public:
    static Channel* getPriorityQueueInstance();
};

#endif //PRIORITYQUEUE_CHANNEL_H
