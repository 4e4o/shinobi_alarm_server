#include "MessageQueue.h"

#define IPC_QUEUE_NAME      "shinobi_alarm_queue"
#define QUEUE_SIZE          1024 * 10

using namespace boost::interprocess;

MessageQueue::MessageQueue(bool child)
    : m_parent(!child) {
    if (child) {
        m_queue.reset(new message_queue(open_only, IPC_QUEUE_NAME));
    } else {
        message_queue::remove(IPC_QUEUE_NAME);
        m_queue.reset(new message_queue(create_only, IPC_QUEUE_NAME, QUEUE_SIZE, sizeof(int)));
    }
}

MessageQueue::~MessageQueue() {
    if (m_parent)
        message_queue::remove(IPC_QUEUE_NAME);
}

void MessageQueue::send(int chid) {
    m_queue->send(&chid, sizeof(chid), 0);
}

int MessageQueue::receive() {
    message_queue::size_type recvd_size;
    unsigned int priority;
    int chId;
    m_queue->receive(&chId, sizeof(chId), recvd_size, priority);

    if(recvd_size != sizeof(chId))
        return -1;

    return chId;
}
