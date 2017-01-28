#include <exception>
#include <QtDebug>

#include <libp7/packetio.h>

#include "Connection.h"

Connection::Connection(QObject *parent): QObject (parent),
    _handle(NULL)
{
    connect(&_initThread, SIGNAL(initialized(p7_handle_t *, int)),this, SLOT(handleInitialized(p7_handle_t *, int)));
    connect(&_optimizeThread, SIGNAL(optimized(p7_handle_t *, int)),this, SLOT(handleOptimized(p7_handle_t *, int)));
    connect(&_lsFileAsync, &lsFilesAsync::listed, this, &Connection::handleListed);
}

Connection::~Connection()
{
    if(_handle)
        stop();
}

void Connection::start()
{
    _initThread.initHandle(_handle);
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
    _lsFileAsync.listFiles(_handle, memoryString(mem));
}

void Connection::optimize()
{
    _optimizeThread.optimize(_handle);
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

void Connection::handleOptimized(p7_handle_t *handle, int err)
{
    if(err) {
        emit errorOccured(err,  QString("Optimization error: ") + p7_strerror(err));
        return;
    }
    emit optimized();
}

void Connection::handleListed(FileInfoList lst, int err)
{
    if(err) {
        emit errorOccured(err,  QString("List files error: ") + p7_strerror(err));
        return;
    }
    emit listed(lst);
}

void Connection::copyFile()
{
    int err = p7_copyfile(_handle, "hidden", "file.txt", NULL, "VISIBLE.txt", "fls0");
    // TODO
}

void Connection::deleteFile()
{
    int err = p7_delfile(_handle, NULL, "responsabilities.txt", "fls0");
    // TODO
}

bool Connection::isStarted()
{
    return _handle;
}

void Connection::sendFile(QString file, QString dir, Memory mem)
{
    /*
    FILE *fp = fopen("myaddin.g1a", "r");
    int err = p7_sendfile(_handle, fp, dir.toStdString().c_str(),
                          file.toStdString().c_str(), memoryString(mem).toStdString().c_str(), 1, NULL, &Connection::progress);
    if(err) {
        throw CommunicationException(err);
    }*/
}

void Connection::receiveFile(QString file, QString dir, Memory mem)
{
    /*
    FILE *fp = fopen("myaddin.g1a", "w");
    int err = p7_reqfile(_handle, fp, dir.toStdString().c_str(), file.toStdString().c_str(),
                         memoryString(mem).toStdString().c_str(), &Connection::progress));
    if(err) {
        throw CommunicationException(err);
    }*/
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
