#include "AMainWindows.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QToolButton>
#include <QApplication>

#include <QDebug>

// CBlueLayer 构造函数
CBlueLayer::CBlueLayer(QWidget* pParent)
    : QWidget(pParent), m_pToolBar(new QToolBar("Control ToolBar", this))
{
    setStyleSheet("background-color: blue;");
    m_pToolBar->setOrientation(Qt::Vertical); // 设置工具栏垂直方向
    m_pToolBar->setGeometry(0, 0, 30, height()); // 工具栏初始大小
}

// CBlueLayer::paintEvent 实现
void CBlueLayer::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white); // 修改为绿色

    // 在蓝色区域内绘制一个绿色矩形
    const QRect rect(width() / 4, height() / 4, width() / 2, height() / 2);
    painter.drawRect(rect);
}


// CGreenLayer 构造函数
CGreenLayer::CGreenLayer(QWidget* pParent)
    : QWidget(pParent), m_bDragging(false), m_nInitialHeight(0)
{
    setStyleSheet("background-color: green;");
    setFixedHeight(0); // 初始状态为隐藏
}

// CGreenLayer::mousePressEvent 实现
void CGreenLayer::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bDragging = true;
        m_oDragStartPosition = pEvent->globalPos();
        m_nInitialHeight = height();
    }
}

// CGreenLayer::mouseMoveEvent 实现
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

// CGreenLayer::mouseReleaseEvent 实现
void CGreenLayer::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bDragging = false;
    }
}

// CAMainWindow::CAMainWindow 构造函数
CAMainWindow::CAMainWindow(QWidget* pParent)
    : QWidget(pParent), m_bDragging(false), m_nInitialHeight(0)
{
    setStyleSheet("background-color: blue;"); // 将整个窗口背景改为蓝色

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->setSpacing(0);

    // 创建蓝色图层
    m_pBlueLayer = new CBlueLayer(this);
    m_pBlueLayer->setGeometry(0, 0, this->width(), this->height());
    m_pBlueLayer->show();

    // 创建绿色图层
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
        // 设置按钮的 checkable 状态
        pButton->setCheckable(true);

        // 设置按钮样式
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

        // 连接按钮的 toggled 信号到 toggleGreenLayer 函数
        connect(pButton, &QToolButton::toggled, this, &CAMainWindow::toggleGreenLayer);
    }
}

void CAMainWindow::toggleGreenLayer(const bool checked) const
{
    if (checked) // 根据按钮的 isChecked 状态来处理
    {
        // 显示并设置绿色区域高度为100
        m_pGreenLayer->setFixedHeight(100);
        m_pGreenLayer->setGeometry(30, m_pBlueLayer->height() - 100, this->width() - 30, 100);

        // 将分割条移动到绿色区域的顶部
        m_pDragBar->move(30, m_pGreenLayer->y() - m_pDragBar->height());
    }
    else
    {
        // 隐藏绿色区域，将分割条移动到窗口底部
        m_pGreenLayer->setFixedHeight(0);
        m_pDragBar->move(30, this->height() - m_pDragBar->height());
    }

    updateToggleAction();
}

void CAMainWindow::updateToggleAction() const
{
    QWidget* pWidget = m_pBlueLayer->toolBar()->widgetForAction(m_pToggleAction);
    if (QToolButton* pButton = qobject_cast<QToolButton*>(pWidget))
    {
        if (m_pGreenLayer->height() > 0)
        {
            // 设置为选中状态，按钮背景变成红色
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
            // 恢复默认状态，按钮背景恢复为灰色
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


void CAMainWindow::resizeEvent(QResizeEvent* pEvent)
{
    m_pBlueLayer->setGeometry(0, 0, this->width(), this->height());

    m_pDragBar->setGeometry(30, m_pBlueLayer->height() - m_pGreenLayer->height() - m_pDragBar->height(),
                            this->width() - 30, 5);

    m_pGreenLayer->setGeometry(30, m_pBlueLayer->height() - m_pGreenLayer->height(), this->width() - 30,
                               m_pGreenLayer->height());

    QWidget::resizeEvent(pEvent);
}

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

                // 更新按钮状态以同步绿色区域高度
                updateToggleAction();

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
