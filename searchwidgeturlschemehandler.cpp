#include "searchwidgeturlschemehandler.h"

#include <iostream>

SearchWidgetNavigationHandler::SearchWidgetNavigationHandler(
        QObject *parent) :
    QWebEngineUrlSchemeHandler(parent) {

}

SearchWidgetNavigationHandler::~SearchWidgetNavigationHandler() { }

void SearchWidgetNavigationHandler::requestStarted(
        QWebEngineUrlRequestJob* request) {
    QList<QString> parsedRequest = request->requestUrl().path().split("/");
    if (parsedRequest[0].compare("item") == 0) {
        emit listTermClicked(parsedRequest[1].toInt());
    } else if (parsedRequest[0].compare("term") == 0) {
        emit termLinkClicked(parsedRequest[1]);
    }
    std::cout << request->requestUrl().path().toStdString() << std::endl;
}
