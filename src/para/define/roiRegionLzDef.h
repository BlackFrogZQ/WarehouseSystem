#pragma once

namespace TIGER_LzRoiDef
{
	struct CLzRoiPara
	{
		int x;
		int y;
		int w;
		int h;
		bool isShow;
		CLzRoiPara::CLzRoiPara()
		{
			x = 810;
			y = 800;
			w = 150;
			h = 250;
			isShow = true;
		};
	};
	CLzRoiPara *lzRoiParas();
};