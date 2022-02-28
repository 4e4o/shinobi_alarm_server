#include "MotionState.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Misc/Timer.hpp>
#include <Misc/Debug.hpp>

#include <fstream>
#include <iostream>

#define FILE_PREFIX     "md_"
#define PATH_PREFIX     "/dev/shm/streams/dropInEvents"

#define MOTION_INTERVAL_NO_EVENT_MAX_SEC    30
#define MOTION_FINAL_EVENT_SEC              5

namespace bpt = boost::posix_time;

MotionState::MotionState(boost::asio::io_context &io,
                         const std::string& dir,
                         const std::string& id)
    : StrandHolder(io),
      m_dir(dir),
      m_id(id),
      m_evensEndTimer(new Timer(io, MOTION_FINAL_EVENT_SEC)),
      m_eventsGoing(false) {
    m_evensEndTimer->setStrand(this, false);
    m_evensEndTimer->onTimeout.connect([this] (Timer*) {
        onEventsEndTick();
    });

    debug_print(boost::format("MotionState::MotionState %1%") % this);
}

MotionState::~MotionState() {
    debug_print(boost::format("MotionState::~MotionState %1%") % this);
}

void MotionState::trigger() {
    auto self = shared_from_this();
    post([self]() {
        self->triggerImpl();
    });
}

void MotionState::onEventsEndTick() {
    //debug_print(boost::format("MotionState::onEventsEndTick %1%") % this);
    sendEvent();
    m_eventsGoing = false;
}

void MotionState::triggerImpl() {
    if (m_eventsGoing) {
        auto now = bpt::second_clock::local_time();
        bpt::time_duration duration = now - m_lastEventTime;

        // если будут часто лететь motion эвенты,
        // то мы их будем разрежать до 1 эвента в 30 секунд
        if (duration.total_seconds() < MOTION_INTERVAL_NO_EVENT_MAX_SEC) {
            m_evensEndTimer->startTimer();
            return;
        }
    }

    sendEvent();
    m_evensEndTimer->startTimer();
    m_eventsGoing = true;
}

std::string MotionState::make_uuid() {
    return boost::lexical_cast<std::string>(m_generator());
}

std::string MotionState::makeUniqueFilename() {
    while (1) {
        std::string path = PATH_PREFIX + std::string("/") +
                m_dir + std::string("/") +
                m_id + std::string("/") + std::string(FILE_PREFIX) + make_uuid();

        if (!boost::filesystem::exists(path))
            return path;

        usleep(50000);
    }
}

void MotionState::sendEvent() {
    const std::string path = makeUniqueFilename();
    std::ofstream fout;
    fout.open(path, std::ios::binary | std::ios::out);

//    debug_print(boost::format("MotionState::sendEvent %1% %2%") % this % path);

    if (fout.is_open()) {
        char byte = 1;
        fout.write((char*) &byte, sizeof(byte));
//        debug_print(boost::format("MotionState file event created %1% %2%") % this % path);
        fout.close();
    }

    m_lastEventTime = bpt::second_clock::local_time();
}
