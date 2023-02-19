#pragma once

namespace TIGER_YcgRoiDef
{
	struct CYcgRoiPara
	{
		int x;
		int y;
		int w;
		int h;
		bool isShow;
		CYcgRoiPara::CYcgRoiPara()
		{
			x = 686;
			y = 390;
			w = 130;
			h = 510;
			isShow = true;
		};
	};
	CYcgRoiPara *ycgRoiParas();
};