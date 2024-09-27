#ifndef PDFIUM_UTILS_H
#define PDFIUM_UTILS_H

#include <QImage>
#include "fpdfview.h"

// 初始化 PDFium
void initializePdFium();

// 将 PDFium 位图数据转换为 QImage
QImage pdfiumBitmapToQImage(FPDF_BITMAP bitmap);

// 渲染 PDF 页面到 QImage
QImage renderPdfPageToImage(FPDF_PAGE page);

#endif // PDFIUM_UTILS_H
