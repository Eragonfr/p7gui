#ifndef CONNECTION_H
#define CONNECTION_H

#include "AsyncPool.h"

#include <libp7.h>

#include <QObject>
#include "FileInfo.h"

class Connection: public QObject
{
    Q_OBJECT
public:
    enum Memory {StorageMemory, SdCardMemory};

    Connection(QObject *parent = Q_NULLPTR);
    ~Connection();

    void listFiles(Memory mem);

    void sendFile(QString file, QString dir, Memory mem);
    void receiveFile(Memory mem, QString dir, QString file, QString des);

    void copyFile(Memory memory, QString dir, QString filename, QString newdir, QString newname);
    void deleteFile(Memory memory, QString dir, QString filename);

    bool isStarted();
    bool isWorking();

    QString username() const;
    int rom() const;
    int ram() const;
    QString cpuid() const;
    QString envid() const;
    QString productid() const;

    static QString memoryString(Memory mem);

public slots:
    void start();
    void stop();
    void optimize();

private slots:
    void handleInitialized(p7_handle_t *handle, int err);
    void handleOptimized(int err);
    void handleFilesListed(FileInfoList lst, int err);
    void handleDeleted(int err);
    void handleSent(int err);
    void handleReceived(int err);
    void handleCopied(int err);

signals:
    void transferProgress(int transferred, int total);
    void connected(bool);
    void disconnected(bool);
    void optimized();
    void listed(FileInfoList lst);
    void deleted();
    void errorOccured(int err, QString message);
    void started(QString);
    void finished();
    void received();
    void sent();
    void copied();

private:
    p7_handle_t *_handle;

    QString _username;
    int _rom;
    int _ram;
    QString _cpuid;
    QString _envid;
    QString _productid;

    AsyncPool _pool;
};

Q_DECLARE_METATYPE(Connection::Memory)


#endif // CONNECTION_H
