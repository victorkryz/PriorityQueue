/**
 * PriorityQueue
 */

#include <cstring>
#include <sstream>   
#include <chrono>
#include <thread>
#include "Client.h"
#include "Utils/apptm.h"

/**
* Client class implementation
*/

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
    pData->dwTicks = ticks;

	// form some data:
    memset(pData->Data, 0, sizeof(pData->Data));
    std::stringstream strstr;
    strstr << "Message from client "  <<  id_ << ".";
    std::strcpy(pData->Data, strstr.str().c_str());

    {
      std::lock_guard<std::mutex> guard(mtx_);
      pData->cPriority = uid_(rnGen_);
    }

    return (Msg(pData));
}
