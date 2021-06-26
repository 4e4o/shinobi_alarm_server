#include "ActivityWatcher.hpp"
#include "IClosable.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>

#define CHECK_INTERVAL_SEC 20

static ActivityWatcher* g_aw = nullptr;

ActivityWatcher::ActivityWatcher(boost::asio::io_context& io_context)
    : m_activityTimer(io_context, boost::asio::chrono::seconds(CHECK_INTERVAL_SEC)),
      m_activityTriggered(false) {
    g_aw = this;
    startActivityTimer();
}

ActivityWatcher* ActivityWatcher::get() {
    return g_aw;
}

void ActivityWatcher::startActivityTimer() {
    m_activityTimer.expires_from_now(boost::asio::chrono::seconds(CHECK_INTERVAL_SEC));
    m_activityTimer.async_wait(boost::bind(&ActivityWatcher::onActivityTick,
                                           this,
                                           boost::asio::placeholders::error));
}

void ActivityWatcher::onActivityTick(const boost::system::error_code& e) {
    if (e)
        return;

    if (!m_activityTriggered) {
        std::cout << "Stale detected, closing " << this << std::endl;

        for(IClosable* obj : m_objs)
            obj->close();

        return;
    }

//    std::cout << "onActivityTick, good " << " " << this << std::endl;

    m_activityTriggered = false;
    startActivityTimer();
}

void ActivityWatcher::add(IClosable* o) {
    m_objs.insert(o);
}

void ActivityWatcher::remove(IClosable* o) {
    m_objs.erase(o);
}

void ActivityWatcher::trigger() {
    m_activityTriggered = true;
}

ActivityWatcher::~ActivityWatcher() {
}
