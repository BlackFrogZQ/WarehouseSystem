#pragma once
#include "system/basic.h"

enum CModbusType : uint8_t
{
    cmtCoils,
    cmtHoldingRegisters,
    cmtMax
};
const QStringList cModbusName = {"Coils", "HoldingRegisters"};
const QStringList cModbusCnName = {cnStr("线圈"), cnStr("保持寄存器")};

#define starAddress 0
#define valuesNumber 6

const QString cStyleSheet = "border:1px groove gray;\nborder-radius:5px;\npadding:2px 3px;\nbackground:transparent;\nbackground-color: rgb(255, 255, 255,150);";
const QString cWindowStyleSheet = "#widgetAllImage{border:1px groove gray;\nborder-radius:5px;\npadding:2px 3px;\nbackground:transparent;\nbackground-color: rgb(255, 255, 255, 150);}";
const QString cYcgExposureTimeName = cnStr("延长杆曝光时间%1ms");
const QString cLzExposureTimeName = cnStr("螺柱曝光时间%1ms");
const QString cStartAcquireName = cnStr("开始录制");
const QString cStopAcquireName = cnStr("停止录制");
const QString cConnectModbusName = cnStr("连接");
const QString cDisconnectModbusName = cnStr("断开");
const QString cStartRun = cnStr("开始运行");
const QString cStopRun = cnStr("停止运行");