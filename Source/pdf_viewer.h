#ifndef PDF_VIEWER_H
#define PDF_VIEWER_H

#include <QWidget>
#include <QImage>
#include "fpdfview.h"

// PDFViewer �࣬������ʾ PDF �ļ�
class PDFViewer : public QWidget {
public:
    explicit PDFViewer(const QString& pdfFilePath, QWidget* parent = nullptr);
    ~PDFViewer() override;

    PDFViewer(const PDFViewer&) = delete;
    PDFViewer& operator=(const PDFViewer&) = delete;
    PDFViewer(PDFViewer&&) = delete;
    PDFViewer&& operator=(PDFViewer&&) = delete;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    FPDF_DOCUMENT m_pPDFDoc;
    QImage m_oPDFImage;
};

#endif // PDF_VIEWER_H
