﻿#include "paraTreeDelegate.h"
#include "paraEditor.h"
#include <QPainter>
#include <QDebug>
#include "paraTreeDelegateDraw.h"
const int cMargin = 10;
const int cMinH = 50;
const int cMinFontSize = 12;

CParaTreeDelegate::CParaTreeDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget *CParaTreeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        CParaEditor *editor = new CParaEditor(parent, nodeFromIndex(index)->getAttributes(), option.rect.width(), option.rect.height());
        connect(editor, SIGNAL(editingFinished()), this, SLOT(slotsCommitAndCloseEditor()));
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index); //第0列,则选择默认编辑器
}

QSize CParaTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(qMax(size.height(),cMinH));
    return size;
}

void CParaTreeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        CParaEditor *pEditor = dynamic_cast<CParaEditor *>(editor);
        pEditor->setValue(index.data());
        return;
    }
    QStyledItemDelegate::setEditorData(editor, index);
}

void CParaTreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        CParaEditor *pEditor = dynamic_cast<CParaEditor *>(editor);
        model->setData(index, pEditor->value(), Qt::EditRole);
        return;
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void CParaTreeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void CParaTreeDelegate::slotsCommitAndCloseEditor()
{
    CParaEditor *pEditor = dynamic_cast<CParaEditor *>(sender());
    emit commitData(pEditor);
    emit closeEditor(pEditor);
}

void CParaTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    IParaNode* node = nodeFromIndex(index);
    QStyleOptionViewItem styleOption = option;
    styleOption.font.setPointSize(qMax(option.font.pointSize(),cMinFontSize));
    int x = option.rect.x();
    int width = option.rect.width();
    int height = option.rect.height();
    if (index.column() == 0)
    {
        styleOption.rect = QRect(x + getIndentation(node) * cMargin, option.rect.y(), width - getIndentation(node) * cMargin, height);
    }
    CParaNodeAttributes paraNodeAttributes = node->getAttributes();
    drawBack(painter, styleOption, option.rect, paraNodeAttributes.type, index.column());
    drawText(painter, styleOption, paraNodeAttributes, index);
    drawGrid(painter, option, paraNodeAttributes.type);
}
