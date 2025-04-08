/**
 * PriorityQueue
 */

#pragma once

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

