#include "pdfium_utils.h"

// ��ʼ�� PDFium
void initializePdFium()
{
    FPDF_LIBRARY_CONFIG config;
    config.version = 3;
    config.m_pUserFontPaths = nullptr;
    config.m_pIsolate = nullptr;
    config.m_v8EmbedderSlot = 0;
    FPDF_InitLibraryWithConfig(&config);
}

// �� PDFium λͼ����ת��Ϊ QImage
QImage pdfiumBitmapToQImage(const FPDF_BITMAP bitmap)
{
    const int width = FPDFBitmap_GetWidth(bitmap);
    const int height = FPDFBitmap_GetHeight(bitmap);
    void* buffer = FPDFBitmap_GetBuffer(bitmap);

    const QImage image(static_cast<uchar*>(buffer), width, height, QImage::Format_RGB32);
    return image.rgbSwapped();
}

// ��Ⱦ PDF ҳ�浽 QImage
QImage renderPdfPageToImage(const FPDF_PAGE page)
{
    const int width = static_cast<int>(FPDF_GetPageWidth(page));
    const int height = static_cast<int>(FPDF_GetPageHeight(page));

    const FPDF_BITMAP bitmap = FPDFBitmap_Create(width, height, 1);
    FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF); // ��ɫ����
    FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, 0, FPDF_ANNOT);

    QImage image = pdfiumBitmapToQImage(bitmap);

    FPDFBitmap_Destroy(bitmap);

    return image;
}
