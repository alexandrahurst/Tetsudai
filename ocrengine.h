#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <tesseract/baseapi.h>
#include <QImage>
#include <QRect>
#include <QObject>

class OCREngine : public QObject {
    Q_OBJECT
public:
    OCREngine();
    virtual ~OCREngine();

public slots:
    void startRecognition(QImage image, QRect selection);

signals:
    void recognitionFinished(QString recognizedText);

private:
    tesseract::TessBaseAPI *api;
};

#endif // OCRENGINE_H
