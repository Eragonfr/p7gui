#ifndef TASKNOTIFIERWIDGET_H
#define TASKNOTIFIERWIDGET_H

#include <QWidget>

namespace Ui {
class TaskNotifierWidget;
}

class TaskNotifierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskNotifierWidget(QWidget *parent = 0);
    ~TaskNotifierWidget();

public slots:
    void setMaximum(int maximum);
    void setValue(int value);
    void setProgress(int value, int maximum);
    void setMessage(QString message);

private:
    Ui::TaskNotifierWidget *ui;
};

#endif // TASKNOTIFIERWIDGET_H
