#include "listAsync.h"

#include <functional>

listAsync::listAsync(QObject *parent): QThread(parent)
{
    qRegisterMetaType<FileInfoList>("FileInfoList");
}

void listAsync::listFiles(p7_handle_t *handle, QString memory)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
}

FileInfoList listAsync::finfol = FileInfoList();

void listAsync::fileinfoCallback(void *cookie, const char *dir, const char *filename, p7uint_t filesize)
{
    finfol.append(FileInfo(dir, filename, filesize));
}

void listAsync::run()
{
    finfol.clear();

    _mutex.lock();
    int err = p7_list(_handle, _mem.toStdString().c_str(), fileinfoCallback, NULL);
    _mutex.unlock();
    emit listed(finfol, err);
}
