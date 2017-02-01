#include "CalculatorFileTree.h"

#include <QContextMenuEvent>
#include <QFileIconProvider>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QMimeData>
#include <QInputDialog>
#include <QHeaderView>

CalculatorFileTree::CalculatorFileTree(QWidget *parent): QTreeView (parent)
{
    _model = new CalculatorFileModel(this);
    QStringList lbls;
    lbls << "Name" << "Size";
    _model->setHorizontalHeaderLabels(lbls);

    setModel(_model);

    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
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
        DirItem* rootItem = new DirItem("root", FileInfo(), iconProvider.icon(QFileIconProvider::Folder));
        _model->appendRow(rootItem);

        foreach(FileInfo info, lst) {
            rootItem->createItem(info.dir().split("/"), info);
        }
        header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

        setRootIndex(_model->indexFromItem(rootItem));
    });

    connect(_connection, &Connection::deleted, this, &CalculatorFileTree::refresh);
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
    FileTreeItem* item = dynamic_cast<FileTreeItem*>(_model->itemFromIndex(index));
    QMenu menu;

    if(item) {
        FileInfo finfo = item->info();

        menu.addAction("Remove", [=]() {
            _connection->deleteFile(_mem, finfo.dir(), finfo.filename());
        });
        menu.addAction("Rename", [=]() {
            QString newname = QInputDialog::getText(this, "Rename", "Rename file '"+finfo.filename()+"' to:");
            _connection->copyFile(memory(), finfo.dir(), finfo.filename(), finfo.dir(), newname);
            _connection->deleteFile(_mem, finfo.dir(), finfo.filename());
        });
        menu.addAction("Transfer", [=]() {
        });
    }

    menu.addSeparator();

    menu.addAction("Refresh",this, &CalculatorFileTree::refresh);

    menu.exec(event->globalPos());
}

FileTreeItem::FileTreeItem(const QString &text, const FileInfo &finfo, const QIcon &icon): QStandardItem (icon, text),
    _finfo(finfo)
{

}

DirItem *FileTreeItem::toDirItem()
{
    return dynamic_cast<DirItem*>(this);
}

FileItem *FileTreeItem::toFileItem()
{
    return dynamic_cast<FileItem*>(this);
}

bool FileTreeItem::isFile() const
{
    return false;
}

bool FileTreeItem::isDir() const
{
    return false;
}

const FileInfo &FileTreeItem::info() const
{
    return _finfo;
}

DirItem::DirItem(const QString &dirname, const FileInfo &finfo, const QIcon &icon): FileTreeItem(dirname,finfo, icon)
{

}

QString sizeHumanize(float size)
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

FileTreeItem *DirItem::createItem(QStringList dirChain, FileInfo fileinfo)
{
    QFileIconProvider iconProvider;

    if(dirChain.isEmpty() || dirChain.value(0).isEmpty()) {
        QString filename = fileinfo.filename();
        if(filename.isEmpty())
            return this;
        if(_items.contains(filename))
            return _items.value(filename);
        FileItem* item = new FileItem(fileinfo, iconProvider.icon(QFileIconProvider::File));
        _items.insert(filename, item);
        QStandardItem* sizeItem = new QStandardItem(sizeHumanize(fileinfo.filesize()));
        appendRow(item);
        setChild(item->row(), 1, sizeItem);
        return item;
    } else {
        QString dirName = dirChain.takeFirst();
        DirItem* item;
        if(_items.contains(dirName)) {
            item = _items.value(dirName)->toDirItem();
        } else {
            item = new DirItem(dirName, fileinfo, iconProvider.icon(QFileIconProvider::Folder));
            _items.insert(dirName, item);
            appendRow(item);
        }
        return item->createItem(dirChain, fileinfo);
    }
}

bool DirItem::isDir() const
{
    return true;
}

FileItem::FileItem(const FileInfo &finfo, const QIcon &icon): FileTreeItem ( finfo.filename(), finfo, icon)
{

}

bool FileItem::isFile() const
{
    return true;
}

CalculatorFileModel::CalculatorFileModel(CalculatorFileTree *parent): QStandardItemModel(parent),
    _tree(parent)
{

}

bool CalculatorFileModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;
    QUrl url = data->urls().first();

    if(url.scheme() == "file") {
        FileTreeItem* treeItem = dynamic_cast<FileTreeItem*>(itemFromIndex(parent));

        if(!treeItem || !treeItem->isDir())
            treeItem = dynamic_cast<FileTreeItem*>(itemFromIndex(parent.parent()));

        _tree->connection()->sendFile(url.path(), treeItem->info().dir(),_tree->memory());
    } else {
        FileTreeItem* treeItem = dynamic_cast<FileTreeItem*>(itemFromIndex(parent));

        if(!treeItem || !treeItem->isDir())
            treeItem = dynamic_cast<FileTreeItem*>(itemFromIndex(parent.parent()));
        QStringList dir =  url.path().split("/");
        dir.removeFirst();
        dir.removeLast();
        _tree->connection()->copyFile(_tree->memory(), dir.join("/"), url.fileName(), treeItem->info().dir(), url.fileName());
        _tree->connection()->deleteFile(_tree->memory(), dir.join("/"), url.fileName());
    }

    return true;
}

bool CalculatorFileModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    QStringList authorizedScheme;
    authorizedScheme << "file" << "fls0" << "crd0";
    if(!_tree->connection()->isStarted())
        return false;
    foreach(QUrl url, data->urls())
        if(!authorizedScheme.contains(url.scheme()))
            return false;
    return true;
}

QStringList CalculatorFileModel::mimeTypes() const
{
    QStringList types;
    types << "text/uri-list";
    return types;
}

QMimeData *CalculatorFileModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();

    QList<QUrl> urlList;
    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            FileTreeItem* treeItem = dynamic_cast<FileTreeItem*>(itemFromIndex(index));
            if(treeItem) {
                QString filename = treeItem->info().filename();
                QString dir = treeItem->info().dir();
                if(!(dir.isEmpty()))
                    filename.prepend(dir+"/");
                urlList.append(QUrl(Connection::memoryString(_tree->memory())+":///"+treeItem->info().filename()));
            }
        }
    }

    mimeData->setUrls(urlList);
    return mimeData;
}
