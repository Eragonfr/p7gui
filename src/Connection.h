#ifndef CONNECTION_H
#define CONNECTION_H

#include <libp7.h>

#include <QList>
#include <QObject>

class FileInfo {

public:
    FileInfo(const QString &dir, const QString &file, unsigned int filesize);

    QString dir() const;
    QString filename() const;
    unsigned int filesize() const;

    bool isDir() const;

    bool operator<(const FileInfo &other) const;

private:
    QString _dir;
    QString _filename;
    unsigned int _filesize;
};

typedef QList<FileInfo> FileInfoList;

class CommunicationException : public std::exception
{
public:
    CommunicationException(int err);

    const char * what() const noexcept;

private:
    int _err;
};

class Connection: public QObject
{
    Q_OBJECT
public:
    enum Memory {StorageMemory, SdCardMemory};

    Connection(QObject *parent = Q_NULLPTR);
    ~Connection();

    FileInfoList listFiles(Memory mem);

    void sendFile(QString file, QString dir, Memory mem);
    void receiveFile(QString file, QString dir, Memory mem);

    void copyFile();
    void deleteFile();

    bool isStarted();

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

signals:
    void transferProgress(int transferred, int total);
    void connected(bool);
    void disconnected(bool);

private:
    static FileInfoList _fileinfoListBuffer;
    static void lsfilesCallback(const char *dir, const char *filename, p7uint_t filesize);

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
};

Q_DECLARE_METATYPE(Connection::Memory)


#endif // CONNECTION_H
