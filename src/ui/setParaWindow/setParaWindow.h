﻿#pragma once
#include "para\define\paraDef.h"
#include "paraTree/paraModel.h"
#include <QDialog>
using namespace TIGER_ParaDef;
class QTreeView;
class CSetParaWindow : public QDialog
{
    Q_OBJECT
public:
    CSetParaWindow(QWidget *parent = nullptr);
    ~CSetParaWindow();
    void setShowNode(CParaNode* p_node);

protected:
    QTreeView *m_treeView;
    CParaTreeModel *m_pTreeModel;
};