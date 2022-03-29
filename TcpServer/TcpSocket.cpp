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
	//qDebug() << "TcpSocket Thread:" << this->thread();
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
