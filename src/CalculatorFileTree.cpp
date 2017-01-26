#include "CalculatorFileTree.h"

#include <QContextMenuEvent>
#include <QFileIconProvider>
#include <QMenu>
#include <QMessageBox>

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


void CalculatorFileTree::refresh()
{
    if(!_connection->isStarted())
        return;
    try {
        FileInfoList finfo = _connection->listFiles(_mem);
        std::sort(finfo.begin(), finfo.end());

        _model->clear();
        QStringList lbls;
        lbls << "Name" << "Size";
        _model->setHorizontalHeaderLabels(lbls);

        QFileIconProvider iconProvider;
        foreach(FileInfo info, finfo) {
            if(info.isDir()) {
                QStandardItem* dirItem = new QStandardItem(info.dir());
                dirItem->setIcon(iconProvider.icon(QFileIconProvider::Folder));
                _model->appendRow(dirItem);
            } else {
                QStandardItem* filenameItem = new QStandardItem(info.filename());
                filenameItem->setIcon(iconProvider.icon(QFileIconProvider::File));
                _model->appendRow(filenameItem);
                QStandardItem* sizeItem = new QStandardItem(sizeHumanize(info.filesize()));
                _model->setItem(filenameItem->row(), 1, sizeItem);
            }
        }
    } catch(const CommunicationException& e) {
        QString errMsg = QString("Error: ") + e.what();
        QMessageBox::critical(this, "Error", errMsg);
    }
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
