/**
 * PriorityQueue
 */

#include <cstring>
#include <sstream>   
#include <iostream>
#include "Client.h"
#include "Utils/apptm.h"

/**
* Client class implementation
*/

void Client::run(std::shared_ptr<Client>& spClient, std::unique_ptr<Channel>& spChannel, size_t msgCount)
{
    using namespace std::chrono_literals;

    {
      std::lock_guard<std::mutex> guard(mtx_);
      std::cout << "Client " << spClient->id_ << " started ->" << std::endl;
    }

    if ( (nullptr != spClient) &&
            (nullptr != spChannel))
    {
            while (!gl_getCancelStatus() &&
                          (msgCount-- != 0)) {
                spClient->doAction(*spChannel);
            }
    }

    {
      std::lock_guard<std::mutex> guard(mtx_);
      std::cout << "Client " << spClient->id_ << " finished <-" << std::endl;
    }
}

void Client::doAction(Channel& channel)
{
    Msg msg;
    generateMsg(msg);
    channel.pushMsg(std::move(msg));
}

void Client::generateMsg(Msg& msg)
{
    const auto ticks = getTicksSinceAppStart();

    TDATA& data = msg.getContent();
    
    data.dwClientId = id_;
    data.dwTicks = ticks;

	  // generate message text:
    std::stringstream strstr;
    strstr << "Message from client "  <<  id_ << ".";
    std::strcpy(data.Data, strstr.str().c_str());

    {
      std::lock_guard<std::mutex> guard(mtx_);
      data.cPriority = uid_(rnGen_);
    }
}
