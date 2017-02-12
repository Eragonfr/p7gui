#ifndef LSFILESASYNC_H
#define LSFILESASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>

#include <FileInfo.h>

class listAsync : public QThread
{
    Q_OBJECT
public:
    listAsync(QObject *parent = NULL);

    void listFiles(p7_handle_t *_handle, QString memory);

signals:
    void listed(FileInfoList lst, int err);

protected:
    void run();

private:
    static FileInfoList finfol;
    static void fileinfoCallback(void *cookie, const char *dir, const char *filename,
                                 p7uint_t filesize);

    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
};


#endif // LSFILESASYNC_H
