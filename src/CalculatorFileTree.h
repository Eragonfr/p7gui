#ifndef CALCULATORFILETREE_H
#define CALCULATORFILETREE_H

#include "Connection.h"

#include <QStandardItemModel>
#include <QTreeView>

class FileItem;
class DirItem;
class CalculatorFileTree;

class FileTreeItem: public QStandardItem
{
public:
    FileTreeItem(const QString& text, const FileInfo &finfo, const QIcon &icon);

    DirItem* toDirItem();
    FileItem* toFileItem();

    virtual bool isFile() const;
    virtual bool isDir() const;
    const FileInfo& info() const;

private:
    FileInfo _finfo;
};

class DirItem: public FileTreeItem
{
public:
    DirItem(const QString& dirname, const FileInfo &finfo, const QIcon &icon);

    FileTreeItem* createItem(QStringList dirChain, FileInfo fileinfo);
    bool isDir() const;

private:
    QMap<QString, FileTreeItem*> _items;
};

class FileItem: public FileTreeItem
{
public:
    FileItem(const FileInfo &finfo, const QIcon &icon);

    bool isFile() const;
};

class CalculatorFileModel: public QStandardItemModel
{
public:
    CalculatorFileModel(CalculatorFileTree* tree);

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    QStringList mimeTypes() const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;

private:
    CalculatorFileTree* _tree;
};

class CalculatorFileTree : public QTreeView
{
    Q_OBJECT
public:
    CalculatorFileTree(QWidget *parent = Q_NULLPTR);

    Connection *connection() const;
    void setConnection(Connection *connection);

    Connection::Memory memory() const;

public slots:
    void refresh();
    void setMemory(const Connection::Memory &mem);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    static QString sizeHumanize(float);

    Connection* _connection;
    CalculatorFileModel* _model;
    Connection::Memory _mem;
};

#endif // CALCULATORFILETREE_H
