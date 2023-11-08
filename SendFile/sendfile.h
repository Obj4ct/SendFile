#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>
class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);
    //连接服务器
    void ConnectServer(unsigned short port,QString ip);
    //发送文件
    void SendFileToServer(QString path);
signals:
    void ConnectOK();//连接成功
    void Over();//结束
    void CurPercent(int num);//进度条
private:
    QTcpSocket *m_tcp;
};

#endif // SENDFILE_H
