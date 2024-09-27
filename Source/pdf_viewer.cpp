#include "pdf_viewer.h"
#include "pdfium_utils.h"
#include <QPainter>
#include <QFileDialog>
#include <iostream>

PDFViewer::PDFViewer(const QString& pdfFilePath, QWidget* parent)
    : QWidget(parent), m_pPDFDoc(nullptr)
{
    const QByteArray filePathBytes = pdfFilePath.toLocal8Bit();
    m_pPDFDoc = FPDF_LoadDocument(filePathBytes.constData(), nullptr);
    if (!m_pPDFDoc)
    {
        std::cerr << "Failed to open PDF file: " << pdfFilePath.toStdString() << '\n';
    }

    if (m_pPDFDoc)
    {
        const FPDF_PAGE page = FPDF_LoadPage(m_pPDFDoc, 0); // 加载第一页
        if (page)
        {
            m_oPDFImage = renderPdfPageToImage(page); // 渲染第一页
            FPDF_ClosePage(page);
        }
    }

    setFixedSize(m_oPDFImage.size());
}

PDFViewer::~PDFViewer()
{
    if (m_pPDFDoc)
    {
        FPDF_CloseDocument(m_pPDFDoc);
    }
    FPDF_DestroyLibrary();
}

void PDFViewer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (!m_oPDFImage.isNull())
    {
        painter.drawImage(0, 0, m_oPDFImage);
    }
}
