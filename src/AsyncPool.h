#ifndef ASYNCPOOL_H
#define ASYNCPOOL_H

#include <QObject>



class AsyncPool: public QObject
{
    Q_OBJECT
public:
    AsyncPool(QObject* parent);

    void addThread(QThread* thread, const QString& msgStart = QString(), const QString& msgFinished = QString());
    bool isRunning() const;

signals:
    void started(QString);
    void finished(QString);

private slots:
    void next();

private:
    QList<QThread*> _pool;
    QThread* _current;
    QStringList _msgStart;
    QStringList _msgFinished;

};

#endif // ASYNCPOOL_H
