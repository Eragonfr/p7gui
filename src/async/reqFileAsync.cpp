#include "reqFileAsync.h"

#include <QFileInfo>

reqFileAsync::reqFileAsync(QObject *parent): QThread(parent)
{

}

void reqFileAsync::reqFile(p7_handle_t *handle, QString file, QString dir, QString mem)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _dir = dir;
    _filename = file;
    _mem = mem;
    start(QThread::NormalPriority);
}

void reqFileAsync::run()
{
    _mutex.lock();

    QFileInfo finfo(_filename);
    FILE *fp = fopen(_filename.toStdString().c_str(), "w");
    int err = p7_reqfile(_handle, fp, _dir.toStdString().c_str(), finfo.fileName().toStdString().c_str(),
                         _mem.toStdString().c_str(), &reqFileAsync::handleProgress);
    _mutex.unlock();
    emit received(err);
}

reqFileAsync *reqFileAsync::_instance = NULL;

void reqFileAsync::handleProgress(p7ushort_t p, p7ushort_t tot)
{
    emit _instance->downloadProgress(p, tot);
}
