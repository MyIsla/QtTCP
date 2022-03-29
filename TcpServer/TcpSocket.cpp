#pragma execution_character_set("utf-8")
#include "TcpSocket.h"

TcpSocket::TcpSocket(qintptr socketDescriptor)
{
	this->setSocketDescriptor(socketDescriptor);
	createConnections();
}

TcpSocket::~TcpSocket()
{
}

/// <summary>
/// 响应readyRead
/// </summary>
void TcpSocket::read()
{
	QByteArray buf = this->readAll();
	emit recvData(buf);
}

/// <summary>
/// 发送数据
/// </summary>
/// <param name="buf">数据</param>
void TcpSocket::sendData(QByteArray buf)
{
	//qDebug() << "TcpSocket Thread:" << this->thread();
	this->write(buf);
	this->flush();
}

/// <summary>
/// 创建连接
/// </summary>
void TcpSocket::createConnections()
{
	connect(this, &QTcpSocket::readyRead, this, &TcpSocket::read);
}
