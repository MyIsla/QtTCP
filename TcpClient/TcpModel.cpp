#pragma execution_character_set("utf-8")
#include "TcpModel.h"

TcpModel::TcpModel(QString addr, QString port)
{
	QThread* thread = new QThread();
	socket = new TcpSocket(addr, port);
	socket->moveToThread(thread);
	createConnections();
	thread->start();
}

TcpModel::~TcpModel()
{
}

void TcpModel::sendClientTypeFromClient(QString type)
{
	QByteArray buf = type.toUtf8();
	QByteArray sendBuf = TrHelper::IntToQByteArray(buf.size());
	sendBuf += buf;
	emit sendDataToSocket(sendBuf);
}

void TcpModel::sendMsgFromClient(QString msg, QString type)
{
	sendMsgToServer(msg, type);
}

void TcpModel::sendFileFromClient(QString path, QString type)
{
	//��ȡ�ļ���Ϣ
	QFileInfo fileInfo(path);
	if (type == "")
		type = fileInfo.suffix();
	QString fileName = fileInfo.baseName();
	//�鿴�Ƿ����б���
	for (int i = 0; i < sendList.count(); i++)
	{
		if (sendList[i]->type == type && sendList[i]->fileName == fileName)
			return;
	}
	//�������ڴ���
	QFile file;
	file.setFileName(path);
	file.open(QIODevice::ReadOnly);
	QByteArray buf = file.readAll();
	file.close();
	TcpData* data = new TcpData(type, fileName, file.size(), "", 0, buf);
	connect(data, SIGNAL(clearData(TcpData*)), this, SLOT(clearMember(TcpData*)));
	sendList.append(data);
	sendHeadToClient(type, fileName, file.size(), 0);
}

/// <summary>
/// ������socket������
/// </summary>
void TcpModel::createConnections()
{
	connect(this, &TcpModel::sendDataToSocket, socket, &TcpSocket::sendData);
	connect(socket, &TcpSocket::disconnected, this, &TcpModel::disconnected);
	connect(socket, &TcpSocket::recvData, this, &TcpModel::recvDataFromSocket);
}

void TcpModel::sendMsgToServer(QString msg, QString type /*= 0*/, int state /*= 0*/)
{
	//qDebug() << "Server send:" << msg << " The state:" << QString::number(state);
	QJsonObject jsonObj;
	jsonObj.insert("type", "message");
	jsonObj.insert("filename", type);
	jsonObj.insert("data", msg);
	jsonObj.insert("filesize", QString::number(msg.size()));
	jsonObj.insert("state", QString::number(state));
	jsonObj.insert("md5", QString(QCryptographicHash::hash(msg.toUtf8(), QCryptographicHash::Md5).toHex()));
	QJsonDocument jsonDoc(jsonObj);
	QByteArray buf = jsonDoc.toJson();
	QByteArray sendBuf = TrHelper::IntToQByteArray(buf.size());
	sendBuf += buf;
	emit sendDataToSocket(sendBuf);
}

void TcpModel::sendHeadToClient(QString type, QString fileName, quint64 fileSize, int state, QString md5 /*= ""*/)
{
	//������Ϊ����ͷ��״̬
	if (state == 0)
	{
		bool get = false;
		for (int i = 0; i < sendList.count(); i++)
		{
			if (sendList[i]->type == type && sendList[i]->fileName == fileName &&
				sendList[i]->fileSize == fileSize)
			{
				QByteArray block = {};
				if (sendList[i]->nowSize + TCPMODEL_BUF_SIZE > sendList[i]->fileSize)
					block = sendList[i]->buf.mid(sendList[i]->nowSize, sendList[i]->fileSize - sendList[i]->nowSize);
				else
					block = sendList[i]->buf.mid(sendList[i]->nowSize, TCPMODEL_BUF_SIZE);
				md5 = QString(QCryptographicHash::hash(block, QCryptographicHash::Md5).toHex());
				get = true;
				break;
			}
		}
		if (!get) return;
	}
	QJsonObject jsonObj;
	jsonObj.insert("type", type);
	jsonObj.insert("filename", fileName);
	jsonObj.insert("data", "");
	jsonObj.insert("filesize", QString::number(fileSize));
	jsonObj.insert("state", QString::number(state));
	jsonObj.insert("md5", md5);
	QJsonDocument jsonDoc(jsonObj);
	QByteArray buf = jsonDoc.toJson();
	//qDebug() << "send to client:" << "\n" << buf;
	QByteArray sendBuf = TrHelper::IntToQByteArray(buf.size());
	sendBuf += buf;
	emit sendDataToSocket(sendBuf);
}

