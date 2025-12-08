/**
 * PriorityQueue
 */

#pragma once

#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include "Channel.h"

 /**
 * ChannelImpl class declaration
 */

class ChannelImpl : public Channel {

   public:
     ChannelImpl() : queue_([](Msg& left, Msg& right)
                            { return left.getContent().cPriority >
                                      right.getContent().cPriority; }) {
     }
     ~ChannelImpl() override = default; // ensures the base destructor is virtual

     // non-copyable/moveable 
     ChannelImpl(const ChannelImpl&) = delete;
     ChannelImpl& operator=(const ChannelImpl&) = delete;
     ChannelImpl(ChannelImpl&&) = delete;
     ChannelImpl& operator=(ChannelImpl&&) = delete;

     void pushMsg(const Msg& msg) override;
     void pushMsg(Msg&& msg) override;
     bool popMsg(MsgObserver* observ) override;
     bool isEmpty() override {
         return queue_.empty();
     }
     bool waitForMsg(const std::chrono::duration<double, std::milli>& timeout) override;

   private:
    std::mutex mtx_, cv_mtx_;
    std::condition_variable_any cv_;
    std::priority_queue<Msg, std::vector<Msg>, std::function<bool (Msg&, Msg&)>> queue_;
};

