#ifndef SEARCHWIDGETNAVIGATIONHANDLER_H
#define SEARCHWIDGETNAVIGATIONHANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QList>
#include <QString>
#include <QObject>

class SearchWidgetNavigationHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT
public:
    SearchWidgetNavigationHandler(QObject *parent = 0);
    virtual ~SearchWidgetNavigationHandler();

    virtual void requestStarted(QWebEngineUrlRequestJob* request);

signals:
    void listTermClicked(int termIndex);
    void termLinkClicked(const QString& term);
};

#endif // SEARCHWIDGETNAVIGATIONHANDLER_H
