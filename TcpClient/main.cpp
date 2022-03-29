#pragma once
#include <QtCore/QCoreApplication>
#include <QApplication>
#include "TcpClient.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread* thread = new QThread();
    TcpClient::getInstance()->moveToThread(thread);
    thread->start();
    TcpClient::getInstance()->startServer("172.16.30.106", "8088");
    widget w;
    w.show();
    return a.exec();
}
