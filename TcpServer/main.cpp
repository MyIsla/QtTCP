#pragma once
#include <QCoreApplication>
#include <QSettings>
#include "TcpServer.h"

#define AUTO_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

/// <summary>
/// 开机自启
/// </summary>
void startAutomatically()
{
	QString appName = QCoreApplication::applicationName();
	QString appPath = QCoreApplication::applicationFilePath();
	appPath = appPath.replace("/", "\\");
	QSettings* setting = new QSettings(AUTO_KEY, QSettings::NativeFormat);
	QString val = setting->value(appName).toString();
	if (val != appPath)
		setting->setValue(appName, appPath);
	setting->deleteLater();
}

/// <summary>
/// 关闭自启
/// </summary>
void stopAutomatically()
{
	QString appName = QCoreApplication::applicationName();
	QString appPath = QCoreApplication::applicationFilePath();
	appPath = appPath.replace("/", "\\");
	QSettings* setting = new QSettings(AUTO_KEY, QSettings::NativeFormat);
	QString val = setting->value(appName).toString();
	if (val == appPath)
		setting->remove(appName);
	setting->deleteLater();
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	startAutomatically();
	//stopAutomatically();
	TcpServer* server = new TcpServer();
	server->startServer("172.16.30.106", "8088");
	return a.exec();
}
