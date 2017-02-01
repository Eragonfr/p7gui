#include "FileSystemTree.h"

#include <QContextMenuEvent>
#include <QDrag>
#include <QMenu>
#include <QMimeData>
#include <QSettings>
#include <QDebug>
#include <QHeaderView>

FileSystemTree::FileSystemTree(QWidget *parent): QTreeView (parent)
{
    _model = new FileSystemModel(this);
    _model->setRootPath(QDir::currentPath());

    setModel(_model);

    for(int i = 2; i < _model->columnCount(); i++)
        hideColumn(i);

    QSettings settings("Intelligide", "p7gui");
    setCurrentIndex(_model->index(settings.value("lastTreePath", QDir::currentPath()).toString()));
    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}

FileSystemTree::~FileSystemTree()
{
    QSettings settings("Intelligide", "p7gui");

    settings.setValue("lastTreePath", _model->filePath(currentIndex()));
}

void FileSystemTree::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    QMenu menu;

    if(_model->isDir(index)) {
        menu.addAction("Remove", [=]() {
            _model->rmdir(index);
        });
    } else {
        menu.addAction("Remove", [=]() {
            _model->remove(index);
        });
    }
    menu.addAction("Transfer", [=]() {
    });
    QMenu* transferMenu = menu.addMenu("Transfer to...");
    transferMenu->addAction("Storage memory", [=]() {
        _connection->sendFile(_model->filePath(index), "", Connection::StorageMemory);
    });
    transferMenu->addAction("SD Card", [=]() {
        _connection->sendFile(_model->filePath(index), "", Connection::SdCardMemory);
    });


    if(!_connection->isStarted()) {
        qDebug()<< "ok";
        transferMenu->setEnabled(false);
    }

    menu.exec(event->globalPos());
}

Connection *FileSystemTree::connection() const
{
    return _connection;
}

void FileSystemTree::setConnection(Connection *connection)
{
    _connection = connection;
}

FileSystemModel::FileSystemModel(FileSystemTree *tree): QFileSystemModel (tree),
    _tree(tree)
{
    setReadOnly(false);
}

bool FileSystemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;
    QUrl url = data->urls().first();

    QStringList dir =  url.path().split("/");
    dir.removeFirst();
    dir.removeLast();
    _tree->connection()->receiveFile(url.scheme() == "fls0" ? Connection::StorageMemory : Connection::SdCardMemory,
                                     dir.join("/"), url.fileName(), filePath(parent).toStdString().c_str());

    return true;
}

bool FileSystemModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    QStringList authorizedScheme;
    authorizedScheme << "fls0" << "crd0";
    foreach(QUrl url, data->urls())
        if(!authorizedScheme.contains(url.scheme()))
            return false;
    return true;
}
