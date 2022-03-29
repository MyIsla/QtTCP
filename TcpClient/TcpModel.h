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
#define TCPMODEL_HEAD_SIZE 4			//���ݿ�ͷ��С
#define TCPMODEL_BUF_SIZE 20*1024*1024	//���η��ʹ�С

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
	void createConnections();						//������socket������

	//send
	QList<TcpData*> sendList;						//������Ϣ���б�
	void sendMsgToServer(QString msg, QString type = "", int state = 0);
	void sendHeadToClient(QString type, QString fileName, quint64 fileSize, int state, QString md5 = "");//����Jsonͷ
	void sendDataToClient(QString type, QString fileName, quint64 fileSize);	//��������

	//recevice
	QList<TcpData*> recvList;								//������Ϣ���б�
	QByteArray partialBlockBuf = {};						//ʣ�಻���������ݿ�
	void recvAnalysisFromServer(QByteArray buf);			//�Խ��յ������ݽ��н���
	void recvJsonFromServer(QJsonObject obj);				//�Խӵ���json���ݽ��н���
	void recvHeadFromServer(QString type, QString fileName, quint64 fileSize, QString md5);	//����Json�õ�����ͷ
	void recvMsgFromServer(QString msg, QString name, QString md5, int state);//����Json�õ�������Ϣ
	void recvDataFromServer(QByteArray buf);				//�ӵ�����(�ֽ�����)
	
	//analysis
	void analysisData(QString fileName, QByteArray buf, QString type = "message");//������������
	void analysisMSG(QString fileName, QString buf);		//������Ϣ
	void analysisBMP(QString fileName, QByteArray buf);		//����bmp����
	void analysisPNG(QString fileName, QByteArray buf);		//����png����
	void analysisCSV(QString fileName, QByteArray buf);		//����csv����
	void analysisJson(QString fileName, QByteArray buf);	//����json����

private slots:
	//From TcpSocket
	void recvDataFromSocket(QByteArray buf);		//��������
	//From TcpData
	void clearMember(TcpData* obj);					//�����Ա
signals:
	//From TcpSocket to TcpClient
	void disconnected();							//�Ͽ�����
	void recvMsgToClient(QString type, QString msg);
	void recvDeviceToClient(QString devlist);
	void recvFileNumToClient(QString deviceNum, QString fileNum);
	void recvEletricityToClient(QString powerNum, QString power);
	void recvBmpToClient(QString deviceNum, QBitmap bmp);
	void recvPngToClient(QString deviceNum, QPixmap png);
	//From TcpClient to TcpSocket
	void sendDataToSocket(QByteArray buf);			//��������
};