#pragma once
#include <QTcpServer>
#include <QList>
#include <QHostAddress>
#include "TcpModel.h"

/// <summary>
/// Tcp服务器
/// 接收和分发各客户端的信息
/// </summary>
class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	TcpServer();
	~TcpServer();
public slots:
	void startServer(QString addr, QString port);				//开启服务 
	void closeServer();											//关闭服务
	//From recvModel
	void recvDataFromModel(QByteArray buf);					//接收数据做处理分发给客户端
protected:
	void incomingConnection(qintptr socketDescriptor) override;	//重载客户端连接函数，将每个客户端放入一个线程中
private:
	QList<TcpModel*> modelList;
	void createConnections(TcpModel* model);
private slots:
	void deleteThread();
};