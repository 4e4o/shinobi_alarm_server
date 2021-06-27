#include "MotionState.h"

#include <boost/filesystem.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>
#include <iostream>

#define FILE_PREFIX     "md_"
#define PATH_PREFIX     "/dev/shm/streams/dropInEvents"

#define MOTION_INTERVAL_NO_EVENT_MAX_SEC    30
#define MOTION_FINAL_EVENT_SEC              5

namespace bpt = boost::posix_time;

MotionState::MotionState(boost::asio::io_context &c,
                         const std::string& dir,
                         const std::string& id)
    : m_context(c),
      m_dir(dir),
      m_id(id),
      m_evensEndTimer(c),
      m_eventsGoing(false) {
}

MotionState::~MotionState() {
}

void MotionState::trigger() {
    boost::asio::post(m_context.get_executor(), [this]() {
        triggerImpl();
    });
}

void MotionState::onEventsEndTick(const boost::system::error_code& e) {
    if (e)
        return;

    sendEvent();
    m_eventsGoing = false;
}

void MotionState::restartEventsEndTimer() {
    m_evensEndTimer.expires_from_now(boost::asio::chrono::seconds(MOTION_FINAL_EVENT_SEC));
    m_evensEndTimer.async_wait(boost::bind(&MotionState::onEventsEndTick,
                                           this,
                                           boost::asio::placeholders::error));
}

void MotionState::triggerImpl() {
    if (m_eventsGoing) {
        auto now = bpt::second_clock::local_time();
        bpt::time_duration duration = now - m_lastEventTime;

        // если будут часто лететь motion эвенты,
        // то мы их будем разрежать до 1 эвента в 30 секунд
        if (duration.total_seconds() < MOTION_INTERVAL_NO_EVENT_MAX_SEC) {
            restartEventsEndTimer();
            return;
        }
    }

    sendEvent();
    restartEventsEndTimer();
    m_eventsGoing = true;
}


static std::string make_uuid() {
    return boost::lexical_cast<std::string>((boost::uuids::random_generator())());
}

std::string MotionState::makeUniqueFilename() {
    while (1) {
        std::string path = PATH_PREFIX + std::string("/") +
                m_dir + std::string("/") +
                m_id + std::string("/") + std::string(FILE_PREFIX) + make_uuid();

        if (!boost::filesystem::exists(path))
            return path;

        usleep(100000);
    }
}

void MotionState::sendEvent() {
    const std::string path = makeUniqueFilename();
    std::ofstream fout;
    fout.open(path, std::ios::binary | std::ios::out);

    if (fout.is_open()) {
        char byte = 1;
        fout.write((char*) &byte, sizeof(byte));
//        std::cout << "MotionState file event created " << this << " , " << path << std::endl;
        fout.close();
    }

    m_lastEventTime = bpt::second_clock::local_time();
}
