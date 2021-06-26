#ifndef MOTION_RECEIVER_H
#define MOTION_RECEIVER_H

#include <boost/asio.hpp>

class MotionState;

class MotionReceiver {
public:
    typedef std::map<int, std::string> TCamMap;

    MotionReceiver(boost::asio::io_context&,
                   const std::string&,
                   const TCamMap &id);

    void start();

private:
    typedef std::map<int, MotionState*> TStateMap;
    boost::asio::io_context& m_context;
    std::unique_ptr<std::thread> m_thread;
    std::string m_acId;
    TCamMap m_id;
    TStateMap m_states;
};

#endif /* MOTION_RECEIVER_H */
