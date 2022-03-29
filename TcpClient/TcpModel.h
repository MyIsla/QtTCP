#pragma once
#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCrypToGraphicHash>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>
#include <QPixmap>
#include <QBitmap>
#include <QDateTime>
#include <QCoreApplication>
#include "TcpSocket.h"
#include "TrHelper.h"
#include "TcpData.h"

#define TCPMODEL_TEST 0
#define TCPMODEL_HEAD_SIZE 4			//数据块头大小
#define TCPMODEL_BUF_SIZE 20*1024*1024	//单次发送大小

class TcpModel : public QObject
{
	Q_OBJECT
public:
	TcpModel(QString addr, QString port);
	~TcpModel();
public slots:
	//From TcpClient
	void sendClientTypeFromClient(QString type);
	void sendMsgFromClient(QString msg, QString type);
	void sendFileFromClient(QString path, QString type);
private:
	TcpSocket *socket;
	void createConnections();						//创建与socket的连接

	//send
	QList<TcpData*> sendList;						//发送消息的列表
	void sendMsgToServer(QString msg, QString type = "", int state = 0);
	void sendHeadToClient(QString type, QString fileName, quint64 fileSize, int state, QString md5 = "");//发送Json头
	void sendDataToClient(QString type, QString fileName, quint64 fileSize);	//发送数据

	//recevice
	QList<TcpData*> recvList;								//发送消息的列表
	QByteArray partialBlockBuf = {};						//剩余不完整的数据块
	void recvAnalysisFromServer(QByteArray buf);			//对接收到的数据进行解析
	void recvJsonFromServer(QJsonObject obj);				//对接到的json数据进行解析
	void recvHeadFromServer(QString type, QString fileName, quint64 fileSize, QString md5);	//解析Json得到的是头
	void recvMsgFromServer(QString msg, QString name, QString md5, int state);//解析Json得到的是消息
	void recvDataFromServer(QByteArray buf);				//接到数据(字节数组)
	
	//analysis
	void analysisData(QString fileName, QByteArray buf, QString type = "message");//初步解析数据
	void analysisMSG(QString fileName, QString buf);		//解析信息
	void analysisBMP(QString fileName, QByteArray buf);		//解析bmp数据
	void analysisPNG(QString fileName, QByteArray buf);		//解析png数据
	void analysisCSV(QString fileName, QByteArray buf);		//解析csv数据
	void analysisJson(QString fileName, QByteArray buf);	//解析json数据

private slots:
	//From TcpSocket
	void recvDataFromSocket(QByteArray buf);		//接收数据
	//From TcpData
	void clearMember(TcpData* obj);					//清除成员
signals:
	//From TcpSocket to TcpClient
	void disconnected();							//断开连接
	void recvMsgToClient(QString type, QString msg);
	void recvDeviceToClient(QString devlist);
	void recvFileNumToClient(QString deviceNum, QString fileNum);
	void recvEletricityToClient(QString powerNum, QString power);
	void recvBmpToClient(QString deviceNum, QBitmap bmp);
	void recvPngToClient(QString deviceNum, QPixmap png);
	//From TcpClient to TcpSocket
	void sendDataToSocket(QByteArray buf);			//发送数据
};