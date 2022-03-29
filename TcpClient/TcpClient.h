#pragma once
#include <QObject>
#include "TcpModel.h"

class TcpClient : public QObject
{
	Q_OBJECT
public:
	TcpClient();
	~TcpClient();
	static TcpClient* getInstance();
public slots:
	void startServer(QString addr,QString port);
	void sendClientTypeFromClient(QString type);
	void sendMsgFromClient(QString msg, QString type = "");
	void sendFileFromClient(QString path, QString type = "");
private:
	static TcpClient* instance;
	void createConnections(TcpModel* model);
signals:
	void sendClientTypeToModel(QString type);
	void sendMsgToModel(QString msg, QString type);						//������Ϣ
	void sendFileToModel(QString path, QString type);					//�����ļ�
	void recvMsgFromModel(QString type, QString msg);					//��ȡһ����Ϣ
	void recvDeviceFromModel(QString devlist);							//��ȡ�豸�б�
	void recvFileNumFromModel(QString deviceNum, QString fileNum);		//��ȡ�ļ���Ϣ
	void recvEletricityFromModel(QString powerNum, QString power);		//��ȡ����
	void recvBmpFromModel(QString deviceNum, QBitmap bmp);				//��ȡbmp
	void recvPngFromModel(QString deviceNum, QPixmap png);				//��ȡpng
};