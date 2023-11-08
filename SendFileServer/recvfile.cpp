#include "recvfile.h"
#include<QFile>
RecvFile::RecvFile(QTcpSocket* tcp,QObject *parent)
    : QThread{parent}
{
    this->m_socket=tcp;
}

void RecvFile::run()
{
    QFile *file=new QFile("recv.txt");
    file->open(QFile::WriteOnly);
    //接收数据
    connect(m_socket,&QTcpSocket::readyRead,this,[=](){
        static int count=0;
        static int total=0;
        if(count==0)
        {
            //得到文件总大小
            m_socket->read((char*)&total,4);
        }
        //读出剩余数据
        QByteArray all=m_socket->readAll();
        count+=all.size();
        file->write(all);
        //判断是否接收完毕
        if(count==total)
        {
            m_socket->close();
            m_socket->deleteLater();
            file->close();
            file->deleteLater();
            emit Over();
        }
    });
    //进入循环
    exec();
}
