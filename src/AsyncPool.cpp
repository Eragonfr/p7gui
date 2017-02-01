#include "AsyncPool.h"

#include <QThread>
#include <QDebug>

AsyncPool::AsyncPool(QObject *parent): QObject(parent),
    _current(NULL)
{

}

void AsyncPool::addThread(QThread *thread, const QString &msgStart, const QString &msgFinished)
{
    _pool.append(thread);
    _msgStart.append(msgStart);
    _msgFinished.append(msgFinished);
    if(!_current || !_current->isRunning())
        next();
}

bool AsyncPool::isRunning() const
{
    return _current && _current->isRunning();
}

void AsyncPool::next()
{
    if(_current) {
        if(_current->isRunning())
            return;
        emit finished(_msgFinished.takeFirst());
        _current->deleteLater();
        _current = NULL;
    }
    if(_pool.isEmpty())
        return;
    _current = _pool.takeFirst();
    connect(_current, SIGNAL(finished()), this, SLOT(next()));
    emit started(_msgStart.takeFirst());
    _current->start();
}
