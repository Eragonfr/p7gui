#include "lsFilesAsync.h"

#include <functional>

lsFilesAsync::lsFilesAsync(QObject *parent): QThread(parent)
{
    qRegisterMetaType<FileInfoList>("FileInfoList");
}

void lsFilesAsync::listFiles(p7_handle_t *handle, QString memory)
{
    QMutexLocker locker(&_mutex);
    _handle = handle;
    _mem = memory;
    start(QThread::NormalPriority);
}

FileInfoList lsFilesAsync::finfol = FileInfoList();

void lsFilesAsync::fileinfoCallback(const char *dir, const char *filename, p7uint_t filesize)
{
    finfol.append(FileInfo(dir, filename, filesize));
}

void lsFilesAsync::run()
{
    finfol.clear();

    _mutex.lock();
    int err = p7_lsfiles(_handle, _mem.toStdString().c_str(), fileinfoCallback);
    _mutex.unlock();
    emit listed(finfol, err);
}

FileInfo::FileInfo(const QString &dir, const QString &file, unsigned int filesize):
    _dir(dir),
    _filename(file),
    _filesize(filesize)
{

}

QString FileInfo::dir() const
{
    return _dir;
}

QString FileInfo::filename() const
{
    return _filename;
}

unsigned int FileInfo::filesize() const
{
    return _filesize;
}

bool FileInfo::isDir() const
{
    return _filename.isEmpty();
}

bool FileInfo::operator<(const FileInfo &other) const
{
    return _dir + "/" + _filename < other._dir + "/" + other._filename;
}

