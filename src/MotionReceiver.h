#ifndef MOTION_RECEIVER_H
#define MOTION_RECEIVER_H

#include <Coroutine/CoroutineTask.hpp>

#include <map>

class MotionState;
class ShinobiConfig;

class MotionReceiver : public CoroutineTask<void> {
public:
    MotionReceiver(boost::asio::io_context&,
                   const ShinobiConfig*);
    ~MotionReceiver();

private:
    TAwaitVoid run() override final;

    typedef std::unordered_map<int, std::string> TCamMap;
    typedef std::unordered_map<int, std::shared_ptr<MotionState>> TStateMap;

    const ShinobiConfig* m_config;
    TCamMap m_mappedToCamId;
    TStateMap m_states;
};

#endif /* MOTION_RECEIVER_H */
