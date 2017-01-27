#ifndef LSFILESASYNC_H
#define LSFILESASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>
#include <Connection.h>

class lsFilesAsync : public QThread
{
    Q_OBJECT
public:
    lsFilesAsync(QObject *parent = NULL);

    void listFiles(p7_handle_t *_handle, QString memory);

signals:
    void listed(const FileInfoList& lst, int err);

protected:
    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
};


#endif // LSFILESASYNC_H
