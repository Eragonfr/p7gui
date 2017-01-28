#ifndef CALCULATORFILETREE_H
#define CALCULATORFILETREE_H

#include "Connection.h"

#include <QStandardItemModel>
#include <QTreeView>

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
    QStandardItemModel* _model;
    Connection::Memory _mem;
};

#endif // CALCULATORFILETREE_H
