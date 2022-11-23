#pragma once
#include <QString>
namespace TIGER_ScannerDef
{
	struct CScannerPara
	{
		QString scannerPort;
		CScannerPara::CScannerPara() : scannerPort("COM5"){};
	};
	CScannerPara *scannerParas();
};