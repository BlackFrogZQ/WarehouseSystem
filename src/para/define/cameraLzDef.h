#pragma once
#include <QString>
namespace TIGER_CameraLzDef
{
	struct CCameraParaLz
	{
		QString ip;
		int exposureTime;
		int heartbeatTime;
		CCameraParaLz::CCameraParaLz()
		{
			ip = "192.168.0.10";
			exposureTime = 35000;
			heartbeatTime = 5000;
		};
	};
	CCameraParaLz *cameraParasLz();
};