#pragma execution_character_set("utf-8")
#include "widget.h"

widget::widget()
{
	vLayout = new QVBoxLayout();
	sendLab = new QLabel("发送");
	recvLab = new QLabel("接收");
	sendEdit = new QTextEdit();
	recvEdit = new QTextEdit();
	sendBtn = new QPushButton();
	hLayout = new QHBoxLayout();
	lineEdit = new QLineEdit();
	lineBtn = new QPushButton();
	ele = new QLabel();
	numLab = new QLabel();
	picLab = new QLabel();
	picLab->setFixedSize(278, 278);
	fileBtn = new QPushButton("...");

	hLayout->addWidget(lineEdit);
	hLayout->addWidget(lineBtn);
	hLayout->addWidget(fileBtn);

	vLayout->addLayout(hLayout);
	vLayout->addWidget(sendLab);
	vLayout->addWidget(sendEdit);
	vLayout->addWidget(sendBtn);
	vLayout->addWidget(recvLab);
	vLayout->addWidget(recvEdit);


	vLayout->addWidget(ele);
	vLayout->addWidget(numLab);
	vLayout->addWidget(picLab);


	this->setLayout(vLayout);

	connect(sendBtn, &QPushButton::clicked, this, &widget::sendText);
	connect(lineBtn, &QPushButton::clicked, this, &widget::sendLine);
	connect(fileBtn, &QPushButton::clicked, this, &widget::seletefile);
	connect(TcpClient::getInstance(), &TcpClient::recvMsgFromModel, this, &widget::recvText);
	connect(TcpClient::getInstance(), &TcpClient::recvDeviceFromModel, this, &widget::recvDevi);
	connect(TcpClient::getInstance(), &TcpClient::recvEletricityFromModel, this, &widget::recveler);
	connect(TcpClient::getInstance(), &TcpClient::recvFileNumFromModel, this, &widget::recvfileNum);
	connect(TcpClient::getInstance(), &TcpClient::recvBmpFromModel, this, qOverload<QString,QBitmap>(&widget::recvPic));
	connect(TcpClient::getInstance(), &TcpClient::recvPngFromModel, this, qOverload<QString,QPixmap>(&widget::recvPic));
}

widget::~widget()
{
}

void widget::sendLine()
{
	TcpClient::getInstance()->sendClientTypeFromClient(lineEdit->text());
	lineBtn->setEnabled(false);
}

void widget::sendText()
{
	QString str = sendEdit->toPlainText();
	QStringList list = str.split(" ");
	if (list.count() < 2)
		TcpClient::getInstance()->sendMsgFromClient(list[0]);
	else
		TcpClient::getInstance()->sendMsgFromClient(list[0], list[1]);
	sendEdit->clear();

}

void widget::recvText(QString type, QString msg)
{
	recvEdit->append(msg + " " + type);
}

void widget::recvDevi(QString msg)
{
	recvEdit->append(msg);
}

void widget::recveler(QString powernum, QString power)
{
	ele->setText("电量" + powernum + ":" + power + "%");
}

void widget::recvfileNum(QString fileName,QString fileNum)
{
	numLab->setText("相机" + fileName + "数量:" + fileNum);
}

void widget::recvPic(QString, QBitmap p)
{
	p.scaledToHeight(picLab->height());
	picLab->setPixmap(p.scaledToWidth(picLab->width()));
	picLab->update();
}

void widget::recvPic(QString, QPixmap p)
{
	p.scaledToHeight(picLab->height());
	picLab->setPixmap(p.scaledToWidth(picLab->width()));
	picLab->update();
}

void widget::seletefile()
{
	QStringList filePath = QFileDialog::getOpenFileNames(this, "相机配置文件(先json,后csv)", "C:\\Users\\Administrator\\Desktop\\", "*.csv *.json;;*.*");
	for (int i = 0; i < filePath.count(); i++)
	{
		if (filePath[i] == "")continue;
		TcpClient::getInstance()->sendFileFromClient(filePath[i]);
	}
}