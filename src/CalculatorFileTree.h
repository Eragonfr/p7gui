#ifndef CALCULATORFILETREE_H
#define CALCULATORFILETREE_H

#include "Connection.h"

#include <QStandardItemModel>
#include <QTreeView>

class FileItem;
class DirItem;

class FileTreeItem: public QStandardItem
{
public:
    FileTreeItem(const QIcon &icon, const QString &text);

    DirItem* toDirItem();

    virtual bool isFile() const;
    virtual bool isDir() const;
};

class DirItem: public FileTreeItem
{
public:
    DirItem(const QString &text, const QIcon &icon);

    FileTreeItem* createItem(QStringList dirChain, QString filename);
    bool isDir() const;

private:
    QMap<QString, FileTreeItem*> _items;
};

class FileItem: public FileTreeItem
{
public:
    FileItem(const QString &text, const QIcon &icon);

    bool isFile() const;

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
    void createItem(QString filename, QStringList dirChain);
    static QString sizeHumanize(float);

    Connection* _connection;
    QStandardItemModel* _model;
    Connection::Memory _mem;
};

#endif // CALCULATORFILETREE_H
