#ifndef ACTIVTIY_WATCHER_H
#define ACTIVTIY_WATCHER_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <set>

class IClosable;

class ActivityWatcher
        : public boost::enable_shared_from_this<ActivityWatcher> {
public:
    ActivityWatcher(boost::asio::io_context& io_context);
    static ActivityWatcher* get();

    void add(IClosable*);
    void remove(IClosable*);

    void trigger();

    ~ActivityWatcher();

private:
    void startActivityTimer();
    void onActivityTick(const boost::system::error_code& /*e*/);

    boost::asio::steady_timer m_activityTimer;
    std::set<IClosable*> m_objs;
    bool m_activityTriggered;
};

#endif /* ACTIVTIY_WATCHER_H*/
