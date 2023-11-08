#include "sendfile.h"
#include<QFileInfo>
#include <QFile>
#include <QHostAddress>

SendFile::SendFile(QObject *parent)
    : QObject{parent}
{

}

void SendFile::ConnectServer(unsigned short port, QString ip)
{
    m_tcp=new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip),port);
    connect(m_tcp,&QTcpSocket::connected,this,&SendFile::ConnectOK);
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        m_tcp->close();
        m_tcp->disconnected();
        emit Over();
    });
}



void SendFile::SendFileToServer(QString path)
{
   QFile file(path);
   QFileInfo info(path);
   int fileSize=info.size();
   file.open(QFile::ReadOnly);
   while(!file.atEnd())
   {
       //判断是不是第一次发送文件
       static int num=0;
       if(num==0)
       {
           m_tcp->write((char*)&fileSize,4);
       }
       QByteArray line=file.readLine();
       num+=line.size();
       int percent=(num*100/fileSize);
       emit CurPercent(percent);
       m_tcp->write(line);
   }

}
