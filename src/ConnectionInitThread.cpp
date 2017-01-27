#include "ConnectionInitThread.h"

ConnectionInitThread::ConnectionInitThread(QObject *parent): QThread(parent)
{

}

void ConnectionInitThread::initHandle(p7_handle_t *handle)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    start(QThread::NormalPriority);
}

void ConnectionInitThread::run()
{
    _mutex.lock();
    int err = p7_init(&_handle, 1, 1);
    _mutex.unlock();
    emit initialized(_handle, err);
}
