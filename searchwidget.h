#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <memory>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QWebEngineSettings>
#include <QWidget>
#include <QScreen>
#include <sstream>

#include "searchengine.h"
#include "searchwidgeturlschemehandler.h"
#include "overlaywindow.h"
#include "ocrengine.h"

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    virtual ~SearchWidget();

    QString listEntryToHtml (int entryNum);

public slots:
    void search();
    void setEntries(const QList<Entry*>& entries);
    void selectListItem(int index);

private:
    Ui::SearchWidget *m_ui;
    SearchEngine m_engine;
    QList<Entry*> m_entries;
    SearchWidgetNavigationHandler termlistUrlHandler;

    OCREngine m_ocrEngine;
    QList<OverlayWindow*> m_screenCaptureWindows;

    QColor freqToColor (Scale<int> freq);
    QString entriesToHtml (const QList<Entry*>& entries);

private slots:
    void activateScreenCapture();
    void closeScreenCaptureWindows();
};

#endif // SEARCHWIDGET_H
