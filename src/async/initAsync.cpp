#include "initAsync.h"

initAsync::initAsync(QObject *parent): QThread (parent)
{

}

void initAsync::initHandle(p7_handle_t *handle)
{

    QMutexLocker locker(&_mutex);
    _handle = handle;
}

void initAsync::run()
{
    _mutex.lock();
    int err = p7_init(&_handle, 1, 1);
    _mutex.unlock();
    emit initialized(_handle, err);
}
