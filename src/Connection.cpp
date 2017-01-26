#include <exception>

#include <libp7/packetio.h>

#include "Connection.h"

Connection::Connection(QObject *parent): QObject (parent),
    _handle(NULL)
{

}

Connection::~Connection()
{
    if(_handle)
        stop();
}

void Connection::start()
{
    int err = p7_init(&_handle, 1, 1);
    if(err) {
        throw CommunicationException(err);
    }
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

void Connection::stop()
{
    p7_exit(_handle, 1);
    _handle = NULL;
    emit connected(false);
    emit disconnected(true);
}

FileInfoList Connection::listFiles(Memory mem)
{
    _fileinfoListBuffer.clear();    
    int err = p7_lsfiles(_handle, memoryString(mem).toStdString().c_str(), &Connection::lsfilesCallback);
    if(err) {
        throw CommunicationException(err);
    }
    return _fileinfoListBuffer;
}

void Connection::optimize()
{
    int err = p7_optimize(_handle, "fls0");
    if(err) {
        throw CommunicationException(err);
    }
}

void Connection::copyFile()
{
    int err = p7_copyfile(_handle, "hidden", "file.txt", NULL, "VISIBLE.txt", "fls0");
    if(err) {
        throw CommunicationException(err);
    }
}

void Connection::deleteFile()
{
    int err = p7_delfile(_handle, NULL, "responsabilities.txt", "fls0");
    if(err) {
        throw CommunicationException(err);
    }
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

FileInfoList Connection::_fileinfoListBuffer = FileInfoList();

void Connection::lsfilesCallback(const char *dir, const char *filename, p7uint_t filesize)
{
    _fileinfoListBuffer.append(FileInfo(dir, filename, filesize));
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

CommunicationException::CommunicationException(int err)
{
    _err = err;
}

const char *CommunicationException::what() const noexcept
{
    return p7_strerror(_err);
}

FileInfo::FileInfo(const QString &dir, const QString &file, unsigned int filesize):
    _dir(dir),
    _filename(file),
    _filesize(filesize)
{

}

QString FileInfo::dir() const
{
    return _dir;
}

QString FileInfo::filename() const
{
    return _filename;
}

unsigned int FileInfo::filesize() const
{
    return _filesize;
}

bool FileInfo::isDir() const
{
    return _filename.isEmpty();
}

bool FileInfo::operator<(const FileInfo &other) const
{
    return _dir + "/" + _filename < other._dir + "/" + other._filename;
}


