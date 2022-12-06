#pragma once
#include <QDialog>
class QTableWidget;
class QPushButton;

class CAssemblyType:public QDialog
{
    Q_OBJECT
public:
    CAssemblyType(QWidget *parent = nullptr);
    ~CAssemblyType();

public slots:
    void setCurrentItem(const QModelIndex  &p_index);
    bool getCurrentItem(int &p_item);

protected:
    void initLayout();

protected:
    QTableWidget *m_pChoiceType;
    QPushButton *m_pSave;
    int m_item;
    bool m_isSave;
};
