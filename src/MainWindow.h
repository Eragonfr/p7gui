#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Connection.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void aboutGui();
    void aboutP7();
    void aboutQt();
    void startConnection();
    void stopConnection();
    void showConnectionInfos();

private:
    void log(QString txt);

    Connection* _connection;
    Ui::MainWindow *ui;

    QProgressBar* _progressBar;

    QStringList _logs;
};

#endif // MAINWINDOW_H
