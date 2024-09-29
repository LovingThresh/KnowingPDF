/*!
 * @brief 
 *
 * 
 *
 * @author LiuYe
 * @date 2024-09-29
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#pragma once

#include <QToolBar>
#include <QAction>
#include <QFrame>
#include <QPoint>

class QVBoxLayout;

class CBlueLayer : public QWidget
{
    Q_OBJECT

public:
    explicit CBlueLayer(QWidget* pParent = nullptr);

    QToolBar* toolBar() const
    {
        return m_pToolBar;
    } // 提供获取工具栏的接口

protected:
    void paintEvent(QPaintEvent* pEvent) override;

private:
    QToolBar* m_pToolBar; // 工具栏
};

class CGreenLayer : public QWidget
{
public:
    explicit CGreenLayer(QWidget* pParent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* pEvent) override;
    void mouseMoveEvent(QMouseEvent* pEvent) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;

private:
    bool m_bDragging;
    QPoint m_oDragStartPosition;
    int m_nInitialHeight;
};

class CAMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CAMainWindow(QWidget* pParent = nullptr);

protected:
    void resizeEvent(QResizeEvent* pEvent) override;
    bool eventFilter(QObject* pObj, QEvent* pEvent) override;

private slots:
    void toggleGreenLayer(const bool checked) const;

private:
    CBlueLayer* m_pBlueLayer;
    CGreenLayer* m_pGreenLayer;
    QFrame* m_pDragBar;
    QAction* m_pToggleAction; // 用于控制绿色区域的Action
    bool m_bDragging;
    QPoint m_oDragStartPosition;
    int m_nInitialHeight;

    void updateToggleAction() const; // 更新Action状态
};
