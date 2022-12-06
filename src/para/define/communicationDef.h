#pragma once
#include <QString>
namespace TIGER_PCDef
{
	struct CPCPara
	{
		QString serverIp;
		int serverPort;
		int serverAddr;
		CPCPara::CPCPara() : serverIp("192.168.0.32"), serverPort(502), serverAddr(255){};
	};
	CPCPara *pcParas();
};