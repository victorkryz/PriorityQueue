/**
 * PriorityQueue
 */

#ifndef PRIORITYQUEUE_BREACKINGHANDLER_H
#define PRIORITYQUEUE_BREACKINGHANDLER_H

namespace Breaking
{
    enum class BreakingStatus
    {
        Initial,
        Pending,
        Processing,
        Handled
    };

    void reactivateHandler();
    void deactivateHandler();
    BreakingStatus getStatus();
    void setStatus(BreakingStatus status);

} //namespace Breaking

#endif //PRIORITYQUEUE_BREACKINGHANDLER_H
