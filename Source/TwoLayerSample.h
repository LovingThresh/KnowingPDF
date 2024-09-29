/*!
 * @brief 提供树形控件管理的头文件。
 *
 * 本文件定义了 `TreeWidgetManager` 类，用于管理 `QTreeWidget` 控件的初始化和事件处理。
 * `TreeWidgetManager` 类负责设置 `QTreeWidget` 的外观、列标题、节点数据以及处理列头点击事件等功能，
 * 使 `QTreeWidget` 能够以自定义的方式呈现和交互。
 *
 * @author LiuYe
 * @date 2024-09-27
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#pragma once

#include <QTreeWidget>

/*!
 * @brief 管理 QTreeWidget 控件的类，提供初始化和事件处理功能。
 *
 * `TreeWidgetManager` 类继承自 `QObject`，用于设置 `QTreeWidget` 的结构、样式、数据，以及处理列头点击事件。
 * 通过调用 `setupTreeWidget()` 方法，可以将指定的 `QTreeWidget` 初始化为一个包含多层节点、可编辑的树状结构。
 *
 * @param treeWidget 需要被管理的 `QTreeWidget` 对象指针
 * @param parent 父对象，默认为 nullptr
 * @date 2024.09.29
 */
class TreeWidgetManager : public QObject {
    Q_OBJECT

public:
    explicit TreeWidgetManager(QTreeWidget* treeWidget, QObject* parent = nullptr);
    void setupTreeWidget(); // Function to set up the tree structure

private slots:
    static void onHeaderClicked(int index); // Slot for handling header click

private:
    QTreeWidget* m_pTreeWidget; // Pointer to QTreeWidget
};

