#include "lsFilesAsync.h"

lsFilesAsync::lsFilesAsync(QObject *parent): QThread(parent)
{

}

void lsFilesAsync::listFiles(p7_handle_t *handle, QString memory)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
    start(QThread::NormalPriority);
}

void lsFilesAsync::run()
{
    _mutex.lock();

    FileInfoList finfol;
    int err = p7_lsfiles(_handle, _mem.toStdString().c_str(), [](const char *dir, const char *filename, p7uint_t filesize) {
        finfol.append(FileInfo(dir, filename, filesize));
    });
    _mutex.unlock();
    emit listed(finfol, err);
}

