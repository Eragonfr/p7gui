#ifndef CONNECTION_H
#define CONNECTION_H

#include <libp7.h>

#include <QObject>

#include <async/initAsync.h>
#include <async/lsFilesAsync.h>
#include <async/optimizeAsync.h>
#include <async/sendFileAsync.h>
#include <async/delFileAsync.h>
#include <async/reqFileAsync.h>
#include <async/copyFileAsync.h>

class Connection: public QObject
{
    Q_OBJECT
public:
    enum Memory {StorageMemory, SdCardMemory};

    Connection(QObject *parent = Q_NULLPTR);
    ~Connection();

    void listFiles(Memory mem);

    void sendFile(QString file, QString dir, Memory mem);
    void receiveFile(QString file, QString dir, Memory mem);

    void copyFile();
    void deleteFile();

    bool isStarted();
    bool isWorking();

    QString username() const;
    int rom() const;
    int ram() const;
    QString cpuid() const;
    QString envid() const;
    QString productid() const;

public slots:
    void start();
    void stop();
    void optimize();

private slots:
    void handleInitialized(p7_handle_t *handle, int err);
    void handleOptimized(int err);
    void handleFilesListed(FileInfoList lst, int err);

signals:
    void transferProgress(int transferred, int total);
    void connected(bool);
    void disconnected(bool);
    void optimized();
    void listed(FileInfoList lst);
    void errorOccured(int err, QString message);

private:

    typedef void (Connection::*FNMETHOD) ( p7ushort_t, p7ushort_t );
    void progress(p7ushort_t t, p7ushort_t total);
    QString memoryString(Memory mem);

    p7_handle_t *_handle;

    QString _username;
    int _rom;
    int _ram;
    QString _cpuid;
    QString _envid;
    QString _productid;

    bool _working;

    // async
    initAsync _initAsync;
    optimizeAsync _optimizeAsync;
    lsFilesAsync _lsFilesAsync;
    sendFileAsync _sendFileAsync;
    copyFileAsync _copyFileAsync;
    reqFileAsync _reqFileAsync;
    delFileAsync _delFileAsync;
};

Q_DECLARE_METATYPE(Connection::Memory)


#endif // CONNECTION_H
