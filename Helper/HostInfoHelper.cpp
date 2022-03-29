#pragma execution_character_set("utf-8")
#include "HostInfoHelper.h"

/// <summary>
/// ��ȡ������������
/// </summary>
/// <returns>������������</returns>
QString HostInfoHelper::localHostName()
{
	return QHostInfo::localHostName();
}

/// <summary>
/// ��ȡ��������IP��ַ
/// </summary>
QList<QString> HostInfoHelper::localIPAddresses()
{
	QList<QString> ipAddresses;
	QString localName = localHostName();
	QHostInfo info = QHostInfo::fromName(localName);
	for each (QHostAddress address in info.addresses())
	{
		ipAddresses.append(address.toString());
	}
	return ipAddresses;
}

/// <summary>
/// ��ȡ����IPv4��ַ
/// </summary>
QString HostInfoHelper::localIPv4()
{
	QString ipAddress = "";
	QString localName = localHostName();
	QHostInfo info = QHostInfo::fromName(localName);
	for each (QHostAddress address in info.addresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol)
			ipAddress = address.toString();
	}
	return ipAddress;
}

/// <summary>
/// ��ȡ����IPv6��ַ
/// </summary>
QString HostInfoHelper::localIPv6()
{
	QString ipAddress = "";
	QString localName = localHostName();
	QHostInfo info = QHostInfo::fromName(localName);
	for each (QHostAddress address in info.addresses())
	{
		if (address.protocol() == QAbstractSocket::IPv6Protocol)
			ipAddress = address.toString();
	}
	return ipAddress;
}