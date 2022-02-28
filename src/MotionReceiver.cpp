#include "MotionReceiver.h"
#include "MotionState.h"
#include "Config/ShinobiConfig.hpp"
#include "Config/CameraConfig.hpp"

#include <IPC/IntQueue.h>
#include <Misc/ScopeGuard.hpp>
#include <Misc/Debug.hpp>

#include <iostream>

using namespace boost::interprocess;

#define IPC_QUEUE_NAME  "shinobi_alarm_queue"

MotionReceiver::MotionReceiver(boost::asio::io_context &io,
                               const ShinobiConfig* config)
    : StrandHolder(io),
      m_config(config),
      m_queue(new IPCIntQueue(IPC_QUEUE_NAME, false)) {
    for (const auto& cam : config->getCameras()) {
        m_id[cam->mappedId()] = cam->camId();
    }

    debug_print(boost::format("MotionReceiver::MotionReceiver %1%") % this);
}

MotionReceiver::~MotionReceiver() {
    debug_print(boost::format("MotionReceiver::~MotionReceiver %1%") % this);
}

void MotionReceiver::start() {
    postReceive();
}

void MotionReceiver::stop() {
    if (m_stopped.test_and_set())
        return;

    auto self = shared_from_this();
    boost::asio::post([self] {
        self->m_queue->send(-1);
    });
}

void MotionReceiver::postReceive() {
    auto self = shared_from_this();
    post([self] {
        self->receive();
    });
}

void MotionReceiver::receive() {
    if (m_stopped.test())
        return;

    ScopeGuard s([this]() {
        postReceive();
    });

    try {
        const int chId = m_queue->receive();

        if (chId < 0 || !m_id.contains(chId))
            return;

        std::shared_ptr<MotionState> state;

        if (m_states.contains(chId))
            state = m_states[chId];

        if (state.get() == nullptr) {
            state.reset(new MotionState(io(), m_config->getId(), m_id[chId]));
            m_states[chId] = state;
        }

        state->trigger();
    } catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
    }
}
