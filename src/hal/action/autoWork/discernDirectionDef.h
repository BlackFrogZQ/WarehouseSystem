#pragma once
#include "system/basic.h"
#include <QQueue>
enum CDiscernDirection : quint8
{
    cddUndef = 0,
    cddNegativeDirection = 0x11,
    cddPositiveDirection = 0x12,
    cddPoseError = 0x20
};

class CDirTest
{
    friend CDirTest *dirTest();

public:
    void init(int negativeTimes = 22, int positiveTimes = 22, int T = 50);
    void resetTypes();
    CDiscernDirection getTestType();

protected:
    CDirTest();
    ~CDirTest();

protected:
    int m_t;
    int m_negativeTimes;
    int m_positiveTimes;
    QQueue<CDiscernDirection> m_testTypes;
};

CDirTest *dirTest();