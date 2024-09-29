/*!
 * @brief 实现了一个用于管理 QTreeWidget 的类 TreeWidgetManager。
 *
 * 该文件定义并实现了一个名为 TreeWidgetManager 的类，主要用于初始化和管理一个 QTreeWidget 控件，
 * 包括设置 QTreeWidget 的样式、添加节点、处理列头点击事件等功能。此类能够将一个 QTreeWidget
 * 转换成一个可交互、可编辑的树形结构，便于显示和操作复杂的数据。
 *
 * 该文件的主要功能包括：
 * 1. 加载 QSS 样式表文件来定制 QTreeWidget 的外观。
 * 2. 初始化 QTreeWidget 的列标题并设置父子项，以及相关属性如背景色、字体等。
 * 3. 响应 QTreeWidget 的列头点击事件并显示对应的提示信息。
 *
 * @author LiuYe
 * @date 2024-09-27
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */


#include "TwoLayerSample.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QFont>
#include <QFile>

 /*!
  * @brief TreeWidgetManager 构造函数，用于初始化树形控件管理器。
  *
  * 构造函数接收一个指向 QTreeWidget 的指针，并将其保存到成员变量 m_pTreeWidget 中，以便后续对树形控件的操作和管理。
  *
  * @param treeWidget 指向需要被管理的 QTreeWidget 对象。
  * @param parent 父对象，默认为 nullptr。
  * @date 2024.09.29
  */
TreeWidgetManager::TreeWidgetManager(QTreeWidget* treeWidget, QObject* parent)
    : QObject(parent), m_pTreeWidget(treeWidget) {
}

/*!
 * @brief 初始化和设置 QTreeWidget 的各项属性、外观和内容。
 *
 * setupTreeWidget() 函数负责配置树形控件，包括设置样式、添加列标题、添加父子节点并设置可编辑性等。
 * 它还配置了各个父子项的字体、背景颜色，并设置列宽和整体控件的大小。
 *
 * @author LiuYe
 * @date 2024.09.29
 */
void TreeWidgetManager::setupTreeWidget() {

    QFile file(":/styles.qss"); // 假设文件放在资源文件中
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        m_pTreeWidget->setStyleSheet(styleSheet); // 设置样式表
    }
    else {
        qWarning("无法加载QSS文件");
    }

    // 设置列标题
    m_pTreeWidget->setColumnCount(6);
    const QStringList headers = { "Type", "Page", "Color", "Length", "Content", "Remark" };
    m_pTreeWidget->setHeaderLabels(headers);

    // 设置标题可点击并连接点击事件
    m_pTreeWidget->header()->setSectionsClickable(true);
    QObject::connect(m_pTreeWidget->header(), &QHeaderView::sectionClicked, this, &TreeWidgetManager::onHeaderClicked);

    // 设置整个树的字体大小
    QFont font = m_pTreeWidget->font();
    font.setPointSize(16); // 设置字体大小为16
    m_pTreeWidget->setFont(font);

    // 添加父节点 "Length" 并设置背景颜色
    QTreeWidgetItem* lengthItem = new QTreeWidgetItem(m_pTreeWidget);
    lengthItem->setText(0, "Length");
    lengthItem->setFont(0, font);  // 设置字体大小
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        lengthItem->setBackground(i, QBrush(Qt::gray));  // 将背景颜色设置为灰色
    }

    // 添加子节点并使其可编辑
    for (int i = 1; i <= 3; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(lengthItem);
        subItem->setText(0, QString("Length %1").arg(i));
        subItem->setText(1, QString::number(i)); // 设置页面号
        subItem->setText(4, QString("Editable content %1").arg(i)); // 设置内容
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // 使其可编辑
    }
    lengthItem->setExpanded(true);

    // 添加父节点 "Area" 并设置背景颜色
    QTreeWidgetItem* areaItem = new QTreeWidgetItem(m_pTreeWidget);
    areaItem->setText(0, "Area");
    areaItem->setFont(0, font);  // 设置字体大小
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        areaItem->setBackground(i, QBrush(Qt::gray));  // 设置灰色背景
    }

    // 添加子节点并使其可编辑
    for (int i = 1; i <= 2; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(areaItem);
        subItem->setText(0, QString("Area %1").arg(i));
        subItem->setText(1, QString::number(i + 3)); // 设置页面号
        subItem->setText(4, QString("Editable content %1").arg(i + 3)); // 设置内容
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // 使其可编辑
    }
    areaItem->setExpanded(true);

    // 添加父节点 "Text" 并设置背景颜色
    QTreeWidgetItem* textItem = new QTreeWidgetItem(m_pTreeWidget);
    textItem->setText(0, "Text");
    textItem->setFont(0, font);  // 设置字体大小
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        textItem->setBackground(i, QBrush(Qt::gray));  // 设置灰色背景
    }

    // 添加子节点并使其可编辑
    for (int i = 1; i <= 2; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(textItem);
        subItem->setText(0, QString("Text %1").arg(i));
        subItem->setText(1, QString::number(i + 5)); // 设置页面号
        subItem->setText(4, QString("Editable content %1").arg(i + 5)); // 设置内容
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // 使其可编辑
    }
    textItem->setExpanded(true);

    // 设置列宽度自适应
    m_pTreeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置控件大小策略以确保自适应
    m_pTreeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 设置控件的最小尺寸
    m_pTreeWidget->setMinimumSize(1800, 1200);
}

/*!
 * @brief 当点击树形控件的列标题时触发。
 *
 * 此槽函数将在用户点击 QTreeWidget 的列标题时被调用，显示相应列名的消息框。
 *
 * @param index 被点击的列索引。
 * @date 2024.09.29
 */
void TreeWidgetManager::onHeaderClicked(const int index) {
    QStringList headers = { "Type", "Page", "Color", "Length", "Content", "Remark" };
    QMessageBox::information(nullptr, "Header Clicked", headers[index] + " header was clicked");
}

