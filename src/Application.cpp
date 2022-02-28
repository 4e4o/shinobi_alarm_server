#include "Application.h"
#include "Config/ShinobiConfig.hpp"
#include "MotionReceiver.h"

#include <Config/Config.hpp>

#define PROG_NAME   "shinobi_alarm_server"

Application::Application(int argc, char* argv[])
    : BaseConfigApplication(PROG_NAME, argc, argv) {
    config()->registerType<ConfigItem, ShinobiConfig, const boost::json::object&>();
}

Application::~Application() {
}

void Application::doExit() {
    m_mr->stop();
}

bool Application::start(TConfigItems &cfg) {
    if (cfg.size() != 1)
        return false;

    m_mr.reset(new MotionReceiver(io(), static_cast<ShinobiConfig*>(cfg.front().get())));
    m_mr->start();
    return true;
}
