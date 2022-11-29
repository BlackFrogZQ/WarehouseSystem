#pragma once

namespace TIGER_CameraDef
{
	struct CCameraPara
	{
		QString ip;
		int exposureTime;
		int heartbeatTime;
		CCameraPara::CCameraPara()
		{
			ip = "169.254.169.99";
			exposureTime = 35000;
			heartbeatTime = 5000;
		};
	};
	CCameraPara *cameraParas();
};