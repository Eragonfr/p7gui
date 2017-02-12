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
    int err = p7_init(&_handle, P7_ACTIVE | P7_CHECK | P7_TERM);
    _mutex.unlock();
    emit initialized(_handle, err);
}
