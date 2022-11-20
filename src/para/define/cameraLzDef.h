#pragma once
#include <QString>
namespace TIGER_CameraLzDef
{
	struct CCameraParaLz
	{
		int exposureTime;
		int heartbeatTime;
		QString ip;
		CCameraParaLz::CCameraParaLz()
		{
			exposureTime = 35000;
			heartbeatTime = 5000;
			ip = "192.168.0.10";
		};
	};
	CCameraParaLz *cameraParasLz();
};