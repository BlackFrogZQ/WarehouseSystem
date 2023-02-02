#pragma once
#include "hal/communication/serialPort/typeDef.h"

namespace TIGER_LzRoiDef
{
	struct CLzRoiPara
	{
		bool assemblyStates[cytMax];
		CLzRoiPara::CLzRoiPara()
		{
			for (size_t i = 0; i < cytMax-1; i++)
			{
				assemblyStates[i]=false;
			}
		};
	};
	CLzRoiPara *lzRoiParas();
};