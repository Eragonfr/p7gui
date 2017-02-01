#include "TaskNotifierWidget.h"
#include "ui_TaskNotifierWidget.h"

TaskNotifierWidget::TaskNotifierWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskNotifierWidget)
{
    ui->setupUi(this);
}

TaskNotifierWidget::~TaskNotifierWidget()
{
    delete ui;
}

void TaskNotifierWidget::setMaximum(int maximum)
{
    ui->progressBar->setMaximum(maximum);
}

void TaskNotifierWidget::setValue(int value)
{
    ui->progressBar->setValue(value);
}

void TaskNotifierWidget::setProgress(int value, int maximum)
{
    ui->progressBar->setMaximum(maximum);
    ui->progressBar->setValue(value);
}

void TaskNotifierWidget::setMessage(QString message)
{
    ui->label->setText(message);
}
