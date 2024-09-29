/*!
 * @brief 实现了一个具有双层布局和可交互性控件的窗口。
 *
 * 本文件定义并实现了三个主要类：`CBlueLayer`、`CGreenLayer` 和 `CAMainWindow`，分别代表了应用程序中的蓝色图层、
 * 可调整高度的绿色图层以及主窗口。通过对这些类的构建，实现了一个界面上可以覆盖和控制显示的双层布局，
 * 并通过工具栏按钮与绿色区域实现同步控制。
 *
 * 主要功能包括：
 * - 创建两个图层（蓝色和绿色），并实现图层的覆盖和可见性切换。
 * - 通过工具栏上的按钮，实现对绿色区域的显隐控制。
 * - 通过鼠标事件实现分割条的拖动，来调整绿色区域的高度。
 *
 * @author LiuYe
 * @date 2024-09-27
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#include "CustomTreeWidget.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QToolButton>
#include <QApplication>

#include <QDebug>

 // CBlueLayer 构造函数
 /*!
  * @brief 构造函数，初始化蓝色图层并添加工具栏。
  *
  * 初始化蓝色图层 `CBlueLayer`，并在其中添加一个垂直方向的 `QToolBar`，用于添加控制绿色区域的按钮。
  *
  * @param pParent 父窗口对象
  */
CBlueLayer::CBlueLayer(QWidget* pParent)
    : QWidget(pParent), m_pToolBar(new QToolBar("Control ToolBar", this))
{
    setStyleSheet("background-color: blue;");
    m_pToolBar->setOrientation(Qt::Vertical); // 设置工具栏垂直方向
    m_pToolBar->setGeometry(0, 0, 30, height()); // 工具栏初始大小
}

/*!
 * @brief 重写 paintEvent，绘制一个白色矩形。
 *
 * 在蓝色区域中绘制一个白色矩形，位置和大小根据窗口尺寸调整。
 *
 * @param pEvent 指向绘制事件的指针
 */
void CBlueLayer::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white); // 使用白色画刷

    // 在蓝色区域内绘制一个白色矩形
    const QRect rect(width() / 4, height() / 4, width() / 2, height() / 2);
    painter.drawRect(rect);
}

/*!
 * @brief 构造函数，初始化绿色图层。
 *
 * 初始化绿色图层 `CGreenLayer`，并将其设置为初始隐藏状态。
 *
 * @param pParent 父窗口对象
 */
CGreenLayer::CGreenLayer(QWidget* pParent)
    : QWidget(pParent), m_bDragging(false), m_nInitialHeight(0)
{
    setStyleSheet("background-color: green;");
    setFixedHeight(0); // 初始状态为隐藏
}

/*!
 * @brief 处理鼠标按下事件，开始拖动绿色区域。
 *
 * 当鼠标左键按下时，记录开始拖动的位置和当前绿色区域的高度。
 *
 * @param pEvent 指向鼠标事件的指针
 */
void CGreenLayer::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bDragging = true;
        m_oDragStartPosition = pEvent->globalPos();
        m_nInitialHeight = height();
    }
}

/*!
 * @brief 处理鼠标移动事件，调整绿色区域的高度。
 *
 * 根据鼠标移动的距离，动态调整绿色区域的高度，实现拖动效果。
 *
 * @param pEvent 指向鼠标事件的指针
 */
void CGreenLayer::mouseMoveEvent(QMouseEvent* pEvent)
{
    if (m_bDragging)
    {
        const int dy = pEvent->globalPos().y() - m_oDragStartPosition.y();
        const int newHeight = std::max(0, m_nInitialHeight - dy); // 保证高度不小于 0
        setFixedHeight(newHeight);
        update();
    }
}

/*!
 * @brief 处理鼠标释放事件，结束拖动。
 *
 * 当鼠标左键释放时，结束对绿色区域的拖动操作。
 *
 * @param pEvent 指向鼠标事件的指针
 */
void CGreenLayer::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bDragging = false;
    }
}

/*!
 * @brief 主窗口的构造函数，初始化蓝色和绿色图层以及分割条。
 *
 * 初始化主窗口 `CAMainWindow`，创建并设置蓝色和绿色图层、分割条，以及工具栏中的控制按钮。
 *
 * @param pParent 父窗口对象
 */
CAMainWindow::CAMainWindow(QWidget* pParent)
    : QWidget(pParent), m_bDragging(false), m_nInitialHeight(0)
{
    setStyleSheet("background-color: blue;"); // 设置整个窗口背景为蓝色

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->setSpacing(0);

    // 初始化蓝色图层
    m_pBlueLayer = new CBlueLayer(this);
    m_pBlueLayer->setGeometry(0, 0, this->width(), this->height());
    m_pBlueLayer->show();

    // 初始化绿色图层
    m_pGreenLayer = new CGreenLayer(this);
    m_pGreenLayer->setGeometry(30, this->height(), this->width() - 30, 0);
    m_pGreenLayer->raise();

    // 创建分割条
    m_pDragBar = new QFrame(this);
    m_pDragBar->setStyleSheet("background-color: grey;");
    m_pDragBar->setCursor(Qt::SizeVerCursor);
    m_pDragBar->setGeometry(30, this->height() - 5, this->width(), 5);
    m_pDragBar->installEventFilter(this);

    // 在工具栏添加 Action
    m_pToggleAction = m_pBlueLayer->toolBar()->addAction("A");
    connect(m_pToggleAction, &QAction::triggered, this, [this]()
        {
            QWidget* pWidget = m_pBlueLayer->toolBar()->widgetForAction(m_pToggleAction);
            if (QToolButton* pButton = qobject_cast<QToolButton*>(pWidget))
            {
                pButton->setChecked(!pButton->isChecked()); // 切换按钮状态
            }
        });

    // 获取关联的 QToolButton
    QWidget* pWidget = m_pBlueLayer->toolBar()->widgetForAction(m_pToggleAction);
    if (QToolButton* pButton = qobject_cast<QToolButton*>(pWidget))
    {
        pButton->setCheckable(true);
        pButton->setStyleSheet("QToolButton { "
            "background-color: lightgray; "
            "border: 1px solid black; "
            "color: blue; "
            "padding: 5px; "
            "border-radius: 4px; }"
            "QToolButton:hover { "
            "background-color: lightblue; }"
            "QToolButton:checked { "
            "background-color: green; "
            "color: white; }");

        connect(pButton, &QToolButton::toggled, this, &CAMainWindow::toggleGreenLayer);
    }
}

