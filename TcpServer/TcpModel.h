#pragma once
#include <QObject>
#include <QThread>
#include "TcpSocket.h"
#include "TrHelper.h"
#include "TcpData.h"

#define TCPMODEL_TEST 0
#define TCPMODEL_HEAD_SIZE 4					//���ݿ�ͷ��С
#define TCPMODEL_BUF_SIZE 10*1024*1024			//���η��ʹ�С

class TcpModel : public QObject
{
	Q_OBJECT
public:
	QString type = "";
	TcpModel(const qintptr socketDescriptor);
	~TcpModel();
public slots:
	//From TcpServer to TcpSocket
	void sendDataFromServer(QByteArray buf);	//��������
private:
	TcpSocket *socket;
	QByteArray partialBlockBuf = {};			//ʣ�಻���������ݿ�
	void createConnections();					//������socket������
private slots:
	//From TcpSocket
	void recvDataFromSocket(QByteArray buf);	//��������
signals:
	//From TcpSocket to TcpServer
	void disconnected();						//�Ͽ�����
	void recvDataToServer(QByteArray buf);
	//From TcpServer to TcpSocket
	void sendDataToSocket(QByteArray buf);		//��������
};