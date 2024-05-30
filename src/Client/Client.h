/**
 * PriorityQueue
 */

#ifndef PRIORITYQUEUE_CLIENT_H
#define PRIORITYQUEUE_CLIENT_H

#include <random>
#include <msg.h>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <Channel/Channel.h>


 /**
 * Client class declaration
 */

class Client {

  public:
    static void run(Client* pClient, Channel* pChannel, size_t msgCount);

    Client(DWORD id) : id_(id){
    }

	virtual ~Client() = default;

   protected:
     void doAction(Channel& channel);

   private:
    Msg generateMsg();

   private:
    DWORD id_;

    static std::mt19937 rnGen_;
    static std::uniform_int_distribution<unsigned short> uid_;
    static std::mutex mtx_;
};

inline std::uniform_int_distribution<unsigned short> Client::uid_(0,255);
inline std::mutex Client::mtx_;
inline std::mt19937 Client::rnGen_(time(nullptr));

#endif //PRIORITYQUEUE_CLIENT_H
