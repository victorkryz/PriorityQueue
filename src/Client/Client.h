/**
 * PriorityQueue
 */

#pragma once

#include <random>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include "Base/msg.h"
#include "Channel/Channel.h"


 /**
 * Client class declaration
 */

class Client {

  public:
    static void run(std::shared_ptr<Client>&  spClient, std::unique_ptr<Channel>& spChannel, size_t msgCount);

    Client(uint64_t id) : id_(id) {
    }

	virtual ~Client() = default;

  uint64_t getId() const {
    return id_;
  }

  protected:
     void doAction(Channel& channel);

   private:
    Msg generateMsg();

   private:
    uint64_t id_ = 0;

    static std::mt19937 rnGen_;
    static std::uniform_int_distribution<unsigned short> uid_;
    static std::mutex mtx_;
};

inline std::uniform_int_distribution<unsigned short> Client::uid_(0,255);
inline std::mutex Client::mtx_;
inline std::mt19937 Client::rnGen_(static_cast<unsigned int>(time(nullptr)));
