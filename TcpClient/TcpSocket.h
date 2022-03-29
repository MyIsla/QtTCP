#pragma once
#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>

/// <summary>
/// �׽����ֻ࣬�Ǹ�����Ϣ���շ�
/// </summary>
class TcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	TcpSocket(QString addr, QString port);
	~TcpSocket();
public slots:
	//From this
	void read();						//��ӦreadyRead
	//From TcpModel to Client
	void sendData(QByteArray buf);		//��������
private:
	void createConnections();			//��������
signals:
	//From Client to TcpModel
	void recvData(QByteArray buf);		//��������
};