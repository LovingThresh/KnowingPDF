/*!
 * @brief 定义了双层控件布局的头文件，提供蓝色图层、绿色图层以及主窗口的声明。
 *
 * 本文件包含了 `CBlueLayer`、`CGreenLayer` 和 `CAMainWindow` 类的声明，负责实现一个具有工具栏控制和
 * 可调整高度功能的双层界面。蓝色图层包含工具栏，可以控制绿色图层的显示状态；绿色图层通过鼠标事件
 * 进行高度调整；主窗口 `CAMainWindow` 负责将所有组件整合在一起并处理信号与槽的连接。
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

/*!
 * @brief 蓝色图层类，负责绘制并提供工具栏用于控制绿色区域。
 *
 * `CBlueLayer` 类继承自 `QWidget`，提供一个工具栏，并在其区域内绘制了一个白色矩形。
 * 工具栏中包含一个用于控制绿色区域的按钮，提供了与主窗口交互的接口。
 *
 * @param pParent 父窗口对象，默认为 nullptr
 * @date 2024.09.29
 */
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

/*!
 * @brief 绿色图层类，提供拖动调整高度的功能。
 *
 * `CGreenLayer` 类继承自 `QWidget`，能够通过鼠标事件调整自身的高度，实现绿色区域的可伸缩性。
 *
 * @param pParent 父窗口对象，默认为 nullptr
 * @date 2024.09.29
 */
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

/*!
 * @brief 主窗口类，管理蓝色和绿色图层，并提供控制和交互功能。
 *
 * `CAMainWindow` 类继承自 `QWidget`，负责将 `CBlueLayer` 和 `CGreenLayer` 组合在一起，并通过工具栏按钮
 * 和鼠标事件，实现对绿色区域的显示、隐藏和高度调整等功能。
 *
 * @param pParent 父窗口对象，默认为 nullptr
 * @date 2024.09.29
 */
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
