#ifndef DELFILEASYNC_H
#define DELFILEASYNC_H

#include <QThread>
#include <QMutex>
#include <libp7.h>


class delfileAsync : public QThread
{
    Q_OBJECT
public:
    delfileAsync(QObject *parent = NULL);

    void deleteFile(p7_handle_t *handle, QString memory, QString dir, QString filename);

signals:
    void deleted(FileInfoList lst, int err);

protected:
    void run();

private:
    QMutex _mutex;
    p7_handle_t* _handle;
    QString _mem;
    QString _dir;
    QString _filename;
};

#endif // DELFILEASYNC_H
