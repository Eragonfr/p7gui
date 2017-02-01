#ifndef FILEINFO_H
#define FILEINFO_H

#include <QList>

class FileInfo
{
public:

    FileInfo() = default;
    FileInfo(const QString &dir, const QString &file, unsigned int filesize);

    QString dir() const;
    QString filename() const;
    unsigned int filesize() const;

    bool isDir() const;

    bool operator<(const FileInfo &other) const;

private:
    QString _dir;
    QString _filename;
    unsigned int _filesize;
};

typedef QList<FileInfo> FileInfoList;

#endif // FILEINFO_H
