#pragma once
#include <QTcpSocket>
#include <QByteArray>

/// <summary>
/// �׽����ֻ࣬�Ǹ�����Ϣ���շ�
/// </summary>
class TcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	TcpSocket(const qintptr socketDescriptor);
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