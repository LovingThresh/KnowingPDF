#ifndef PDFIUM_UTILS_H
#define PDFIUM_UTILS_H

#include <QImage>
#include "fpdfview.h"

// ��ʼ�� PDFium
void initializePdFium();

// �� PDFium λͼ����ת��Ϊ QImage
QImage pdfiumBitmapToQImage(FPDF_BITMAP bitmap);

// ��Ⱦ PDF ҳ�浽 QImage
QImage renderPdfPageToImage(FPDF_PAGE page);

#endif // PDFIUM_UTILS_H
