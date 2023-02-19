#pragma once
#include <QString>
namespace TIGER_SerialPortDef
{
	struct CSerialPortPara
	{
		QString scannerPort;
		QString indicatorLightPort;
		CSerialPortPara::CSerialPortPara() : scannerPort("COM5"),indicatorLightPort("COM8"){};
	};
	CSerialPortPara *serialPortParas();
};