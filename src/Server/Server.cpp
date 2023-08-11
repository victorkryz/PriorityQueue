/**
 * PriorityQueue
 */

#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>   
#include <random>
#include <experimental/filesystem>
#include "Server.h"

namespace fsys = std::experimental::filesystem;

extern bool gl_getCancelStatus();
extern long long getTicksSinceAppStart();

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

void Server::run(Server* pServer, Channel* pChannel) 
{
    if ( (nullptr != pServer) &&
                    (nullptr != pChannel))
            pServer->loop(pChannel);
}

Server::Server(){
}

Server::~Server() {
	spLogger_->flush();
}

void Server::initLog()
{
    // there's used third-party code "spdlog" for logging (see: https://github.com/gabime/spdlog)
    // init logger

    spdlog::set_pattern("[%H:%M:%S] %v");

    std::time_t currTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string strFileNameSfx(std::ctime(&currTime));

    using namespace std;

    const std::string strSubFolderName("logs");
    std::string logFileName("msg_log");  
    std::stringstream strstr;
    logFileName += strstr.str();
    logFileName += ".log";

    if (!fsys::exists(strSubFolderName))
            fsys::create_directory(strSubFolderName);

    fsys::path logSubFolder(strSubFolderName);
    fsys::path relativeLogFilePath = logSubFolder / fsys::path(logFileName);

    const std::string strRelativeName(relativeLogFilePath.string());

    spLogger_ = spdlog::daily_logger_st("server_logger", strRelativeName);
    spLogger_.get()->name();

    strLogFileName_ = spdlog::sinks::default_daily_file_name_calculator::calc_filename(strRelativeName);
}

void Server::loop(Channel* pChannel)
{
    while(!gl_getCancelStatus())
    {
		if (!pChannel->popMsg(this) )
		{
			onIdle();

			if (bShutDownPending_)
				break;
		}
    }
}

void Server::onMsg(const Msg& msg) 
{
    msgCount_++;
    logMsg(msg);
}

void Server::onIdle()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
}

void Server::logMsg(const Msg& msg)
{
    // for time of logging used ticks since app started:
    const auto ticks = getTicksSinceAppStart();

    const TDATA& content = msg.getContent();
    spLogger_->info("client id: {}, priority: {}, msg creation time: {}, msg logging time: {}",
                    content.dwClientId, content.cPriority, content.dwTicks, (DWORD)ticks);
}
