#pragma once
#include <QString>
namespace TIGER_SerialPortDef
{
	struct CSerialPortPara
	{
		QString pushStorageScanner;
		QString pullStorageScanner;
		CSerialPortPara::CSerialPortPara() : pushStorageScanner("COM8"), pullStorageScanner("COM10"){};
	};
	CSerialPortPara *serialPortParas();
};