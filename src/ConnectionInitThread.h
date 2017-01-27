#ifndef CONNECTIONINITTHREAD_H
#define CONNECTIONINITTHREAD_H

#include <QThread>

#include <QMutex>
#include <libp7.h>

class ConnectionInitThread : public QThread
{
    Q_OBJECT
public:
    ConnectionInitThread(QObject *parent = NULL);

    void initHandle(p7_handle_t *_handle);

signals:
    void initialized(p7_handle_t *_handle, int err);

protected:
    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
};

#endif // CONNECTIONINITTHREAD_H