/*!
 * @brief 切换绿色区域的显示状态。
 *
 * 根据按钮的选中状态，显示或隐藏绿色区域，并调整分割条的位置。
 *
 * @param checked 按钮的选中状态
 */
void CAMainWindow::toggleGreenLayer(const bool checked) const
{
    if (checked)
    {
        m_pGreenLayer->setFixedHeight(100);
        m_pGreenLayer->setGeometry(30, m_pBlueLayer->height() - 100, this->width() - 30, 100);
        m_pDragBar->move(30, m_pGreenLayer->y() - m_pDragBar->height());
    }
    else
    {
        m_pGreenLayer->setFixedHeight(0);
        m_pDragBar->move(30, this->height() - m_pDragBar->height());
    }

    updateToggleAction();
}

/*!
 * @brief 更新工具按钮的状态和样式。
 *
 * 根据绿色区域的高度，更新工具按钮的选中状态和样式。
 */
void CAMainWindow::updateToggleAction() const
{
    QWidget* pWidget = m_pBlueLayer->toolBar()->widgetForAction(m_pToggleAction);
    if (QToolButton* pButton = qobject_cast<QToolButton*>(pWidget))
    {
        if (m_pGreenLayer->height() > 0)
        {
            pButton->setChecked(true);
            pButton->setStyleSheet("QToolButton { "
                "background-color: red; "
                "color: white; "
                "padding: 5px; "
                "border: 1px solid black; "
                "border-radius: 4px; }"
                "QToolButton:hover { "
                "background-color: lightblue; }");
        }
        else
        {
            pButton->setChecked(false);
            pButton->setStyleSheet("QToolButton { "
                "background-color: lightgray; "
                "color: blue; "
                "padding: 5px; "
                "border: 1px solid black; "
                "border-radius: 4px; }"
                "QToolButton:hover { "
                "background-color: lightblue; }");
        }
    }
}

/*!
 * @brief 处理窗口大小变化事件，调整控件布局。
 *
 * 当窗口大小发生变化时，重新设置蓝色图层、绿色图层和分割条的大小和位置。
 *
 * @param pEvent 指向大小变化事件的指针
 */
void CAMainWindow::resizeEvent(QResizeEvent* pEvent)
{
    m_pBlueLayer->setGeometry(0, 0, this->width(), this->height());
    m_pDragBar->setGeometry(30, m_pBlueLayer->height() - m_pGreenLayer->height() - m_pDragBar->height(),
        this->width() - 30, 5);
    m_pGreenLayer->setGeometry(30, m_pBlueLayer->height() - m_pGreenLayer->height(), this->width() - 30,
        m_pGreenLayer->height());
    QWidget::resizeEvent(pEvent);
}

/*!
 * @brief 处理事件过滤器，用于实现分割条的拖动功能。
 *
 * 根据鼠标事件，检测并处理分割条的拖动，以调整绿色区域的高度。
 *
 * @param pObj 事件发生的对象
 * @param pEvent 事件对象指针
 * @return 如果事件被处理返回 true，否则返回 false
 */
bool CAMainWindow::eventFilter(QObject* pObj, QEvent* pEvent)
{
    if (pObj == m_pDragBar)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            const QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
            if (pMouseEvent->button() == Qt::LeftButton)
            {
                m_bDragging = true;
                m_oDragStartPosition = pMouseEvent->globalPos();
                m_nInitialHeight = m_pGreenLayer->height();
                return true;
            }
        }
        if (pEvent->type() == QEvent::MouseMove)
        {
            const QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
            if (m_bDragging)
            {
                const int dy = m_oDragStartPosition.y() - pMouseEvent->globalPos().y();
                const int newHeight = std::max(0, m_nInitialHeight + dy);
                m_pGreenLayer->setFixedHeight(newHeight);
                m_pGreenLayer->setGeometry(30, m_pBlueLayer->height() - newHeight, this->width() - 30, newHeight);
                m_pDragBar->move(30, m_pGreenLayer->y() - m_pDragBar->height());
                updateToggleAction(); // 更新按钮状态以同步绿色区域高度
                return true;
            }
        }
        if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            const QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
            if (pMouseEvent->button() == Qt::LeftButton)
            {
                m_bDragging = false;
                return true;
            }
        }
    }
    return QWidget::eventFilter(pObj, pEvent);
}

