#pragma once
#include <QObject>
#include <QTimer>
#include <QDebug>

#define TCPDATA_TIMER_RUN 1				//������ʱ
#define TCPDATA_DATA_TIME 60			//���ݶ�ʱ��

/// ��������
/// �׽��ֹ涨ͷ�ĸ�ʽΪ
/// type,filename,data,filesize,state,md5
/// 
/// type:�������� message,csv,png...
/// filename:�ļ�����
/// data:message�е���Ϣ
/// filesize:�ļ���С
/// state:0��ʾ��������������ͷ,1��ʾ�ҷ���ȥ��ͷ���յ���׼���������ˣ�2��ʾ��Ҫ����һ���ͷ
/// md5:У��
class TcpData : public QObject
{
	Q_OBJECT
public:
	QString type;				//�������� message,png,csv...
	QString fileName;			//�ļ�����/�豸����
	quint64 fileSize;			//���������С
	quint64 nowSize = 0;		//Ŀǰ�ܹ��յ�/���ʹ�С
	QByteArray buf = {};		//����
	QString md5;				//md5ÿ�θ���

	//���캯��
	TcpData();
	TcpData(QString type = "", QString fileName = "", quint64 fileSize = 0, QString md5 = "", quint64 nowSize = 0, QByteArray buf = {});
	//��������
	~TcpData();

#if TCPDATA_TIMER_RUN
public:
	void resetDataTime();	//���ü�ʱ
private:
	int dataTime;		//�ϴν���/�����ļ���ʱ�䣬��Ϊ��λ
	QTimer* timer;		//��ʱ�������ڼ���������ݵ�ʱ�䣬�������60��û�����ݸ��£��Ͱ��������
private slots:
	void timesUp();		//timer�����Ĳۺ��������ڶ�ʱɾ��û�н��ȵ�����
signals:
	void clearData(TcpData* data);	//ɾ��û�н��ȵ������ź�
#endif // TCPDATA_TIMER_RUN

};