/**
 * PriorityQueue
 */

#include "BreackingHandler.h"

namespace Breaking
{
    volatile BreakingStatus status_  = BreakingStatus::Initial;

    BreakingStatus getStatus() {
        return status_;
    };

    void setStatus(BreakingStatus status) {
        status_ = status;
    }

   

#ifdef _WIN32

    #include <windows.h>

   BOOL WINAPI ctrlBreakHandler(DWORD dwCtrlType)
   {
	   setStatus(BreakingStatus::Pending);
	   return TRUE;
   }

	void reactivateHandler()
	{
		status_ = BreakingStatus::Initial;
		SetConsoleCtrlHandler(ctrlBreakHandler, TRUE);
	}

	void deactivateHandler()
	{
		status_ = BreakingStatus::Initial;
		SetConsoleCtrlHandler(ctrlBreakHandler, FALSE);
	}

#else

#include <signal.h>

    void ctrlBreakHandler(int sig) {
        setStatus(BreakingStatus::Pending);
    }

    void reactivateHandler() {
        signal(SIGINT, ctrlBreakHandler);
    }

    void deactivateHandler(){
        signal(SIGINT, SIG_DFL);
    }


#endif // _WIN32

} //namespace Breaking

// obtains a breaking status
bool gl_getCancelStatus() {
    return (Breaking::BreakingStatus::Pending == Breaking::getStatus());
}



