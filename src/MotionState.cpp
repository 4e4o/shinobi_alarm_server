#include "MotionState.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Coroutine/Awaitables.hpp>
#include <Misc/Debug.hpp>

#include <fstream>
#include <iostream>

#define FILE_PREFIX     "md_"
#define PATH_PREFIX     "/dev/shm/streams/dropInEvents"

using namespace std::literals::chrono_literals;

#define CONTINUITY_INTERVAL         5s
#define CONTINUITY_FLUSH_INTERVAL   (CONTINUITY_INTERVAL * 6)

MotionState::MotionState(boost::asio::io_context &io,
                         const std::string& dir,
                         const std::string& id)
    : TimedCoroutineSpawn(io),
      m_dir(dir),
      m_id(id),
      m_eventsGoing(false),
      m_detectingContinuity(false) {
    debug_print_this("");
}

MotionState::~MotionState() {
    debug_print_this("");
}

void MotionState::trigger() {
    spawn<true>([this]() -> TAwaitVoid {
        // схема такая:
        // если эвенты сплошняком идут то мы их разряжаем до 1 эвента в CONTINUITY_FLUSH_INTERVAL секунд.
        // сплошняком идут это значит что интервал между эвентами не превышает CONTINUITY_INTERVAL секунд.

        if (m_detectingContinuity) {
            debug_print_this("m_detectingContinuity true");
            m_eventsGoing = true;
        } else {
            debug_print_this("m_detectingContinuity started");

            // шлём начальный эвент
            sendEvent();

            // начинаем детектить сплошняк:
            m_detectingContinuity = true;
            std::chrono::seconds waitTime{0};

            do {
                m_eventsGoing = false;
                co_await wait(CONTINUITY_INTERVAL);
                waitTime += CONTINUITY_INTERVAL;

                if (m_eventsGoing && (waitTime >= CONTINUITY_FLUSH_INTERVAL)) {
                    debug_print_this("m_detectingContinuity cycle flush");
                    sendEvent();
                    waitTime = 0s;
                }
            } while(m_eventsGoing);

            // шлём заключительный эвент в сплошняке.
            if (waitTime > CONTINUITY_INTERVAL) {
                debug_print_this("m_detectingContinuity cycle finish event");
                sendEvent();
            }

            debug_print_this("m_detectingContinuity end");
            m_detectingContinuity = false;
        }

        co_return;
    });
}

std::string MotionState::make_uuid() {
    return boost::lexical_cast<std::string>(m_generator());
}

std::string MotionState::makeUniqueFilename() {
    while(1) {
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

    debug_print_this(fmt("%1%") % path);

    if (fout.is_open()) {
        char byte = 1;
        fout.write((char*) &byte, sizeof(byte));
        //        debug_print_this(fmt("file event created %1%") % path);
        fout.close();
    }
}
