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
/// 开启服务器
/// </summary>
/// <param name="addr">ip</param>
/// <param name="port">端口号</param>
void TcpServer::startServer(QString addr, QString port)
{
	if (!this->isListening())
	{
		this->listen(QHostAddress(addr), quint16(port.toInt()));
		qDebug() << "服务器启动\nIP:" << addr << "\n" << "端口号" << port;
	}
	else
	{
		closeServer();
		this->startServer(addr, port);
	}
}

/// <summary>
/// 关闭服务器
/// </summary>
void TcpServer::closeServer()
{
	this->close();
	qDebug() << "服务器关闭";
}

/// <summary>
/// 接收数据做处理分发给客户端
/// </summary>
/// <param name="buf">数据</param>
void TcpServer::recvDataFromModel(QByteArray buf)
{
	//qDebug() << "Server Thread:" << this->thread();
	TcpModel* model = (TcpModel*)sender();
	//CAM,IPC,GUI
	if (model->type == "" && (buf.contains("CAM") || (buf.contains("IPC") || buf.contains("GUI"))))
	{
		qDebug() << "客户端" + QString(buf) + "接入";
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
/// 重载客户端连接函数，将每个客户端放入一个线程中
/// </summary>
/// <param name="socketDescriptor">套接字指针</param>
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "有客户端接入";
	QThread* thread = new QThread();
	TcpModel* model = new TcpModel(socketDescriptor);
	createConnections(model);
	modelList.append(model);
	model->moveToThread(thread);
	thread->start();
	model->sendDataFromServer("connected");
}

/// <summary>
/// 删除list对象
/// </summary>
void TcpServer::deleteThread()
{
	TcpModel* thread = (TcpModel*)sender();
	for (int i = 0; i < modelList.count(); i++)
	{
		if (thread == modelList[i])
		{
			qDebug() << "线程已退出";
			modelList[i]->deleteLater();
			modelList[i] = nullptr;
			modelList.removeAt(i);
		}
	}
}

void TcpServer::createConnections(TcpModel* model)
{
	//From thread	从thread回传的信号
	connect(model, &TcpModel::recvDataToServer, this, &TcpServer::recvDataFromModel);
	connect(model, &TcpModel::disconnected, this, &TcpServer::deleteThread);
}
