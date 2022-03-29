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
/// ������socket������
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
	//������ɵ����ݴ�С
	while (buf.size() != 0)
	{
		int nowSize = TrHelper::QByteArrayToInt(buf, 0);
		if (buf.size() - TCPMODEL_HEAD_SIZE >= nowSize)
		{
			emit recvDataToServer(buf.mid(TCPMODEL_HEAD_SIZE, nowSize));
			buf = buf.mid(TCPMODEL_HEAD_SIZE + nowSize, buf.size() - (TCPMODEL_HEAD_SIZE + nowSize));
			//���ݰ���������������partialBlockBuf�Ѿ�������0�ˣ��ȴ���һ�θ�ֵ
			if (partialBlockBuf.size() != 0)partialBlockBuf = {};
		}
		else
		{
			partialBlockBuf = buf;
			buf = buf.mid(buf.size(), buf.size() - buf.size());
		}
	}
}
