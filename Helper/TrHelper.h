#pragma once
#include <qbytearray.h>

class TrHelper
{
public:
	static QByteArray IntToQByteArray(int num);
	static int QByteArrayToInt(QByteArray array, int start);
};
