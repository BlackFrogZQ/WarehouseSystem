﻿#pragma once
#include "para/define/paraNode.h"
#include <QAbstractItemModel>
using namespace TIGER_ParaDef;

//默认显示服务器根节点
//sepntRootNode 设置显示根节点，本节点不显示
class CParaTreeModel : public QAbstractItemModel
{
public:
        CParaTreeModel(QObject *parent = 0);
        ~CParaTreeModel();
        void sepntRootNode(IParaNode *node);
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QModelIndex index(int row, int column, const QModelIndex &parent) const;
        QModelIndex parent(const QModelIndex &child) const;
        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
        IParaNode *nodeFromIndex(const QModelIndex &index) const;
        IParaNode *m_rootNode;
};