#include "setParaWindow.h"
#include "paraTree/paraModel.h"
#include "paraTree/paraTreeDelegate.h"
#include "ui/basic.h"
#include <QTreeView>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>

CSetParaWindow::CSetParaWindow(QWidget *parent)
    : QDialog(parent),
      m_pTreeModel(nullptr)
{
    setWindowTitle(cnStr("设置参数"));
    setMinimumSize(725 * CUiBasic::getScale(), 544 * CUiBasic::getScale());
    setPalette(QPalette(Qt::darkGray));
    setAutoFillBackground(true);
    m_treeView = new QTreeView;
    m_treeView->setIndentation(0);
    m_treeView->setWordWrap(true);
    m_pTreeModel = new CParaTreeModel(m_treeView);
    m_treeView->setItemDelegate(new CParaTreeDelegate);
    m_treeView->setModel(m_pTreeModel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_treeView);
    this->setLayout(hLayout);
	this->setShowNode(paraRootNode());
    m_treeView->expandAll();
    connect(m_treeView->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](const int &p_value)
            { m_treeView->viewport()->update(); });
}

CSetParaWindow ::~CSetParaWindow()
{

}

void CSetParaWindow::setShowNode(CParaNode *p_node)
{
    auto copySingleVisibleNode = [](CParaNode *node)
    {
        CParaNode *visibleNode = nullptr;
        if (!node->m_attributes.visible)
        {
            return visibleNode;
        }
        visibleNode = new CParaNode({pntSum, cFirstRootNode});
        visibleNode->m_attributes = node->m_attributes;
        if (visibleNode->m_attributes.type != pntRoot)
        {
            visibleNode->setValuePtr(node->getValuePtr());
        }
        return visibleNode;
    };

    std::function<void(CParaNode *, CParaNode *)> copyVisibleNode;
    copyVisibleNode = [&](CParaNode *p_root, CParaNode *p_node2)
    {
        for (size_t i = 0; i < p_node2->m_childrenNode.size(); i++)
        {
            CParaNode *children = copySingleVisibleNode(p_node2->m_childrenNode[i]);
            if (children != nullptr)
            {
                copyVisibleNode(children, p_node2->m_childrenNode[i]);
                if (children->m_attributes.type != pntRoot || children->m_childrenNode.size() > 0)
                {
                    p_root->appendChildren(children);
                }
                else
                {
                    delete children;
                    children = nullptr;
                }
            }
        }
    };

    CParaNode *node = new CParaNode({pntRoot, cFirstRootNode});
    copyVisibleNode(node, p_node);

    m_pTreeModel->sepntRootNode(node);
    m_treeView->expandAll();
}
