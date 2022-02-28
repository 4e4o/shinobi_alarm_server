#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include <Misc/StrandHolder.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid_generators.hpp>

class Timer;

class MotionState : public std::enable_shared_from_this<MotionState>, public StrandHolder {
public:
    MotionState(boost::asio::io_context&,
                const std::string& dir,
                const std::string& id);
    ~MotionState();

    void trigger();

private:
    void sendEvent();
    void triggerImpl();
    void onEventsEndTick();
    std::string makeUniqueFilename();
    std::string make_uuid();

    std::string m_dir;
    std::string m_id;
    boost::posix_time::ptime m_lastEventTime;
    std::unique_ptr<Timer> m_evensEndTimer;
    bool m_eventsGoing;
    boost::uuids::random_generator m_generator;
};


#endif /* MOTION_STATE_H */
