#include <AlarmManager.hpp>

#include "../../../../src/MessageQueue.h"

#include <boost/json.hpp>

class ChannelConfigEx : public ChannelConfig {
public:
    ChannelConfigEx(const boost::json::object& o)
        : ChannelConfig(o), m_mappedId(-1) {
        if (o.contains("mapped_id"))
            m_mappedId = o.at("mapped_id").as_int64();
    }

    int m_mappedId;
};

int main(int argc, char **argv) {
    std::unique_ptr<MessageQueue> queue;
    std::unique_ptr<AlarmManager> am(new AlarmManager(argc, argv));
    am->registerType<ChannelConfig, ChannelConfigEx, const boost::json::object&>();
    using TMD = AlarmManager::TMotion;

    am->setMotionCallback([&queue] (AlarmClient*, const TMD& md, const size_t& size) {
        for (size_t i = 0 ; i < size ; i++) {
            ChannelConfigEx* c = static_cast<ChannelConfigEx*>(md[i]);

            if (c->m_mappedId == -1)
                continue;

            queue->send(c->m_mappedId);
        }
    });

    return am->run();
}
