#pragma execution_character_set("utf-8")
#include "TcpSocket.h"

TcpSocket::TcpSocket(QString addr, QString port)
{
	this->connectToHost(QHostAddress(addr), quint16(port.toInt()));
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
