#ifndef P7ASYNC_H
#define P7ASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>

class P7Async : public QThread
{
    Q_OBJECT
public:
    P7Async(QObject *parent = NULL);

    void optimize(p7_handle_t *_handle);

signals:
    void optimized(p7_handle_t *_handle, int err);

protected:
    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
};

#endif // P7ASYNC_H
