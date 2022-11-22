#pragma once
#include "system/basic.h"

const int typeTotalCount = 30;

enum CYcgType : unsigned char
{
    cytM678 = 1,
    cytM663,
    cytM653,
    cytM643,
    cytM881,
    cytM878,
    cytM873,
    cytM863,
    cytM853,
    cytM843,
    cytM1078,
    cytM1073,
    cytM1063,
    cytM1053,
    cytM1043,
    cytM1033,
    cytM1278,
    cytM1273,
    cytM1268,
    cytM1263,
    cytM1253,
    cytM1243,
    cytM1238,
    cytMax
};
const QStringList cYcgTypeName = {cnStr("M6*78"), cnStr("M6*63"), cnStr("M6*53"), cnStr("M6*43"), 
                                  cnStr("M8*81"), cnStr("M8*78"), cnStr("M8*73"), cnStr("M8*63"), cnStr("M8*53"), cnStr("M8*43"), 
                                  cnStr("M10*78"), cnStr("M10*73"), cnStr("M10*63"), cnStr("M10*53"), cnStr("M10*43"), cnStr("M10*33"), 
                                  cnStr("M12*78"), cnStr("M12*73"), cnStr("M12*68"), cnStr("M12*63"), cnStr("M12*53"), cnStr("M12*43"), cnStr("M12*38")};

enum CLzType : unsigned char
{
    cltM615 = 1,
    cltM630,
    cltM640,
    cltM650,
    cltM812,
    cltM815,
    cltM820,
    cltM830,
    cltM840,
    cltM850,
    cltM1015,
    cltM1020,
    cltM1030,
    cltM1040,
    cltM1050,
    cltM1060,
    cltM1215,
    cltM1220,
    cltM1225,
    cltM1230,
    cltM1240,
    cltM1250,
    cltM1255,
    cltMax
};
const QStringList cLzTypeName = {cnStr("M6*15"), cnStr("M6*30"), cnStr("M6*40"), cnStr("M6*50"), 
                                  cnStr("M8*12"), cnStr("M8*15"), cnStr("M8*20"), cnStr("M8*30"), cnStr("M8*40"), cnStr("M8*50"), 
                                  cnStr("M10*15"), cnStr("M10*20"), cnStr("M10*30"), cnStr("M10*40"), cnStr("M10*50"), cnStr("M10*60"), 
                                  cnStr("M12*15"), cnStr("M12*20"), cnStr("M12*25"), cnStr("M12*30"), cnStr("M12*40"), cnStr("M12*50"), cnStr("M12*55")};
