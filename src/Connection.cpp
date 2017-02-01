#include <exception>

#include <libp7/packetio.h>

#include "Connection.h"

#include <async/initAsync.h>
#include <async/lsFilesAsync.h>
#include <async/optimizeAsync.h>
#include <async/sendFileAsync.h>
#include <async/delFileAsync.h>
#include <async/reqFileAsync.h>
#include <async/copyFileAsync.h>

Connection::Connection(QObject *parent): QObject (parent),
    _handle(NULL),
    _pool(this)
{
    connect(&_pool, SIGNAL(started(QString)), SIGNAL(started(QString)));
}

Connection::~Connection()
{
    if(_handle)
        stop();
}

void Connection::start()
{
    initAsync* initThread = new initAsync(this);
    connect(initThread, SIGNAL(initialized(p7_handle_t *, int)),
            SLOT(handleInitialized(p7_handle_t *, int)));
    initThread->initHandle(_handle);
    _pool.addThread(initThread, "Connecting");
}

void Connection::stop()
{
    p7_exit(_handle, 1);
    _handle = NULL;
    emit connected(false);
    emit disconnected(true);
}

void Connection::listFiles(Memory mem)
{
    lsFilesAsync* lsFilesThread = new lsFilesAsync(this);
    connect(lsFilesThread, SIGNAL(listed(FileInfoList, int)), SLOT(handleFilesListed(FileInfoList, int)));

    lsFilesThread->listFiles(_handle, memoryString(mem));
    _pool.addThread(lsFilesThread, "Refreshing");

}

void Connection::optimize()
{
    optimizeAsync* optimizeThread = new optimizeAsync(this);
    connect(optimizeThread, SIGNAL(optimized(int)), SLOT(handleOptimized(int)));
    optimizeThread->optimize(_handle);
    _pool.addThread(optimizeThread, "Optimizing");
}

void Connection::handleInitialized(p7_handle_t *handle, int err)
{
    emit finished();
    if(err) {
        emit errorOccured(err,  QString("Connection error: ") + p7_strerror(err));
        return;
    }
    _handle = handle;
    const p7_server_t *info = p7_get_info(_handle);
    _cpuid = info->cpuid;
    _envid = info->hwid;
    _productid = info->product_id;
    _rom = info->flash_rom_capacity / 1024;
    _ram = info->ram_capacity / 1024;

    if (info->username[0])
        _username = info->username;

    emit connected(true);
    emit disconnected(false);
}

void Connection::handleOptimized(int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("Optimization error: ") + p7_strerror(err));
        return;
    }
    emit optimized();
}

void Connection::handleFilesListed(FileInfoList lst, int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("List files error: ") + p7_strerror(err));
        return;
    }
    emit listed(lst);
}

void Connection::handleDeleted(int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("Deletion error: ") + p7_strerror(err));
        return;
    }
    emit deleted();
}

void Connection::handleSent(int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("Transfer error: ") + p7_strerror(err));
        return;
    }
    emit sent();
}

void Connection::handleReceived(int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("Transfer error: ") + p7_strerror(err));
        return;
    }
    emit received();
}

void Connection::handleCopied(int err)
{
    emit finished();

    if(err) {
        emit errorOccured(err,  QString("Copy error: ") + p7_strerror(err));
        return;
    }
    emit copied();
}

void Connection::deleteFile(Memory memory, QString dir, QString filename)
{
    delFileAsync* delFileThread = new delFileAsync(this);
    connect(delFileThread, SIGNAL(deleted(int)), SLOT(handleDeleted(int)));

    delFileThread->deleteFile(_handle, memoryString(memory), dir, filename);
    _pool.addThread(delFileThread, "Deleting " + filename);
}

void Connection::sendFile(QString file, QString dir, Memory mem)
{
    QString memstring  = memoryString(mem);

    sendFileAsync* sendFileThread = new sendFileAsync(this);
    connect(sendFileThread, SIGNAL(sent(int)), SLOT(handleSent(int)));
    connect(sendFileThread, SIGNAL(uploadProgress(int,int)), SIGNAL(transferProgress(int, int)));

    sendFileThread->sendFile(_handle, file, dir, memstring);
    _pool.addThread(sendFileThread, "Sending " + file + " to "+ memstring +"://"+dir);
}

void Connection::receiveFile(Memory mem, QString dir, QString file, QString des)
{
    reqFileAsync* reqFileThread = new reqFileAsync(this);
    connect(reqFileThread, SIGNAL(received(int)), SLOT(handleReceived(int)));
    connect(reqFileThread, SIGNAL(downloadProgress(int,int)), SIGNAL(transferProgress(int, int)));

    reqFileThread->reqFile(_handle, memoryString(mem), dir, file, des);
    _pool.addThread(reqFileThread, "Receiving " + file);
}

void Connection::copyFile(Connection::Memory memory, QString dir, QString filename, QString newdir, QString newname)
{
    copyFileAsync* copyFileThread = new copyFileAsync(this);
    connect(copyFileThread, SIGNAL(copied(int)), SLOT(handleCopied(int)));

    copyFileThread->copyFile(_handle,memoryString(memory), dir, filename, newdir, newname);
    _pool.addThread(copyFileThread, "Copying " + memoryString(memory) + "://"+dir + "/" + filename + " to "
                    + memoryString(memory) + "://"+ newdir + "/" + newname);
}

bool Connection::isStarted()
{
    return _handle;
}

bool Connection::isWorking()
{
    return _pool.isRunning();
}

QString Connection::memoryString(Connection::Memory mem)
{
    switch (mem) {
    case SdCardMemory:
        return "crd0";
    case StorageMemory:
    default:
        return "fls0";
    }
}

QString Connection::productid() const
{
    return _productid;
}

QString Connection::envid() const
{
    return _envid;
}

QString Connection::cpuid() const
{
    return _cpuid;
}

int Connection::ram() const
{
    return _ram;
}

int Connection::rom() const
{
    return _rom;
}

QString Connection::username() const
{
    return _username;
}
