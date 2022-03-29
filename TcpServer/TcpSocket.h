#pragma once
#include <QTcpSocket>
#include <QByteArray>

/// <summary>
/// 套接字类，只是负责消息的收发
/// </summary>
class TcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	TcpSocket(const qintptr socketDescriptor);
	~TcpSocket();
public slots:
	//From this
	void read();						//响应readyRead
	//From TcpModel to Client
	void sendData(QByteArray buf);		//发送数据
private:
	void createConnections();			//创建连接
signals:
	//From Client to TcpModel
	void recvData(QByteArray buf);		//接收数据
};