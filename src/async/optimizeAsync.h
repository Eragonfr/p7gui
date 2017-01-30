#ifndef OPTIMIZEASYNC_H
#define OPTIMIZEASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>


class optimizeAsync : public QThread
{
    Q_OBJECT
public:
    optimizeAsync(QObject *parent = NULL);

    void optimize(p7_handle_t *_handle);

signals:
    void optimized(int err);

protected:
    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
};

#endif // OPTIMIZEASYNC_H
