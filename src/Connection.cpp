#include <exception>

#include <libp7/packetio.h>

#include "Connection.h"

Connection::Connection(QObject *parent): QObject (parent),
    _handle(NULL)
{
    connect(&_initAsync, SIGNAL(initialized(p7_handle_t *, int)),
            this, SLOT(handleInitialized(p7_handle_t *, int)));
    connect(&_optimizeAsync, SIGNAL(optimized(int)), this, SLOT(handleOptimized(int)));
    connect(&_lsFilesAsync, SIGNAL(listed(FileInfoList, int)), this, SLOT(handleFilesListed(FileInfoList, int)));
}

Connection::~Connection()
{
    if(_handle)
        stop();
}

void Connection::start()
{
    _initAsync.initHandle(_handle);
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
    _lsFilesAsync.listFiles(_handle, memoryString(mem));
    _working = true;
}

void Connection::optimize()
{
    _optimizeAsync.optimize(_handle);
    _working = true;
}

void Connection::handleInitialized(p7_handle_t *handle, int err)
{
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
    if(err) {
        emit errorOccured(err,  QString("Optimization error: ") + p7_strerror(err));
        return;
    }
    emit optimized();
}

void Connection::handleFilesListed(FileInfoList lst, int err)
{
    if(err) {
        emit errorOccured(err,  QString("List files error: ") + p7_strerror(err));
        return;
    }
    emit listed(lst);
}

void Connection::copyFile()
{
    // TODO
    _working = true;
}

void Connection::deleteFile()
{
    // TODO
    _working = true;
}


void Connection::sendFile(QString file, QString dir, Memory mem)
{
    _sendFileAsync.sendFile(_handle, dir, file, memoryString(mem));
}

void Connection::receiveFile(QString file, QString dir, Memory mem)
{
    _reqFileAsync.reqFile(_handle, dir, file, memoryString(mem));
}

bool Connection::isStarted()
{
    return _handle;
}

bool Connection::isWorking()
{
    return _working;
}

void Connection::progress(p7ushort_t t, p7ushort_t total)
{
    emit transferProgress(t, total);
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
