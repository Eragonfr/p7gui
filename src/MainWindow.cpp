#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <algorithm>

#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _connection(new Connection(this))
{
    ui->setupUi(this);

    this->ui->calculatorFileView->setConnection(_connection);

    this->ui->memoryComboBox->addItem("Storage memory (fls0)", Connection::StorageMemory);
    this->ui->memoryComboBox->addItem("SD Card (crd0)", Connection::SdCardMemory);

    connect(this->ui->memoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]() {
        this->ui->calculatorFileView->setMemory(this->ui->memoryComboBox->currentData().value<Connection::Memory>());
    });

    connect(_connection, SIGNAL(connected(bool)), this->ui->actionConnect, SLOT(setDisabled(bool)));
    connect(_connection, SIGNAL(connected(bool)), this->ui->actionDisconnect, SLOT(setEnabled(bool)));
    connect(_connection, SIGNAL(connected(bool)), this->ui->actionInfo, SLOT(setEnabled(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aboutGui()
{
    QMessageBox::about(this, tr("About P7 Gui"),
            tr("P7gui is a graphical interface designed for communicate with CASIO FX calculators thanks to P7 protocole.\n\n"
               "P7gui is licensed under the GNU GPL version 3. \n\n"
               "<a href='https://github.com/intelligide/p7gui'>Github project repo</a>"));
}

void MainWindow::aboutP7()
{
    QMessageBox::about(this, tr("About P7"),
            tr("This program uses libp7 version 2.0.0.\n\n"
               "Protocol 7.00 (or \"Protocol 7\") is the communication protocol used by CASIO FX calculators, between them and with a PC (serial and USB). It allows model identification, file interaction (with main memory and flash/SD storage devices), screenstreaming and OS updating.\n\n"
               "More on the <a href='https://p7.planet-casio.com/en.html'>P7 homepage</a>.\n\n"
               "libp7 licensed under the GNU GPL version 2 is appropriate for the development of applications provided you can comply with the terms and conditions of the GNU GPL version 2."));
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::startConnection()
{
    try {
        _connection->start();
        this->ui->calculatorFileView->refresh();
    } catch(const CommunicationException& e) {
        QString errMsg = QString("Connection error: ") + e.what();
        QMessageBox::critical(this, "Could not connect to the calculator", errMsg);
        log(errMsg);
    }
}

void MainWindow::stopConnection()
{
    _connection->stop();
}

void MainWindow::showConnectionInfos()
{
    QString txt;
    QMessageBox::information(this, "Informations",
                             "Username: " + _connection->username() + "\n\n"
                             "ROM: " + QString::number(_connection->rom()) + "\n"
                             "RAM: " + QString::number(_connection->ram()) + "\n\n"
                             "CPU Id: " + _connection->cpuid() + "\n"
                             "Environment: " + _connection->envid() + "\n"
                             "Product id: " + _connection->productid());
}

void MainWindow::log(QString txt)
{
    _logs.append(txt);
    statusBar()->showMessage(txt);
}
