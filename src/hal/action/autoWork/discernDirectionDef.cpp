#include "discernDirectionDef.h"
#include <QRandomGenerator>
#include <QThread>
CDirTest::CDirTest():m_negativeTimes(22),m_positiveTimes(22),m_t(50)
{

}

CDirTest::~CDirTest()
{

}

void CDirTest::init(int negativeTimes, int positiveTimes, int T)
{
    assert(T > 0);
    assert(negativeTimes >= 0);
    assert(positiveTimes >= 0);
    assert((positiveTimes + positiveTimes) <= T);
    m_t = T;
    m_negativeTimes = negativeTimes;
    m_positiveTimes = positiveTimes;
}

void CDirTest::resetTypes()
{
    m_testTypes.clear();
    QVector<uint> seeds = QVector<uint>(m_t,0);
    QVector<CDiscernDirection> types;
    QRandomGenerator::global()->fillRange(seeds.data(),m_t);
    for (size_t i = 0; i < m_t; i++)
    {
        if (i<m_positiveTimes)
        {
            types.append(cddPositiveDirection);
        }
        else if (i<(m_positiveTimes+m_negativeTimes))
        {
            types.append(cddNegativeDirection);
        }
        else
        {
            types.append(cddPoseError);
        }
    }
    for (size_t i = 0; i < m_t; i++)
    {
        qSwap(types[seeds[i]%m_t],types[i]);
    }
    m_testTypes.append(types.toList());
}

CDiscernDirection CDirTest::getTestType()
{
    if(m_testTypes.isEmpty())
    {
        resetTypes();
    }
    QThread::msleep(200);
    return m_testTypes.dequeue();
}

CDirTest* dirTest()
{
    static CDirTest g_dirTest;
    return &g_dirTest;
}
