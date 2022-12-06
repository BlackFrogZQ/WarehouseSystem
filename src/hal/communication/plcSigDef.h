#pragma once
#include "system/basic.h"
enum CPlcColis : unsigned char
{
    cpcReset = 0,
    cpcYCGVision,
    cpcStratYCGAction,
    cpcLZVision,
    cpcStratLZAction,
    cpcAuotRun,
    cpcAutoStop,
    cpcTemp1,
    cpcTemp2,
    cpcTemp3,
    cpcMax
};

enum CPcDisColis : unsigned char
{
    cpdcReset = 0,
    cpdcYCGVision,
    cpdcYCGAction,
    cpdcLZVision,
    cpdcLZAction,
    cpdcAuotRun,
    cpdcAutoStop,
    cpdcTemp1,
    cpdcTemp2,
    cpdcTemp3,
    cpdcMax
};

enum CPcHold : unsigned char
{
    cphRunType = 0,
    cphYCGType,
    cphLZType,
    cphTwist,
    cphAllCount,
    cphOkCount,
    cphTemp3,
    cphTemp4,
    cphTemp5,
    cphTemp6,
    cphMax
};

struct CMasterData
{
    friend class CModbusMaster;

public:
    bool connected() const;
    bool colis(CPlcColis p_id) const;
    bool disColis(CPcDisColis p_id) const;
    quint16 hold(CPcHold p_id) const;

protected:
    bool isConnected = false;
    bool plcColis[cpcMax] = {0};
    bool pcDisColis[cpdcMax] = {0};
    quint16 pcHold[cphMax] = {0};
};

CMasterData *masterData();
