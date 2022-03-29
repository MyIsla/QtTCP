#pragma execution_character_set("utf-8")
#include "TcpClient.h"

TcpClient::TcpClient()
{
}

TcpClient::~TcpClient()
{
}

TcpClient* TcpClient::instance = nullptr;

void TcpClient::createConnections(TcpModel* model)
{
	connect(this, &TcpClient::sendClientTypeToModel, model, &TcpModel::sendClientTypeFromClient);
	connect(this, &TcpClient::sendMsgToModel, model, &TcpModel::sendMsgFromClient);
	connect(this, &TcpClient::sendFileToModel, model, &TcpModel::sendFileFromClient);
	connect(model, &TcpModel::recvMsgToClient, this, &TcpClient::recvMsgFromModel);
	connect(model, &TcpModel::recvDeviceToClient, this, &TcpClient::recvDeviceFromModel);
	connect(model, &TcpModel::recvFileNumToClient, this, &TcpClient::recvFileNumFromModel);
	connect(model, &TcpModel::recvEletricityToClient, this, &TcpClient::recvEletricityFromModel);
	connect(model, &TcpModel::recvBmpToClient, this, &TcpClient::recvBmpFromModel);
	connect(model, &TcpModel::recvPngToClient, this, &TcpClient::recvPngFromModel);

}

TcpClient* TcpClient::getInstance()
{
	if (instance == nullptr)
		instance = new TcpClient;
	return instance;
}

void TcpClient::startServer(QString addr, QString port)
{
	QThread* thread = new QThread();
	TcpModel* model = new TcpModel(addr, port);
	createConnections(model);
	model->moveToThread(thread);
	thread->start();
}

void TcpClient::sendClientTypeFromClient(QString type)
{
	emit sendClientTypeToModel(type);
}

void TcpClient::sendMsgFromClient(QString msg, QString type /*= ""*/)
{
	emit sendMsgToModel(msg, type);
}

void TcpClient::sendFileFromClient(QString path, QString type /*= ""*/)
{
	emit sendFileToModel(path, type);
}