void TcpModel::sendDataToClient(QString type, QString fileName, quint64 fileSize)
{
	for (int i = 0; i < sendList.count(); i++)
	{
		if (sendList[i]->type == type && sendList[i]->fileName == fileName &&
			sendList[i]->fileSize == fileSize)
		{
			sendList[i]->resetDataTime();
			QByteArray buf = {};
			if (sendList[i]->nowSize + TCPMODEL_BUF_SIZE > sendList[i]->fileSize)
				buf = sendList[i]->buf.mid(sendList[i]->nowSize, sendList[i]->fileSize - sendList[i]->nowSize);
			else
			{
				buf = sendList[i]->buf.mid(sendList[i]->nowSize, TCPMODEL_BUF_SIZE);
			}
			sendList[i]->nowSize += TCPMODEL_BUF_SIZE;
			if (sendList[i]->nowSize >= sendList[i]->fileSize)
			{
				clearMember(sendList[i]);
			}
			QByteArray sendBuf = TrHelper::IntToQByteArray(buf.size());
			sendBuf += buf;
			emit sendDataToSocket(sendBuf);
			return;
		}
	}
}

void TcpModel::recvAnalysisFromServer(QByteArray buf)
{
	//Э�̺� �����json��ʽ/ֱ����������
	//�����Json��ʽ������һ������
	if (QJsonDocument::fromJson(buf).isObject())
	{
		//qDebug() << "GetBuffer:\n" << buf << "\n\n\n";
		recvJsonFromServer(QJsonDocument::fromJson(buf).object());
	}
	//�����Ϊjson��ʽ���Ǿ�������
	else if (buf.contains("connected"))
	{
		qDebug() << buf;
	}
	else
	{
		recvDataFromServer(buf);
	}
}

void TcpModel::recvJsonFromServer(QJsonObject obj)
{
	//û��type�ֶ�
	if (!obj.contains("type")) return;
	QString type = obj.value("type").toString();
	//û��filename�ֶ�
	if (!obj.contains("filename")) return;
	QString filename = obj.value("filename").toString();
	//û��data�ֶ�
	if (!obj.contains("data")) return;
	QString data = obj.value("data").toString();
	//û��filesize�ֶ�
	if (!obj.contains("filesize")) return;
	quint64 filesize = obj.value("filesize").toString().toInt();
	//û��state�ֶ�
	if (!obj.contains("state")) return;
	int state = obj.value("state").toString().toInt();
	//û��md5�ֶ�
	if (!obj.contains("md5"))return;
	QString md5 = obj.value("md5").toString();
	//�ж�����Ϣ�����ļ�
	if (type == "message")
	{
		recvMsgFromServer(data, filename, md5, state);
		return;
	}
	//�жϴ�����Ϣ��״̬
	switch (state)
	{
	case 0: recvHeadFromServer(type, filename, filesize, md5); break;//���յ�ͷ�ļ�
	case 1:	sendDataToClient(type, filename, filesize); break;//������
	case 2: sendHeadToClient(type, filename, filesize, 0); break;//������һ��ͷ�ļ�
	default:
		break;
	}
}

void TcpModel::recvHeadFromServer(QString type, QString fileName, quint64 fileSize, QString md5)
{
	//�鿴���յ��б������޼�¼
	for (int i = 0; i < recvList.count(); i++)
	{
		if (recvList[i]->type == type && recvList[i]->fileName == fileName &&
			recvList[i]->fileSize == fileSize)
		{
			//�м�¼������MD5Ϊ��ʹ��һ�η����������ݿ��Խ�����֤
			recvList[i]->md5 = md5;
			sendHeadToClient(recvList[i]->type, recvList[i]->fileName,
				recvList[i]->fileSize, 1, recvList[i]->md5);
			return;
		}
	}
	//�޼�¼���½���¼
	TcpData* data = new TcpData(type, fileName, fileSize, md5);
	connect(data, SIGNAL(clearData(TcpData*)), this, SLOT(clearMember(TcpData*)));
	recvList.append(data);
	//�б��е����һ��
	sendHeadToClient(recvList[recvList.count() - 1]->type, recvList[recvList.count() - 1]->fileName,
		recvList[recvList.count() - 1]->fileSize, 1, recvList[recvList.count() - 1]->md5);
}

void TcpModel::recvMsgFromServer(QString msg, QString name, QString md5, int state)
{
	if (QString(QCryptographicHash::hash(msg.toUtf8(), QCryptographicHash::Md5).toHex()) != md5)
	{
		return;
	}
	if (state == 0)
	{
		sendMsgToServer(msg, name, 1);
		analysisData(name, msg.toUtf8());
	}
	else if (state == 1)
	{
		sendMsgToServer(msg, name, 2);
		return;
	}
}

void TcpModel::recvDataFromServer(QByteArray buf)
{
	//qDebug() << "Clinet buf size:" << buf.size();
	bool get = false;
	for (int i = 0; i < recvList.count(); i++)
	{
		if (recvList[i]->md5 == QString(QCryptographicHash::hash(buf, QCryptographicHash::Md5).toHex()))
		{
			recvList[i]->resetDataTime();
			recvList[i]->nowSize += buf.size();
			recvList[i]->buf += buf;
			get = true;
			sendHeadToClient(recvList[i]->type, recvList[i]->fileName,
				recvList[i]->fileSize, 2, recvList[i]->md5);
			if (recvList[i]->fileSize <= recvList[i]->nowSize)
			{
				analysisData(recvList[i]->fileName, recvList[i]->buf, recvList[i]->type);
				clearMember(recvList[i]);
			}
			break;
		}
	}
	if (!get)
	{
		qDebug() << "md5����";
		return;
	}
}

