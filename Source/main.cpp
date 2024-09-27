#include <QApplication>
#include <QFileDialog>

// #include "MainWindow.h"
#include "pdf_viewer.h"
#include "pdfium_utils.h"

// int main(int argc, char* argv[]) {
//     QApplication app(argc, argv);
//
//     // 初始化 PDFium
//     initializePdFium();
//
//     // 选择 PDF 文件
//     const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open PDF File", "", "PDF Files (*.pdf)");
//
//     if (filePath.isEmpty()) {
//         return -1;
//     }
//
//     // 创建 PDF 查看器窗口
//     PDFViewer viewer(filePath);
//     viewer.show();
//
//     // 进入 Qt 主事件循环
//     return QApplication::exec();
// }

// int main(int argc, char* argv[]) {
//     QApplication app(argc, argv);
//
//     MainWindow window;
//     window.show();
//
//     return QApplication::exec();
// }

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create QTreeWidget
    QTreeWidget treeWidget;
    treeWidget.setWindowTitle("QTreeWidget Example");

    // Create and set up TreeWidgetManager
    TreeWidgetManager treeWidgetManager(&treeWidget);
    treeWidgetManager.setupTreeWidget();

    treeWidget.show();

    return QApplication::exec();
}
