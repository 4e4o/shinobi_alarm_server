#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <BaseConfigApplication.h>

#include <boost/signals2.hpp>

class MotionReceiver;

class Application : public BaseConfigApplication {
public:
    Application(int argc, char* argv[]);
    ~Application();

private:
    bool start(TConfigItems&) override final;
    void doExit() override final;

    std::shared_ptr<MotionReceiver> m_mr;
};

#endif /* APPLICATION_HPP */
