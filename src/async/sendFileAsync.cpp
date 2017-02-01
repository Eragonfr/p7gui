#include "sendFileAsync.h"

#include <QFileInfo>

sendFileAsync::sendFileAsync(QObject *parent): QThread(parent)
{

}

void sendFileAsync::sendFile(p7_handle_t *handle, QString file, QString dir, QString mem)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _dir = dir;
    _filename = file;
    _mem = mem;
}

void sendFileAsync::run()
{
    _mutex.lock();
    _instance = this;
    QFileInfo finfo(_filename);
    FILE *fp = fopen(_filename.toStdString().c_str(), "r");
    int err = p7_sendfile(_handle, fp, _dir.toStdString().c_str(),
                          finfo.fileName().toStdString().c_str(), _mem.toStdString().c_str(), 1, NULL, &sendFileAsync::handleProgress);
    _instance = NULL;
    _mutex.unlock();
    emit sent(err);
}

sendFileAsync *sendFileAsync::_instance = NULL;

void sendFileAsync::handleProgress(p7ushort_t p, p7ushort_t tot)
{
    emit _instance->uploadProgress(p, tot);
}
