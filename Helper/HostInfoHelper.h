#pragma once
#include <qhostinfo.h>

class HostInfoHelper
{
public:
	static QString localHostName();			//获取本地主机名称
	static QList<QString> localIPAddresses();	//获取本地所有IP地址
	static QString localIPv4();				//获取本地IPv4地址
	static QString localIPv6();				//获取本地IPv6地址
};