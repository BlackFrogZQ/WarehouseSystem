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
const QStringList cYcgTypeName = {cnStr("M6L78"), cnStr("M6L63"), cnStr("M6L53"), cnStr("M6L43"), 
                                  cnStr("M8L81"), cnStr("M8L78"), cnStr("M8L73"), cnStr("M8L63"), cnStr("M8L53"), cnStr("M8L43"), 
                                  cnStr("M10L78"), cnStr("M10L73"), cnStr("M10L63"), cnStr("M10L53"), cnStr("M10L43"), cnStr("M10L33"), 
                                  cnStr("M12L78"), cnStr("M12L73"), cnStr("M12L68"), cnStr("M12L63"), cnStr("M12L53"), cnStr("M12L43"), cnStr("M12L38")};
const QStringList cAssembleTypeName = {cnStr("M6L78-M6L15"), cnStr("M6L63-M6L30"), cnStr("M6L53-M6L40"), cnStr("M6L43-M6L50"), 
                                  cnStr("M8L81-M8L12"), cnStr("M8L78-M8L15"), cnStr("M8L73-M8L20"), cnStr("M8L63-M8L30"), cnStr("M8L53-M8L40"), cnStr("M8L43-M8L50"), 
                                  cnStr("M10L78-M10L15"), cnStr("M10L73-M10L20"), cnStr("M10L63-M10L30"), cnStr("M10L53-M10L40"), cnStr("M10L43-M10L50"), cnStr("M10L33-M10L60"), 
                                  cnStr("M12L78-M12L15"), cnStr("M12L73-M12L20"), cnStr("M12L68-M12L25"), cnStr("M12L63-M12L30"), cnStr("M12L53-M12L40"), cnStr("M12L43-M12L50"), cnStr("M12L38-M12L55")};

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
const QStringList cLzTypeName = {cnStr("M6L15"), cnStr("M6L30"), cnStr("M6L40"), cnStr("M6L50"), 
                                  cnStr("M8L12"), cnStr("M8L15"), cnStr("M8L20"), cnStr("M8L30"), cnStr("M8L40"), cnStr("M8L50"), 
                                  cnStr("M10L15"), cnStr("M10L20"), cnStr("M10L30"), cnStr("M10L40"), cnStr("M10L50"), cnStr("M10L60"), 
                                  cnStr("M12L15"), cnStr("M12L20"), cnStr("M12L25"), cnStr("M12L30"), cnStr("M12L40"), cnStr("M12L50"), cnStr("M12L55")};
