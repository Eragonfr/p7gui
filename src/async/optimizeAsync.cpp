#include "optimizeAsync.h"

optimizeAsync::optimizeAsync(QObject *parent): QThread(parent)
{

}

void optimizeAsync::optimize(p7_handle_t *handle)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    start(QThread::NormalPriority);
}

void optimizeAsync::run()
{
    _mutex.lock();
    int err = p7_optimize(_handle, "fls0");
    _mutex.unlock();
    emit optimized(err);
}
