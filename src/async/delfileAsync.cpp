#include "delfileAsync.h"

delfileAsync::delfileAsync(QObject *parent): QThread (parent)
{

}

void delfileAsync::deleteFile(p7_handle_t *handle, QString memory, QString dir, QString filename)
{

    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
    _dir = dir;
    _filename = filename;
    start(QThread::NormalPriority);
}

void delfileAsync::run()
{
    _mutex.lock();
    int err = p7_delfile(_handle, _dir.toStdString().c_str(), _filename.toStdString().c_str(),
                         _mem.toStdString().c_str());
    _mutex.unlock();
    emit listed(finfol, err);
}
