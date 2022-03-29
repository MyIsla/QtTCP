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
	void sendMsgToModel(QString msg, QString type);						//发送信息
	void sendFileToModel(QString path, QString type);					//发送文件
	void recvMsgFromModel(QString type, QString msg);					//获取一般信息
	void recvDeviceFromModel(QString devlist);							//获取设备列表
	void recvFileNumFromModel(QString deviceNum, QString fileNum);		//获取文件信息
	void recvEletricityFromModel(QString powerNum, QString power);		//获取电量
	void recvBmpFromModel(QString deviceNum, QBitmap bmp);				//获取bmp
	void recvPngFromModel(QString deviceNum, QPixmap png);				//获取png
};