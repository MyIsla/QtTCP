#pragma once
#include <QTcpServer>
#include <QList>
#include <QHostAddress>
#include "TcpModel.h"

/// <summary>
/// Tcp������
/// ���պͷַ����ͻ��˵���Ϣ
/// </summary>
class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	TcpServer();
	~TcpServer();
public slots:
	void startServer(QString addr, QString port);				//�������� 
	void closeServer();											//�رշ���
	//From recvModel
	void recvDataFromModel(QByteArray buf);					//��������������ַ����ͻ���
protected:
	void incomingConnection(qintptr socketDescriptor) override;	//���ؿͻ������Ӻ�������ÿ���ͻ��˷���һ���߳���
private:
	QList<TcpModel*> modelList;
	void createConnections(TcpModel* model);
private slots:
	void deleteThread();
};