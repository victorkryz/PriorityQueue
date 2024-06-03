/**
 * PriorityQueue
 */

// module Server_Module;

#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>   
#include <random>
#include <filesystem>
#include "Server.h"
#include <ctime>
#include <string_view>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "Utils/apptm.h"


namespace fsys = std::filesystem;

/**
* Server class implementation
*/

Server* Server::srvInst_ = nullptr;
std::mutex Server::mtx_;

Server* Server::getInstance()
{
	if (nullptr == srvInst_)
	{
		std::lock_guard<std::mutex> guard(mtx_);
		if (nullptr == srvInst_)
		{
			srvInst_ = new Server();
			srvInst_->initLog();
		}
	}
    return srvInst_;
}

void Server::releaseInstance() {

    if (nullptr != srvInst_)
    {
            delete srvInst_;
            srvInst_ = nullptr;
    }
}

void Server::run(Server* pServer, std::unique_ptr<Channel>& spChannel) 
{
    if ( (nullptr != pServer) &&
                    (spChannel))
            pServer->loop(spChannel);
}

Server::Server(){
}

Server::~Server() 
{
    if (spLogger_)
	   spLogger_->flush();
}

void Server::initLog()
{
    using namespace std::literals;

    spdlog::set_pattern("[%H:%M:%S] %v");
   
    constexpr const auto strSubFolderName("logs"sv);
    constexpr const auto logFileName("msg_log.log"sv);  

    if (!fsys::exists(strSubFolderName))
            fsys::create_directory(strSubFolderName);

    fsys::path logSubFolder(strSubFolderName);
    fsys::path relativeLogFilePath = logSubFolder / fsys::path(logFileName);

    const std::string strRelativeName(relativeLogFilePath.string());

    spLogger_ = spdlog::daily_logger_st("server_logger", strRelativeName);
    spLogger_.get()->name();

    const std::time_t curr_t = std::time(0);
    const std::tm* curr_tm = std::localtime(&curr_t);

    strLogFileName_ = spdlog::sinks::daily_filename_calculator::calc_filename(strRelativeName, *curr_tm);
}

void Server::loop(std::unique_ptr<Channel>& spChannel)
{
    using namespace std::chrono_literals;

    while(!gl_getCancelStatus())
    {
            if (bShutDownPending_)
				break;

            if ( spChannel->waitForMsg(20ms) )
                 spChannel->popMsg(this);
            else
                onIdle();    
    }
}

void Server::onMsg(const Msg& msg) 
{
    msgCount_++;
    logMsg(msg);
}

void Server::onIdle()
{
}

void Server::logMsg(const Msg& msg)
{
    if (spLogger_)
    {
        // for time of logging used ticks since app started:
        const auto ticks = getTicksSinceAppStart();

        const TDATA& content = msg.getContent();
        spLogger_->info("client id: {}, priority: {}, msg creation time: {}, msg logging time: {}",
                        content.dwClientId, content.cPriority, content.dwTicks, ticks);
    }
}
