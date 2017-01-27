#ifndef CONNECTIONOPTIMIZETHREAD_H
#define CONNECTIONOPTIMIZETHREAD_H

#include <QThread>
#include <QMutex>
#include <libp7.h>


class ConnectionOptimizeThread : public QThread
{
    Q_OBJECT
public:
    ConnectionOptimizeThread(QObject *parent = NULL);

    void optimize(p7_handle_t *_handle);

signals:
    void optimized(p7_handle_t *_handle, int err);

protected:
    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
};

#endif // CONNECTIONOPTIMIZETHREAD_H
