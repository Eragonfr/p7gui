#include "ConnectionOptimizeThread.h"
#include <QtDebug>

ConnectionOptimizeThread::ConnectionOptimizeThread(QObject *parent): QThread(parent)
{

}

void ConnectionOptimizeThread::optimize(p7_handle_t *handle)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    start(QThread::NormalPriority);
}

void ConnectionOptimizeThread::run()
{
    _mutex.lock();
    int err = p7_optimize(_handle, "fls0");
    _mutex.unlock();
    emit optimized(_handle, err);
}
