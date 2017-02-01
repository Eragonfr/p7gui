#include "FileInfo.h"

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
