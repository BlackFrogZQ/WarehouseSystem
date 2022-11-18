#include "plcSigDef.h"

CMasterData* masterData()
{
    static CMasterData gMasterData;
    return &gMasterData;
}

bool CMasterData::connected() const
{
    return isConnected;
}

bool CMasterData::colis(CPlcColis p_id) const
{
    return plcColis[p_id];
}

bool CMasterData::disColis(CPcDisColis p_id) const
{
    return pcDisColis[p_id];
}

quint16 CMasterData::hold(CPcHold p_id) const
{
    return pcHold[p_id];
}