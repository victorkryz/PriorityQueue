/**
 * PriorityQueue
 *
 * @author Viktor Kryzhanivskyi
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <Channel/Channel.h>
#include <Server/Server.h>
#include <Client/Client.h>
#include "BreackingHandler.h"
#include "cxxopts.hpp"


// default limit of issued messages per client
const size_t gl_msgNumberLimit = 1000;

// time of app start
auto gl_appStartTimePoint = std::chrono::system_clock::now();

// some functions declaration
bool processArguments(int argc, char* argv[], size_t& c_clients, size_t& c_msgs);
void showUsage(const cxxopts::Options& options);
std::thread startServer(Server* pServer, Channel* pChannel);
void startClients(std::unique_ptr<Channel>& spChannel, size_t c_clients, size_t msgLimit, 
				  std::vector<std::shared_ptr<Client>>& clients, std::vector<std::thread>& clientThreads);
bool gl_getCancelStatus();


/**
* main procedure
*
* command line parameters:
*	-c, --clients    clients number(not above than 10)
*	- m, --messages   a number of issued messages per a single client(optional, 1000 - by default)
*/
int main(int argc, char* argv[])
{
    size_t arg_c_clients(0), arg_c_msgs(gl_msgNumberLimit);
    if (!processArguments(argc, argv, arg_c_clients, arg_c_msgs))
            return 1;

    std::cout << "Accepted: clients number: " << arg_c_clients <<
                             ", messages per client: " << arg_c_msgs << std::endl;

    std::cout << "Processing ... (use Ctrl+Break to abort)" << std::endl;

    Breaking::reactivateHandler();

	// obtain "channel":
    std::unique_ptr<Channel> spChannel;
    spChannel.reset(ChannelFactory::getPriorityQueueInstance());

	// start server:
	Server* pServer = Server::getInstance();
    std::thread srvTh = startServer(pServer, spChannel.get());

	// start clients:
    std::vector<std::thread> clientThreads;
    std::vector<std::shared_ptr<Client>> clients;
	startClients(spChannel, arg_c_clients, arg_c_msgs, clients, clientThreads);

    // give some time to clint's threads get activated
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);

    // wait while clients are active:
    for ( auto& th: clientThreads)
         th.join();

    // all clients have gone ...
    // notify server it's time to shutdown
    pServer->scheduleShutDown();

    // wait for the server's completion:
    srvTh.join();

    clientThreads.clear();
    clients.clear();

    std::cout << "Done! (" << pServer->getReceivedMsgCount()  << " messages were received)" << std::endl;
    std::cout << "See log file: \"" << pServer->getLogFileName() << "\"" << std::endl;

    Server::releaseInstance();

    return 0;
}

//handle command-line arguments
bool processArguments(int argc, char* argv[], size_t& c_clients, size_t& c_msgs)
{
	// there's used third-party "Lightweight C++ command line option parser"  (https://github.com/jarro2783/cxxopts)

	bool bResult(false);

	try
	{
		cxxopts::Options options("PriorityQueue", "Modelling of client/server communications based on priority queue.");
		options.positional_help("[optional args]")
			.show_positional_help();


		options.add_options()
			("c, clients", "clients number (not above than 10)", cxxopts::value<size_t>(c_clients))
			("m, messages", "a number of issued messages per client (optional, 1000 by default)", cxxopts::value<size_t>(c_msgs));

		auto args = options.parse(argc, argv);

		if (0 != args.count("clients"))
		{
			if (10 >= c_clients)
				bResult = true;
			else
			{
				std::cout << "argument \"clients\" exceeds the allowable value" << std::endl;
				bResult = false;
			}
		}

		if (!bResult )
		{
			// show usage:
			std::string strHelp = options.help();
			std::cout << strHelp;
		}
	}
	catch (const cxxopts::OptionException& e)
	{
		std::cout << "command line parsing error: " << e.what() << std::endl;
		bResult = false;
	}

	return bResult;
}

// show usage:
void showUsage(const cxxopts::Options& options)
{
	std::string strHelp = options.help();
	std::cout << std::endl << strHelp << std::endl;
}


// launches the server on specified channel
std::thread startServer(Server* pServer, Channel* pChannel)
{
    std::thread th([pServer, pChannel]() {
                Server::run(pServer, pChannel); });
    return th;
}


// launches a set of clients on specified channel
void startClients(std::unique_ptr<Channel>& spChannel, size_t c_clients, size_t msgLimit, 
				  std::vector<std::shared_ptr<Client>>& clients, std::vector<std::thread>& clientThreads)
{
	for (size_t i = 0; i < c_clients; i++)
	{
		std::shared_ptr<Client> spClient;
		spClient.reset(new Client(i + 1));

		clientThreads.push_back(std::thread([&spClient, &spChannel, msgLimit]() {
								Client::run(spClient.get(), spChannel.get(), msgLimit); }));
		clients.push_back(spClient);
	}
}

// returns ticks since app started
long long getTicksSinceAppStart()
{
	using namespace std;

	const auto currTicksSinceAppStart =
		chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - gl_appStartTimePoint);
	return currTicksSinceAppStart.count();
}

// obtains a breaking status
bool gl_getCancelStatus() {
    return (Breaking::BreakingStatus::Pending == Breaking::getStatus());
}
