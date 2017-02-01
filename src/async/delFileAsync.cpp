#include "delFileAsync.h"

#include <QDebug>

delFileAsync::delFileAsync(QObject *parent): QThread (parent)
{

}

void delFileAsync::deleteFile(p7_handle_t *handle, QString memory, QString dir, QString filename)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
    _dir = dir;
    _filename = filename;
}

void delFileAsync::run()
{
    _mutex.lock();
    int err = p7_delfile(_handle,
                         _filename.isEmpty() ? NULL : _dir.toStdString().c_str(),
                         _filename.isEmpty() ? _dir.toStdString().c_str() : _filename.toStdString().c_str(),
                         _mem.toStdString().c_str());
    _mutex.unlock();
    emit deleted(err);
}
