#ifndef LSFILESASYNC_H
#define LSFILESASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>
#include <QList>

class FileInfo
{
public:

    FileInfo(QObject *parent = Q_NULLPTR);
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

class lsFilesAsync : public QThread
{
    Q_OBJECT
public:
    lsFilesAsync(QObject *parent = NULL);

    void listFiles(p7_handle_t *_handle, QString memory);

signals:
    void listed(FileInfoList lst, int err);

protected:
    void run();

private:
    static FileInfoList finfol;
    static void fileinfoCallback(const char *dir, const char *filename, p7uint_t filesize);

    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
};


#endif // LSFILESASYNC_H
