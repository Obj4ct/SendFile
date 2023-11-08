#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <qtcpsocket.h>
#include "recvfile.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server=new QTcpServer(this);
    connect(m_server,&QTcpServer::newConnection,this,[=](){
        QTcpSocket * tcp=m_server->nextPendingConnection();//得到套接字对象
        //创建子线程
        RecvFile * subThread=new RecvFile(tcp);
        subThread->start();
        connect(subThread,&RecvFile::Over,this,[=](){
            subThread->exit();
            subThread->wait();
            subThread->deleteLater();
            QMessageBox::information(this,"文件接收","文件接收完毕！");

        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_listen_clicked()
{
    unsigned short port = ui->lineEdit_port->text().toUShort();
    m_server->listen(QHostAddress::Any,port);

}

