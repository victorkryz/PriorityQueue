/**
 * PriorityQueue
 */

#ifndef PRIORITYQUEUE_SERVER_H
#define PRIORITYQUEUE_SERVER_H

#include <Channel/Channel.h>
#include <memory>
#include <mutex>
#include <thread>

 /**
 * Server class declaration
 */

namespace spdlog
{
 class logger;
};

class Server : public MsgObserver
{
  private:
    static Server* srvInst_;
	static std::mutex mtx_;

  public:
    static Server* getInstance();
	static void releaseInstance();
	static void run(Server* pServer, std::unique_ptr<Channel>& spChannel);

  protected:	
	Server();
    ~Server() override;

  public:
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
    void loop(std::unique_ptr<Channel>& spChannel);
    void logMsg(const Msg& msg);
	void onMsg(const Msg& msg) override;

    virtual void onIdle();

  protected:
	void initLog();

  private:		
	std::shared_ptr<spdlog::logger> spLogger_;
	bool bShutDownPending_ = false;
	long msgCount_ = 0;
	std::string strLogFileName_;
};


#endif //PRIORITYQUEUE_SERVER_H
