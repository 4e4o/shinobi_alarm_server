#include "MotionReceiver.h"
#include "MessageQueue.h"
#include "MotionState.h"

#include <iostream>

using namespace boost::interprocess;

MotionReceiver::MotionReceiver(boost::asio::io_context &c,
                               const std::string& acId,
                               const TCamMap &id)
    : m_context(c),
      m_acId(acId),
      m_id(id) {
}

void MotionReceiver::start() {
    m_thread.reset(new std::thread([this] () {
        try {
            MessageQueue mq(false);

            while(1) {
                const int chId = mq.receive();

                if (chId < 0) {
                    std::cout << "received -1 chId" << std::endl;
                    return;
                }

                if (!m_id.contains(chId))
                    continue;

                MotionState *state = nullptr;

                if (m_states.contains(chId))
                    state = m_states[chId];

                if (state == nullptr) {
                    state = new MotionState(m_context, m_acId, m_id[chId]);
                    m_states[chId] = state;
                }

                state->trigger();
            }
        } catch(interprocess_exception &ex) {
            std::cout << ex.what() << std::endl;
            return;
        }
    }));
}
