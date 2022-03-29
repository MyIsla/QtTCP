#pragma once
#include <QObject>
#include <QThread>
#include "TcpSocket.h"
#include "TrHelper.h"
#include "TcpData.h"

#define TCPMODEL_TEST 0
#define TCPMODEL_HEAD_SIZE 4					//数据块头大小
#define TCPMODEL_BUF_SIZE 10*1024*1024			//单次发送大小

class TcpModel : public QObject
{
	Q_OBJECT
public:
	QString type = "";
	TcpModel(const qintptr socketDescriptor);
	~TcpModel();
public slots:
	//From TcpServer to TcpSocket
	void sendDataFromServer(QByteArray buf);	//发送数据
private:
	TcpSocket *socket;
	QByteArray partialBlockBuf = {};			//剩余不完整的数据块
	void createConnections();					//创建与socket的连接
private slots:
	//From TcpSocket
	void recvDataFromSocket(QByteArray buf);	//接收数据
signals:
	//From TcpSocket to TcpServer
	void disconnected();						//断开连接
	void recvDataToServer(QByteArray buf);
	//From TcpServer to TcpSocket
	void sendDataToSocket(QByteArray buf);		//发送数据
};