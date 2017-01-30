#ifndef INITASYNC_H
#define INITASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>

class initAsync : public QThread
{
    Q_OBJECT
public:
    initAsync(QObject *parent = NULL);

    void initHandle(p7_handle_t *handle);

signals:
    void initialized(p7_handle_t *, int);

protected:
    void run();

private:
    QMutex _mutex;
    p7_handle_t* _handle;
};

#endif // INITASYNC_H
