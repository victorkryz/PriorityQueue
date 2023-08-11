/**
 * PriorityQueue
 */


#ifndef PRIORITYQUEUE_CHANNEL_H
#define PRIORITYQUEUE_CHANNEL_H

#include <atomic>
#include <msg.h>

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
    virtual bool popMsg(MsgObserver* observ) = 0;
    virtual bool isEmpty() = 0;
    virtual ~Channel() = default;
};

class ChannelFactory
{
    static std::atomic<Channel*> priorityQueueInstance_;

  public:
    static Channel* getPriorityQueueInstance();
};

#endif //PRIORITYQUEUE_CHANNEL_H
