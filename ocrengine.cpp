#include "ocrengine.h"
#include <QRegExp>

OCREngine::OCREngine() : QObject(),
    api(new tesseract::TessBaseAPI())
{
    api->Init("/usr/share/tesseract-ocr/4.00/tessdata/", "jpn");
}

OCREngine::~OCREngine() {
    delete api;
}

void OCREngine::startRecognition(QImage image, QRect selection) {
    char *recognizedText(api->TesseractRect(image.constBits(),
                                            image.pixelFormat().bitsPerPixel() / 8,
                                            image.bytesPerLine(),
                                            selection.left(),
                                            selection.top(),
                                            selection.width(),
                                            selection.height()));

    if (recognizedText == nullptr) {
        emit recognitionFinished(QString(""));
    } else {
        emit recognitionFinished(QString(recognizedText).remove(QRegExp("\\s+")));
        delete[] recognizedText;
    }
}

