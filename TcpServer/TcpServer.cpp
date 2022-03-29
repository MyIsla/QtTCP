#pragma execution_character_set("utf-8")
#include "TcpServer.h"

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
	qDeleteAll(modelList);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="addr">ip</param>
/// <param name="port">�˿ں�</param>
void TcpServer::startServer(QString addr, QString port)
{
	if (!this->isListening())
	{
		this->listen(QHostAddress(addr), quint16(port.toInt()));
		qDebug() << "����������\nIP:" << addr << "\n" << "�˿ں�" << port;
	}
	else
	{
		closeServer();
		this->startServer(addr, port);
	}
}

/// <summary>
/// �رշ�����
/// </summary>
void TcpServer::closeServer()
{
	this->close();
	qDebug() << "�������ر�";
}

/// <summary>
/// ��������������ַ����ͻ���
/// </summary>
/// <param name="buf">����</param>
void TcpServer::recvDataFromModel(QByteArray buf)
{
	//qDebug() << "Server Thread:" << this->thread();
	TcpModel* model = (TcpModel*)sender();
	//CAM,IPC,GUI
	if (model->type == "" && (buf.contains("CAM") || (buf.contains("IPC") || buf.contains("GUI"))))
	{
		qDebug() << "�ͻ���" + QString(buf) + "����";
		model->type = buf;
		return;
	}
	if (model->type == "GUI")
	{
		for (int i = 0; i < modelList.count(); i++)
		{
			if (modelList[i]->type != "GUI")
				modelList[i]->sendDataFromServer(buf);
		}
	}
	else
	{
		for (int i = 0; i < modelList.count(); i++)
		{
			if (modelList[i]->type == "GUI")
				modelList[i]->sendDataFromServer(buf);
		}
	}
}

/// <summary>
/// ���ؿͻ������Ӻ�������ÿ���ͻ��˷���һ���߳���
/// </summary>
/// <param name="socketDescriptor">�׽���ָ��</param>
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "�пͻ��˽���";
	QThread* thread = new QThread();
	TcpModel* model = new TcpModel(socketDescriptor);
	createConnections(model);
	modelList.append(model);
	model->moveToThread(thread);
	thread->start();
	model->sendDataFromServer("connected");
}

/// <summary>
/// ɾ��list����
/// </summary>
void TcpServer::deleteThread()
{
	TcpModel* thread = (TcpModel*)sender();
	for (int i = 0; i < modelList.count(); i++)
	{
		if (thread == modelList[i])
		{
			qDebug() << "�߳����˳�";
			modelList[i]->deleteLater();
			modelList[i] = nullptr;
			modelList.removeAt(i);
		}
	}
}

void TcpServer::createConnections(TcpModel* model)
{
	//From thread	��thread�ش����ź�
	connect(model, &TcpModel::recvDataToServer, this, &TcpServer::recvDataFromModel);
	connect(model, &TcpModel::disconnected, this, &TcpServer::deleteThread);
}
