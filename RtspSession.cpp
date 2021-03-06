#include <iostream>

#include "RtspSession.h"
#include "RtspClient.h"
#include "RtspCallback.h"



#define RTSP_CLIENT_VERBOSITY_LEVEL 1


RtspSession::RtspSession(int id, const char* progName, const char* url, QObject* parent)
: QThread(parent)
, _id(id)
, _progName(progName)
, _url(url)
, _rtspClient(nullptr)
, _eventLoopWatchVariable(0)
{
}

RtspSession::~RtspSession()
{
    stop();
    quit();
    wait();
}

void RtspSession::stop()
{
    this->_eventLoopWatchVariable = 1;
}

void RtspSession::run()
{
    this->_eventLoopWatchVariable = 0;

    TaskScheduler* scheduler= BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

    if(!this->openURL(*env, this->_progName.c_str(), this->_url.c_str())){
        return;
    }

    env->taskScheduler().doEventLoop(&this->_eventLoopWatchVariable);
    std::cout << "Rtsp event finished" << std::endl;
    env->reclaim();
    env = nullptr;
    delete scheduler;
    scheduler = nullptr;
}

bool RtspSession::openURL(UsageEnvironment &env, const char *progName, const char *url)
{
    this->_rtspClient = CustomRTSPClient::createNew(this, env, url,
                                                    RTSP_CLIENT_VERBOSITY_LEVEL, progName);
    if (this->_rtspClient == nullptr) {
        std::cerr << "Failed to create a RTSP client for URL \""
                  << this->_url << "\": "
                  << env.getResultMsg() << "\n";
        return false;
    }

    this->_rtspClient->sendDescribeCommand(continueAfterDESCRIBE);

    return true;
}

