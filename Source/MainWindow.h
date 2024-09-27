/*!
 * @brief 
 *
 * 
 *
 * @author LiuYe
 * @date 2024-09-27
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */
#pragma once

#include <QTreeWidget>

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

