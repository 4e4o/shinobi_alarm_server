#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <boost/interprocess/ipc/message_queue.hpp>

class MessageQueue {
public:
    MessageQueue(bool child = true);
    ~MessageQueue();

    void send(int chid);
    int receive();

private:
    std::unique_ptr<boost::interprocess::message_queue> m_queue;
    bool m_parent;
};

#endif /* MESSAGE_QUEUE_H */
