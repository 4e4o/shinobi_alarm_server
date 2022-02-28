#ifndef MOTION_RECEIVER_H
#define MOTION_RECEIVER_H

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <Misc/StrandHolder.hpp>

class MotionState;
class IPCIntQueue;
class ShinobiConfig;

class MotionReceiver : public std::enable_shared_from_this<MotionReceiver>, public StrandHolder {
public:
    MotionReceiver(boost::asio::io_context&,
                   const ShinobiConfig*);
    ~MotionReceiver();

    void start();
    void stop();

private:
    void postReceive();
    void receive();

    typedef std::map<int, std::string> TCamMap;
    typedef std::map<int, std::shared_ptr<MotionState>> TStateMap;

    const ShinobiConfig* m_config;
    std::unique_ptr<IPCIntQueue> m_queue;
    TCamMap m_id;
    TStateMap m_states;
    std::atomic_flag m_stopped = ATOMIC_FLAG_INIT;
};

#endif /* MOTION_RECEIVER_H */
