#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include <Coroutine/TimedSpawn.hpp>

#include <boost/uuid/uuid_generators.hpp>

class MotionState : public TimedCoroutineSpawn {
public:
    MotionState(boost::asio::io_context&,
                const std::string& dir,
                const std::string& id);
    ~MotionState();

    void trigger();

private:
    void sendEvent();
    std::string makeUniqueFilename();
    std::string make_uuid();

    std::string m_dir;
    std::string m_id;
    bool m_eventsGoing;
    bool m_detectingContinuity;
    boost::uuids::random_generator m_generator;
};


#endif /* MOTION_STATE_H */
