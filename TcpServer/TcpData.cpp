#pragma execution_character_set("utf-8")
#include "TcpData.h"

TcpData::TcpData()
{
}

TcpData::TcpData(QString type, QString fileName, quint64 fileSize, QString md5, quint64 nowSize, QByteArray buf)
{
	this->type = type;
	this->fileName = fileName;
	this->fileSize = fileSize;
	this->nowSize = nowSize;
	this->buf += buf;
	this->md5 = md5;

#if TCPDATA_TIMER_RUN
	this->dataTime = 0;
	this->timer = new QTimer();
	timer->setInterval(1000);		//1s����һ��
	connect(timer, SIGNAL(timeout()), this, SLOT(timesUp()));
	timer->start();
#endif // TIMER_RUN
}

TcpData::~TcpData()
{
#if TCPDATA_TIMER_RUN
	if (timer->isActive())
		timer->stop();
	delete timer;
	timer = nullptr;
#endif // TCPDATA_TIMER_RUN
}

#if TCPDATA_TIMER_RUN
/// <summary>
/// ���ü�ʱ
/// </summary>
void TcpData::resetDataTime()
{
	dataTime = 0;
}

/// <summary>
/// timer�����Ĳۺ��������ڶ�ʱɾ��û�н��ȵ�����
/// </summary>
void TcpData::timesUp()
{
	//60s ��Ӧʱ��
	if (dataTime >= TCPDATA_DATA_TIME)
	{
		timer->stop();
		emit clearData(this);
	}
	dataTime++;
}
#endif // TIMER_RUN