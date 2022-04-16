#include "MotionReceiver.h"
#include "MotionState.h"
#include "Config/ShinobiConfig.hpp"
#include "Config/CameraConfig.hpp"

#include <AApplication.h>
#include <IPC/IntQueue.h>
#include <Misc/Debug.hpp>

using namespace std::literals::chrono_literals;

#define QUEUE_CHECK_INTERVAL    400ms

#define IPC_QUEUE_NAME  "shinobi_alarm_queue"

using namespace boost::asio;

MotionReceiver::MotionReceiver(io_context &io,
                               const ShinobiConfig* config)
    : CoroutineTask(io),
      m_config(config) {
    debug_print_this("");

    for (const auto& cam : config->getCameras()) {
        m_mappedToCamId[cam->mappedId()] = cam->camId();
    }
}

MotionReceiver::~MotionReceiver() {
    debug_print_this("");
}

TAwaitVoid MotionReceiver::run() {
    ScopeGuard exitApp([]() { AAP->quit(); });
    IPCIntQueue queue(IPC_QUEUE_NAME, false);

    while(running()) {
//        debug_print_this("before queue read");
        int chId = -1;
        const bool received = queue.try_receive(chId);
//        debug_print_this(fmt("after queue read %1% %2%") % chId % received);

        if (!received) {
            co_await wait(QUEUE_CHECK_INTERVAL);
        }

        if ((chId < 0) || !m_mappedToCamId.contains(chId))
            continue;

        std::shared_ptr<MotionState> state;

        if (m_states.contains(chId))
            state = m_states[chId];

        if (state.get() == nullptr) {
            state.reset(new MotionState(io(), m_config->getId(), m_mappedToCamId[chId]));
            m_states[chId] = state;
        }

        state->trigger();
    }

    co_return;
}
