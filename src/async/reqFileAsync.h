#ifndef REQFILEASYNC_H
#define REQFILEASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>

class reqFileAsync : public QThread
{
    Q_OBJECT
public:
    reqFileAsync(QObject *parent = NULL);

    void reqFile(p7_handle_t *_handle, QString file, QString dir, QString mem);

signals:
    void received(int err);
    void downloadProgress(int,int);

protected:
    static reqFileAsync* _instance;
    static void handleProgress(p7ushort_t, p7ushort_t);

    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
    QString _dir;
    QString _filename;
};

#endif // REQFILEASYNC_H
