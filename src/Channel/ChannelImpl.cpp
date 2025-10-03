/**
 * PriorityQueue
 */

#include "Channel.h"
#include "ChannelImpl.h"

 /**
 *  ChannelImpl class implementation
 */

Channel* ChannelFactory::getPriorityQueueInstance()
{
    std::lock_guard lg(mtx_);

    if ( !priorityQueueInstance_)
        priorityQueueInstance_ = new ChannelImpl();

    return priorityQueueInstance_;
}

bool ChannelImpl::popMsg(MsgObserver* observ)
{
    std::lock_guard<std::mutex> guard(mtx_);

    bool bHasMsg(!queue_.empty());
    while ( bHasMsg )
    {
        const Msg &msg = queue_.top();

        if ( nullptr != observ )
            observ->onMsg(msg);

        queue_.pop();
        
        bHasMsg = !queue_.empty();
    }

    return bHasMsg;
}

void ChannelImpl::pushMsg(Msg&& msg)
{
    std::lock_guard<std::mutex> guard(mtx_);
    queue_.emplace(std::move(msg));
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

