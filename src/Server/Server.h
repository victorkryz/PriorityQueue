/**
 * PriorityQueue
 */

#ifndef PRIORITYQUEUE_SERVER_H
#define PRIORITYQUEUE_SERVER_H

#include <Channel/Channel.h>
#include "spdlog/spdlog.h"
#include <memory>

 /**
 * Server class declaration
 */

class Server : public MsgObserver
{
  private:
    static Server* srvInst_;
	static std::mutex mtx_;

  public:
    static Server* getInstance();
	static void releaseInstance();
	static void run(Server* pServer, Channel* pChannel);

    Server();
    virtual ~Server();

	long getReceivedMsgCount() {
		return msgCount_;
	}

	std::string getLogFileName() {
		return strLogFileName_;
	}

	void scheduleShutDown() {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1s);
		bShutDownPending_ = true;
	}

  protected:
    void loop(Channel* pChannel);
    void logMsg(const Msg& msg);
	void onMsg(const Msg& msg) override;

    virtual void onIdle();

  private:
	 void initLog();

	std::shared_ptr<spdlog::logger> spLogger_;
	bool bShutDownPending_ = false;
	long msgCount_ = 0;
	std::string strLogFileName_;
};


#endif //PRIORITYQUEUE_SERVER_H
