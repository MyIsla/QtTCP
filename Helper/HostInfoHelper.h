#pragma once
#include <qhostinfo.h>

class HostInfoHelper
{
public:
	static QString localHostName();			//��ȡ������������
	static QList<QString> localIPAddresses();	//��ȡ��������IP��ַ
	static QString localIPv4();				//��ȡ����IPv4��ַ
	static QString localIPv6();				//��ȡ����IPv6��ַ
};