#pragma execution_character_set("utf-8")
#include "TcpModel.h"

TcpModel::TcpModel(qintptr socketDescriptor)
{
	QThread* thread = new QThread();
	socket = new TcpSocket(socketDescriptor);
	socket->moveToThread(thread);
	createConnections();
	thread->start();
}

TcpModel::~TcpModel()
{
}

void TcpModel::sendDataFromServer(QByteArray buf)
{
#if TCPMODEL_TEST
	//qDebug() << "TcpModel Thread:" << this->thread();
	emit sendDataToSocket(buf);
	return;
#endif // TCPMODEL_TEST

	QByteArray sendBuf = TrHelper::IntToQByteArray(buf.size());
	sendBuf += buf;
	emit sendDataToSocket(sendBuf);
}

/// <summary>
/// 创建与socket的连接
/// </summary>
void TcpModel::createConnections()
{
	connect(this, &TcpModel::sendDataToSocket, socket, &TcpSocket::sendData);
	connect(socket, &TcpSocket::disconnected, this, &TcpModel::disconnected);
	connect(socket, &TcpSocket::recvData, this, &TcpModel::recvDataFromSocket);
}

void TcpModel::recvDataFromSocket(QByteArray buf)
{
#if TCPMODEL_TEST
	emit recvDataToServer(buf);
	return;
#endif // TCPMODEL_TEST

	buf = partialBlockBuf + buf;
	//解析完成的数据大小
	while (buf.size() != 0)
	{
		int nowSize = TrHelper::QByteArrayToInt(buf, 0);
		if (buf.size() - TCPMODEL_HEAD_SIZE >= nowSize)
		{
			emit recvDataToServer(buf.mid(TCPMODEL_HEAD_SIZE, nowSize));
			buf = buf.mid(TCPMODEL_HEAD_SIZE + nowSize, buf.size() - (TCPMODEL_HEAD_SIZE + nowSize));
			//数据包可以完整，代表partialBlockBuf已经可以清0了，等待下一次赋值
			if (partialBlockBuf.size() != 0)partialBlockBuf = {};
		}
		else
		{
			partialBlockBuf = buf;
			buf = buf.mid(buf.size(), buf.size() - buf.size());
		}
	}
}
