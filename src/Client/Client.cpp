/**
 * PriorityQueue
 */

#include <cstring>
#include <sstream>   
#include <chrono>
#include <thread>
#include "Client.h"


extern bool gl_getCancelStatus();
extern long long getTicksSinceAppStart();

/**
* Client class implementation
*/

std::mt19937 Client::rnGen_(time(nullptr));
std::uniform_int_distribution<unsigned short> Client::uid_(0,255);
std::mutex Client::mtx_;


void Client::run(Client* pClient, Channel* pChannel, size_t msgCount)
{
    using namespace std::chrono_literals;

    if ( (nullptr != pClient) &&
            (nullptr != pChannel))
    {
            while (!gl_getCancelStatus() &&
                          (msgCount-- != 0)) {
                pClient->doAction(*pChannel);
				std::this_thread::sleep_for(2ms);
            }
    }
}

void Client::doAction(Channel& channel)
{
    Msg msg = generateMsg();
    channel.pushMsg(std::move(msg));
}

Msg Client::generateMsg()
{
    const auto ticks = getTicksSinceAppStart();

    tagTDATA* pData = new tagTDATA;
    pData->dwClientId = id_;
    pData->dwTicks = static_cast<DWORD>(ticks);

	// form some data:
    memset(pData->Data, 0, sizeof(pData->Data));
    std::stringstream strstr;
    strstr << "Message from client "  <<  id_ << ".";
    std::strcpy(pData->Data, strstr.str().c_str());

    {
      std::lock_guard<std::mutex> guard(mtx_);
      pData->cPriority = static_cast<unsigned short>(uid_(rnGen_));
    }

    return (Msg(pData));
}
