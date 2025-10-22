/**
 * PriorityQueue
 */

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include "Channel/Channel.h"
#include "Server/Server.h"
#include "Client/Client.h"
#include "Utils/BreackingHandler.h"
#include "Utils/cxxopts.hpp"



// default limit of issued messages per client
const size_t gl_msgNumberLimit = 1000;

// some functions declaration
bool processArguments(int argc, char* argv[], size_t& c_clients, size_t& c_msgs);
void showUsage(const cxxopts::Options& options);
std::thread startServer(Server* pServer, std::unique_ptr<Channel>& spChannel);
void startClients(std::unique_ptr<Channel>& spChannel, size_t c_clients, size_t msgLimit, 
				  std::vector<std::shared_ptr<Client>>& clients, std::vector<std::thread>& clientThreads);

/**
* main procedure
*
* command line parameters:
*	-c, --clients    clients number(not above than 10)
*	-m, --messages   a number of issued messages per a single client(optional, 1000 - by default)
*/
int main(int argc, char* argv[])
{
	using namespace std::chrono_literals;

    size_t arg_c_clients(0), arg_c_msgs(gl_msgNumberLimit);
    if (!processArguments(argc, argv, arg_c_clients, arg_c_msgs))
            return 1;

    std::cout << "Accepted: clients number: " << arg_c_clients <<
                             ", messages per client: " << arg_c_msgs << std::endl;

    std::cout << "Processing ... (use Ctrl+Break to abort)" << std::endl;

    Breaking::reactivateHandler();

	// obtain "channel":
    std::unique_ptr<Channel> spChannel(ChannelFactory::getPriorityQueueInstance());

	// start server:
	Server* pServer = Server::getInstance();
    std::thread srvTh = startServer(pServer, spChannel);

	// start clients:
	std::chrono::time_point msg_transf_start_time = std::chrono::steady_clock::now();

    std::vector<std::thread> clientThreads;
    std::vector<std::shared_ptr<Client>> clients;
	startClients(spChannel, arg_c_clients, arg_c_msgs, clients, clientThreads);

    // wait while clients are active:
    for ( auto& th: clientThreads)
         th.join();

    // all clients have gone ...
	std::chrono::time_point msg_transf_finish_time = std::chrono::steady_clock::now();
	std::chrono::duration<float> msg_transf_duration = std::chrono::duration_cast<std::chrono::milliseconds>(msg_transf_finish_time - msg_transf_start_time);

    // notify server it's time to shutdown
    pServer->scheduleShutDown();

    // wait for the server's completion:
    srvTh.join();

    clientThreads.clear();
    clients.clear();

	int exitCode(0);
	std::string strDone("Done");
	if ( Breaking::BreakingStatus::Pending  == Breaking::getStatus() )
	{
		strDone = "Aborted";
		exitCode = 1;
	}

    std::cout << strDone << "! (" << pServer->getReceivedMsgCount()  << " messages were received for " 
								   << std::setprecision(2) << msg_transf_duration.count() << " s)" << std::endl;
    std::cout << "See log file: \"" << pServer->getLogFileName() << "\"" << std::endl;

    Server::releaseInstance();

    return 0;
}

//handle command-line arguments
bool processArguments(int argc, char* argv[], size_t& c_clients, size_t& c_msgs)
{
	// there's used third-party "Lightweight C++ command line option parser"  (https://github.com/jarro2783/cxxopts)

	constexpr size_t MAX_CLIENTS = 50;

	bool bResult(false);

	try
	{
		cxxopts::Options options("PriorityQueue", "Modelling of client/server communications based on priority queue.");
		options.positional_help("[optional args]")
			.show_positional_help();

		options.add_options()
			("c, clients", "clients number (not above than 50)", cxxopts::value<size_t>(c_clients))
			("m, messages", "a number of issued messages per client (optional, 1000 by default)", cxxopts::value<size_t>(c_msgs));

		auto args = options.parse(argc, argv);

		if (0 != args.count("clients"))
		{
			if (MAX_CLIENTS >= c_clients)
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
			showUsage(options);
		}
	}
	catch (const cxxopts::exceptions::exception& e)
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
std::thread startServer(Server* pServer, std::unique_ptr<Channel>& spChannel)
{
    std::thread th([pServer, &spChannel]() {
                Server::run(pServer, spChannel); });
    return th;
}

// launches a set of clients on specified channel
void startClients(std::unique_ptr<Channel>& spChannel, size_t c_clients, size_t msgLimit, 
				  std::vector<std::shared_ptr<Client>>& clients, std::vector<std::thread>& clientThreads)
{
	using namespace std::chrono_literals;
	
	clients.resize(c_clients);
	clientThreads.reserve(c_clients);

	for (int i = 0; i < c_clients; i++)
	{
		std::shared_ptr<Client>& spClient = clients[i];
		spClient = std::make_shared<Client>(i+1);

		std::thread th([&spClient, &spChannel, msgLimit]() {
								Client::run(spClient, spChannel, msgLimit);}); 
		
		clientThreads.push_back(std::move(th));
	}
}



