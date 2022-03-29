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
/// ��ӦreadyRead
/// </summary>
void TcpSocket::read()
{
	QByteArray buf = this->readAll();
	emit recvData(buf);
}

/// <summary>
/// ��������
/// </summary>
/// <param name="buf">����</param>
void TcpSocket::sendData(QByteArray buf)
{
	this->write(buf);
	this->flush();
}

/// <summary>
/// ��������
/// </summary>
void TcpSocket::createConnections()
{
	connect(this, &QTcpSocket::readyRead, this, &TcpSocket::read);
}
