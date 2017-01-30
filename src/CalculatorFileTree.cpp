#include "CalculatorFileTree.h"

#include <QContextMenuEvent>
#include <QFileIconProvider>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

CalculatorFileTree::CalculatorFileTree(QWidget *parent)
{
    _model = new QStandardItemModel(this);
    QStringList lbls;
    lbls << "Name" << "Size";
    _model->setHorizontalHeaderLabels(lbls);

    setModel(_model);
}

Connection *CalculatorFileTree::connection() const
{
    return _connection;
}

void CalculatorFileTree::setConnection(Connection *connection)
{
    _connection = connection;

    connect(_connection, &Connection::listed, [=](FileInfoList lst) {
        _model->clear();
        QStringList lbls;
        lbls << "Name" << "Size";
        _model->setHorizontalHeaderLabels(lbls);

        QFileIconProvider iconProvider;
        DirItem* rootItem = new DirItem("root", iconProvider.icon(QFileIconProvider::Folder));
        _model->appendRow(rootItem);

        foreach(FileInfo info, lst) {
            rootItem->createItem(info.dir().split("/"), info.filename());
        }

        setRootIndex(_model->indexFromItem(rootItem));
    });
}

QString CalculatorFileTree::sizeHumanize(float size)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(size >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        size /= 1024.0;
    }
    return QString().setNum(size,'f',2)+" "+unit;
}


void CalculatorFileTree::refresh()
{
    if(!_connection->isStarted())
        return;
    _connection->listFiles(_mem);
}

Connection::Memory CalculatorFileTree::memory() const
{
    return _mem;
}

void CalculatorFileTree::setMemory(const Connection::Memory &mem)
{
    _mem = mem;
    refresh();
}

void CalculatorFileTree::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    QMenu menu;

    menu.addAction("Remove", [=]() {
    });
    menu.addAction("Transfer", [=]() {
    });

    menu.addAction("Refresh",this, &CalculatorFileTree::refresh);

    menu.exec(event->globalPos());
}

FileTreeItem::FileTreeItem(const QIcon &icon, const QString &text): QStandardItem (icon, text)
{

}

DirItem *FileTreeItem::toDirItem()
{
    return dynamic_cast<DirItem*>(this);
}

bool FileTreeItem::isFile() const
{
    return false;
}

bool FileTreeItem::isDir() const
{
    return false;
}

DirItem::DirItem(const QString &text, const QIcon &icon): FileTreeItem (icon, text)
{

}

FileTreeItem *DirItem::createItem(QStringList dirChain, QString filename)
{
    QFileIconProvider iconProvider;

    if(dirChain.isEmpty() || dirChain.value(0).isEmpty()) {
        if(filename.isEmpty())
            return this;
        if(_items.contains(filename))
            return _items.value(filename);
        FileItem* item = new FileItem(filename, iconProvider.icon(QFileIconProvider::File));
        _items.insert(filename, item);
        appendRow(item);
        return item;
    } else {
        QString dirName = dirChain.takeFirst();
        DirItem* item;
        if(_items.contains(dirName)) {
            item = _items.value(dirName)->toDirItem();
        } else {
            item = new DirItem(dirName, iconProvider.icon(QFileIconProvider::Folder));
            _items.insert(dirName, item);
            appendRow(item);
        }
        return item->createItem(dirChain, filename);
    }
}

bool DirItem::isDir() const
{
    return true;
}

FileItem::FileItem(const QString &text, const QIcon &icon): FileTreeItem (icon, text)
{

}

bool FileItem::isFile() const
{
    return true;
}
