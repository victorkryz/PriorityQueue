/**
 * PriorityQueue
 */

#include "ChannelImpl.h"

 /**
 *  ChannelImpl class implementation
 */

std::atomic<Channel*> ChannelFactory::priorityQueueInstance_(nullptr);

Channel* ChannelFactory::getPriorityQueueInstance()
{
    if ( nullptr == priorityQueueInstance_)
        priorityQueueInstance_ = new ChannelImpl();

    return priorityQueueInstance_;
}

bool ChannelImpl::popMsg(MsgObserver* observ)
{
    std::lock_guard<std::mutex> guard(mtx_);
    const bool bHasMsg(!queue_.empty());
    if ( bHasMsg )
    {
        const Msg &msg = queue_.top();

        if ( nullptr != observ )
            observ->onMsg(msg);

        queue_.pop();
    }

    return bHasMsg;
}


void ChannelImpl::pushMsg(const Msg& msg)
{
    std::lock_guard<std::mutex> guard(mtx_);
    queue_.push(msg);
}
