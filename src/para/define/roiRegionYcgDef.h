#pragma once
#include "hal/communication/serialPort/typeDef.h"

namespace TIGER_YcgRoiDef
{
	struct CYcgRoiPara
	{
		bool ycgStates[cytMax];
		CYcgRoiPara::CYcgRoiPara()
		{
			for (size_t i = 0; i < cytMax-1; i++)
			{
				ycgStates[i]=false;
			}
		};
	};
	CYcgRoiPara *ycgRoiParas();
};