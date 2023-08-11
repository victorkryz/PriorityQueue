/**
 * PriorityQueue
 */

#ifndef PRIORITYQUEUE_CHANNELIMPL_H
#define PRIORITYQUEUE_CHANNELIMPL_H

#include <queue>
#include <mutex>
#include <functional>
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
     virtual ~ChannelImpl() = default;

     void pushMsg(const Msg& msg) override;
     bool popMsg(MsgObserver* observ) override;
     bool isEmpty() override {
         return queue_.empty();
     }

   private:
    std::mutex mtx_;
    std::priority_queue<Msg, std::vector<Msg>, std::function<bool (Msg&, Msg&)>> queue_;
};


#endif //PRIORITYQUEUE_CHANNELIMPL_H
