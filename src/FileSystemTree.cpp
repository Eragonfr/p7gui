#include "FileSystemTree.h"

#include <QContextMenuEvent>
#include <QDrag>
#include <QMenu>
#include <QMimeData>
#include <QSettings>

FileSystemTree::FileSystemTree(QWidget *parent): QTreeView (parent)
{
    _model = new QFileSystemModel;
    _model->setRootPath(QDir::currentPath());

    setModel(_model);

    for(int i = 2; i < _model->columnCount(); i++)
        hideColumn(i);

    QSettings settings("Intelligide", "p7gui");
    setCurrentIndex(_model->index(settings.value("lastTreePath", QDir::currentPath()).toString()));

    resizeColumnToContents(1);
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
    });
    transferMenu->addAction("SD Card", [=]() {
    });

    menu.exec(event->globalPos());
}
