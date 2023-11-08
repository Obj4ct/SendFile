#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendfile.h"
#include <QMessageBox>
#include <QThread>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_port->setText("8989");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    //创建线程对象
    QThread * thread=new QThread;
    //创建任务对象
    class SendFile *worker=new (class SendFile);
    worker->moveToThread(thread);
    connect(this,&MainWindow::SendFile,worker,&SendFile::SendFileToServer);
    connect(this,&MainWindow::StartConnect,worker,&SendFile::ConnectServer);
    //处理发出的信号
    connect(worker,&SendFile::ConnectOK,this,[=](){
        QMessageBox::information(this,"连接服务器","连接服务器成功！");
    });
    connect(worker,&SendFile::Over,this,[=](){
        //释放资源
        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
    });
    connect(worker,&SendFile::CurPercent,ui->progressBar,&QProgressBar::setValue);
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_connect_clicked()
{
    QString ip=ui->lineEdit_ip->text();
    unsigned short port =ui->lineEdit_port->text().toUShort();
    emit StartConnect(port,ip);
}


void MainWindow::on_btn_selectFile_clicked()
{
    QString path=QFileDialog::getOpenFileName();
    if(path.isEmpty())
    {
        QMessageBox::warning(this,"打开文件","选择的文件不能为空！");
        return;
    }
    ui->lineEdit_filePath->setText(path);
}


void MainWindow::on_btn_sendFile_clicked()
{
    emit SendFile(ui->lineEdit_filePath->text());
}

