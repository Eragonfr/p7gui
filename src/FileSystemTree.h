#ifndef FILESYSTEMTREE_H
#define FILESYSTEMTREE_H

#include "Connection.h"

#include <QFileSystemModel>
#include <QTreeView>

class FileSystemTree;

class FileSystemModel: public QFileSystemModel
{
public:
    FileSystemModel(FileSystemTree* tree);

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;

private:
    FileSystemTree* _tree;
};

class FileSystemTree : public QTreeView
{
    Q_OBJECT
public:
    FileSystemTree(QWidget *parent = Q_NULLPTR);
    ~FileSystemTree();

    Connection *connection() const;
    void setConnection(Connection *connection);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    FileSystemModel *_model;
    Connection* _connection;

};

#endif // FILESYSTEMTREE_H
