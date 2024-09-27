/*!
 * @brief 
 *
 * 
 *
 * @author LiuYe
 * @date 2024-09-27
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#include "MainWindow.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QFont>

TreeWidgetManager::TreeWidgetManager(QTreeWidget* treeWidget, QObject* parent)
    : QObject(parent), m_pTreeWidget(treeWidget) {
}

void TreeWidgetManager::setupTreeWidget() {
    // Set up headers
    m_pTreeWidget->setColumnCount(6);
    const QStringList headers = { "Type", "Page", "Color", "Length", "Content", "Remark" };
    m_pTreeWidget->setHeaderLabels(headers);

    // Make the header clickable
    m_pTreeWidget->header()->setSectionsClickable(true);
    QObject::connect(m_pTreeWidget->header(), &QHeaderView::sectionClicked, this, &TreeWidgetManager::onHeaderClicked);

    // Set font size for all items
    QFont font = m_pTreeWidget->font();
    font.setPointSize(16); // Set the font size to 20
    m_pTreeWidget->setFont(font);

    // Add parent item "Length" and set background color
    QTreeWidgetItem* lengthItem = new QTreeWidgetItem(m_pTreeWidget);
    lengthItem->setText(0, "Length");
    lengthItem->setFont(0, font);  // Set font size
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        lengthItem->setBackground(i, QBrush(Qt::gray));  // Set gray background for all columns
    }

    // Add child items and make them editable
    for (int i = 1; i <= 3; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(lengthItem);
        subItem->setText(0, QString("Length %1").arg(i));
        subItem->setText(1, QString::number(i)); // Page number
        subItem->setText(4, QString("Editable content %1").arg(i)); // Content
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // Make items editable
    }
    lengthItem->setExpanded(true);

    // Add parent item "Area"
    QTreeWidgetItem* areaItem = new QTreeWidgetItem(m_pTreeWidget);
    areaItem->setText(0, "Area");
    areaItem->setFont(0, font);  // Set font size
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        areaItem->setBackground(i, QBrush(Qt::gray));  // Set gray background for all columns
    }

    // Add child items and make them editable
    for (int i = 1; i <= 2; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(areaItem);
        subItem->setText(0, QString("Area %1").arg(i));
        subItem->setText(1, QString::number(i + 3)); // Page number
        subItem->setText(4, QString("Editable content %1").arg(i + 3)); // Content
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // Make items editable
    }
    areaItem->setExpanded(true);

    // Add parent item "Text"
    QTreeWidgetItem* textItem = new QTreeWidgetItem(m_pTreeWidget);
    textItem->setText(0, "Text");
    textItem->setFont(0, font);  // Set font size
    for (int i = 0; i < m_pTreeWidget->columnCount(); ++i) {
        textItem->setBackground(i, QBrush(Qt::gray));  // Set gray background for all columns
    }

    // Add child items and make them editable
    for (int i = 1; i <= 2; ++i) {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(textItem);
        subItem->setText(0, QString("Text %1").arg(i));
        subItem->setText(1, QString::number(i + 5)); // Page number
        subItem->setText(4, QString("Editable content %1").arg(i + 5)); // Content
        subItem->setFlags(subItem->flags() | Qt::ItemIsEditable); // Make items editable
    }
    textItem->setExpanded(true);

    // Set columns to stretch and fill the entire width
    m_pTreeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);

    // Set the tree widget to expand and fill the available space
    m_pTreeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Optionally set the widget to a minimum size if needed
    m_pTreeWidget->setMinimumSize(1800, 1200);

}

void TreeWidgetManager::onHeaderClicked(const int index) {
    QStringList headers = { "Type", "Page", "Color", "Length", "Content", "Remark" };
    QMessageBox::information(nullptr, "Header Clicked", headers[index] + " header was clicked");
}

