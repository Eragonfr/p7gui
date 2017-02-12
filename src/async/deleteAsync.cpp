#include "deleteAsync.h"

#include <QDebug>

deleteAsync::deleteAsync(QObject *parent): QThread (parent)
{

}

void deleteAsync::deleteFile(p7_handle_t *handle, QString memory, QString dir, QString filename)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
    _dir = dir;
    _filename = filename;
}

void deleteAsync::run()
{
    _mutex.lock();
    int err = p7_delete(_handle,
                        _dir.isEmpty() ? NULL : _dir.toStdString().c_str(),
                        _filename.isEmpty() ? NULL :_filename.toStdString().c_str(),
                         _mem.toStdString().c_str());
    _mutex.unlock();
    emit deleted(err);
}