void TcpModel::analysisData(QString fileName, QByteArray buf, QString type /*= "message"*/)
{
	if (type == "message")
		analysisMSG(fileName, buf);
	else if (type == "bmp")
		analysisBMP(fileName, buf);
	else if (type == "png")
		analysisPNG(fileName, buf);
	else if (type == "csv")
		analysisCSV(fileName, buf);
	else if (type == "json")
		analysisJson(fileName, buf);
}

void TcpModel::analysisMSG(QString fileName, QString buf)
{
	//�����豸�б�
	if (fileName == "devlist")
	{
		QStringList list = buf.split(",");
		for (int i = 0; i < list.count(); i++)
		{
			if (list[i] == "") continue;
			emit recvDeviceToClient(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss ") + list[i] + " ���ӳɹ�");
		}
	}
	else if (fileName != "" && fileName.contains("CAM"))
		emit recvFileNumToClient(fileName, buf);
	else if (fileName != "" && fileName.contains("power"))
		emit recvEletricityToClient(fileName.replace("power", ""), QString::number(buf.toFloat()));
	else
		emit recvMsgToClient(fileName, buf);
}

void TcpModel::analysisBMP(QString fileName, QByteArray buf)
{
	QBitmap b;
	b.loadFromData(buf);
	QStringList stringList = fileName.split("_");
	QString num = stringList[0].replace(QRegExp("[a-z]|[A-Z]"), "");
	emit recvBmpToClient(num, b);
}

void TcpModel::analysisPNG(QString fileName, QByteArray buf)
{
	QPixmap p;
	p.loadFromData(buf);
	QStringList stringList = fileName.split("_");
	QString num = stringList[0].replace(QRegExp("[a-z]|[A-Z]"), "").toInt();
	emit recvPngToClient(num, p);
}

void TcpModel::analysisCSV(QString fileName, QByteArray buf)
{
	buf.replace("\r", "");
	QString path = QCoreApplication::applicationDirPath() + "/nature.csv";
	QFile f(path);
	if (f.exists()) f.remove();
	if (!f.open(QIODevice::WriteOnly))
	{
		qDebug() << "��ʧ��";
	}
	f.write(buf, buf.size());
	f.close();
}

void TcpModel::analysisJson(QString fileName, QByteArray buf)
{
	QString path = QCoreApplication::applicationDirPath() + "/savepath.json";
	QFile f(path);
	if (f.exists()) f.remove();
	if (!f.open(QIODevice::WriteOnly))
	{
		qDebug() << "��ʧ��";
	}
	f.write(buf, buf.size());
	f.close();
}

void TcpModel::recvDataFromSocket(QByteArray buf)
{
	buf = partialBlockBuf + buf;
	//������ɵ����ݴ�С
	while (buf.size() != 0)
	{
		int nowSize = TrHelper::QByteArrayToInt(buf, 0);
		if (buf.size() - TCPMODEL_HEAD_SIZE >= nowSize)
		{
			recvAnalysisFromServer(buf.mid(TCPMODEL_HEAD_SIZE, nowSize));
			buf = buf.mid(TCPMODEL_HEAD_SIZE + nowSize, buf.size() - (TCPMODEL_HEAD_SIZE + nowSize));
			//���ݰ���������������partialBlockBuf�Ѿ�������0�ˣ��ȴ���һ�θ�ֵ
			if (partialBlockBuf.size() != 0)partialBlockBuf = {};
		}
		else
		{
			partialBlockBuf = buf;
			buf = buf.mid(buf.size(), buf.size() - buf.size());
		}
	}
}

void TcpModel::clearMember(TcpData* obj)
{
	//�ҽ����б�
	for (int i = 0; i < recvList.count(); i++)
	{
		if (recvList[i] == obj)
		{
			if (recvList[i]->nowSize >= recvList[i]->fileSize)
			{
				qDebug() << recvList[i]->fileName + "." + recvList[i]->type + "�������";
			}
			else
			{
				qDebug() << recvList[i]->fileName + "." + recvList[i]->type + "����ʧ��";
			}
			recvList[i]->deleteLater();
			recvList[i] = nullptr;
			recvList.removeAt(i);
			return;
		}
	}
	//�ҷ����б�
	for (int i = 0; i < sendList.count(); i++)
	{
		if (sendList[i] == obj)
		{
			if (sendList[i]->nowSize >= sendList[i]->fileSize)
			{
				qDebug() << sendList[i]->fileName + "." + sendList[i]->type + "�������";
			}
			else
			{
				qDebug() << sendList[i]->fileName + "." + sendList[i]->type + "����ʧ��";
			}
			sendList[i]->deleteLater();
			sendList[i] = nullptr;
			sendList.removeAt(i);
			return;
		}
	}
}
