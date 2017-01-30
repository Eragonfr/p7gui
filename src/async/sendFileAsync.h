#ifndef SENFFILEASYNC_H
#define SENFFILEASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>

class sendFileAsync : public QThread
{
    Q_OBJECT
public:
    sendFileAsync(QObject *parent = NULL);

    void sendFile(p7_handle_t *_handle, QString file, QString dir, QString mem);

signals:
    void sent(int err);
    void uploadProgress(int,int);

protected:
    static sendFileAsync* _instance;
    static void handleProgress(p7ushort_t, p7ushort_t);

    void run();
    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
    QString _dir;
    QString _filename;
};

#endif // SENFFILEASYNC_H
