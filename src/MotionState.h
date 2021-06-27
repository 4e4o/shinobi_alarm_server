#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include <boost/asio.hpp>

class MotionState {
public:
    MotionState(boost::asio::io_context&,
                const std::string& dir,
                const std::string& id);
    ~MotionState();

    void trigger();

private:
    void sendEvent();
    void triggerImpl();
    void onEventsEndTick(const boost::system::error_code& e);
    void restartEventsEndTimer();
    std::string makeUniqueFilename();

    boost::asio::io_context& m_context;
    std::string m_dir;
    std::string m_id;
    boost::posix_time::ptime m_lastEventTime;
    boost::asio::steady_timer m_evensEndTimer;
    bool m_eventsGoing;
};


#endif /* MOTION_STATE_H */
