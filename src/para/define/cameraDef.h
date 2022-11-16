#pragma once

namespace TIGER_CameraDef
{
	struct CCameraPara
	{
		int exposureTime;
		int heartbeatTime;
		QString ip;
		CCameraPara::CCameraPara()
		{
			exposureTime = 35000;
			heartbeatTime = 5000;
			ip = "192.168.0.7";
		};
	};
	CCameraPara *cameraParas();
};