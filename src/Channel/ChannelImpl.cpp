/**
 * PriorityQueue
 */

#include "ChannelImpl.h"

 /**
 *  ChannelImpl class implementation
 */

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

void ChannelImpl::pushMsg(Msg&& msg)
{
    std::lock_guard<std::mutex> guard(mtx_);
    queue_.emplace(msg);
    cv_.notify_all();
}

void ChannelImpl::pushMsg(const Msg& msg)
{
    std::lock_guard<std::mutex> guard(mtx_);
    queue_.push(msg);
    cv_.notify_all();
}

bool ChannelImpl::waitForMsg(const std::chrono::duration<double, std::milli>& timeout)
{
    bool result(false);
    std::unique_lock lc(cv_mtx_);
    cv_.wait_for(lc, timeout, [this, &result](){ return (result = !queue_.empty());});
    return result;
}

