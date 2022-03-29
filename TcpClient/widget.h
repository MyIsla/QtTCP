#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include <QFileDialog>
#include "TcpClient.h"

class widget : public QWidget
{
	Q_OBJECT
public:
	widget();
	~widget();

private:
	QVBoxLayout* vLayout;
	QLabel* sendLab;
	QLabel* recvLab;
	QTextEdit* sendEdit;
	QTextEdit* recvEdit;
	QPushButton* sendBtn;

	QHBoxLayout* hLayout;
	QLineEdit* lineEdit;
	QPushButton* lineBtn;

	QLabel* ele;

	QLabel* numLab;
	QLabel* picLab;
	QPushButton* fileBtn;
	
private slots:
	void sendLine();
	void sendText();
	void recvText(QString, QString);
	void recvDevi(QString);
	void recveler(QString,QString);
	void recvfileNum(QString,QString);
	void recvPic(QString, QBitmap);
	void recvPic(QString, QPixmap);
	void seletefile();
};
