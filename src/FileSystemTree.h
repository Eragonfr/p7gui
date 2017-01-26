#ifndef FILESYSTEMTREE_H
#define FILESYSTEMTREE_H

#include <QFileSystemModel>
#include <QTreeView>



class FileSystemTree : public QTreeView
{
    Q_OBJECT
public:
    FileSystemTree(QWidget *parent = Q_NULLPTR);
    ~FileSystemTree();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QFileSystemModel *_model;
};

#endif // FILESYSTEMTREE_H
