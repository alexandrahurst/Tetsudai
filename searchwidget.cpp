#include "searchwidget.h"
#include "ui_searchwidget.h"

#include <iostream>
#include <QStyleFactory>
#include <cmath>
#include <QWebEngineProfile>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent),
    m_ui(new Ui::SearchWidget),
    m_engine(),
    m_entries(),
    //termlistUrlHandler(this)
    m_ocrEngine(),
    m_screenCaptureWindows()
{
    m_ui->setupUi(this);

    connect(m_ui->searchButton, SIGNAL(clicked()), this, SLOT(search()));
    connect(m_ui->searchBar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(m_ui->advancedCheckbox, SIGNAL(clicked(bool)),
            m_ui->advancedScrollArea, SLOT(setVisible(bool)));

    connect(m_ui->inputToolsButton, SIGNAL(clicked()), this,
            SLOT(activateScreenCapture()));

    /////
    m_ui->advancedCheckbox->setVisible(false);
    m_ui->sortByComboBox->setVisible(false);
    m_ui->sortByLabel->setVisible(false);
    /////

    search();
    m_ui->advancedScrollArea->setVisible(false);
    m_ui->advancedCheckbox->setChecked(false);

    m_ui->contentsView->settings()->setDefaultTextEncoding("utf-8");
    m_ui->contentsView->page()->profile()->installUrlSchemeHandler("tetsudai", &termlistUrlHandler);

    connect(&termlistUrlHandler, SIGNAL(listTermClicked(int)), this, SLOT(selectListItem(int)));

    //ui->contentsView->setStyleSheet("");
    //QString entryListViewHtml = QCoreApplication::applicationDirPath()
    //        + "/res/entrylistview.html";
    //ui->contentsView->load(QUrl::fromLocalFile(entryListViewHtml));

    std::cout << QStyleFactory::keys().join(" ").toStdString() << std::endl;
}

SearchWidget::~SearchWidget() {
    m_ui->contentsView->page()->profile()->removeUrlScheme("tetsudai");

    for (Entry* entry : m_entries) {
        delete entry;
    }

    delete m_ui;
}

void SearchWidget::search() {
    QString searchTerm = m_ui->searchBar->text();
    if (m_ui->deinflectAutomaticallyCheckbox->isChecked()) {
        searchTerm = m_engine.deconjugate(searchTerm);
    }
    setEntries(m_engine.search(searchTerm));
}

void SearchWidget::setEntries(const QList<Entry*>& entries) {
    if (&m_entries != &entries) {
        for (Entry* entry : m_entries) {
            delete entry;
        }
        m_entries = entries;
        std::sort(m_entries.begin(), m_entries.end(), [](const Entry* l, const Entry* r){ return l->frequency().value < r->frequency().value; });

        for (int i = 0; i < m_entries.size(); i++) {
            std::cout << m_entries[i]->frequency().value <<std::endl;
        }

        m_ui->contentsView->setHtml(entriesToHtml(m_entries));
    }
}

void SearchWidget::selectListItem(int index) {
    m_ui->contentsView->setHtml(m_entries[index]->displayHtml());
}

QColor SearchWidget::freqToColor (Scale<int> freq) {

    double value = 510 - (std::pow(freq.value / (double) freq.max, 0.35) * 510);
    //value = 510 - ((freq / maxFreq) * 510);
    //value = 510 - ((std::sqrt(std::sqrt(freq)) / std::sqrt(std::sqrt(maxFreq))) * 510);

    // value must be between [0, 510]

    double redValue;
    double greenValue;

    if (value < 255) {
        redValue = 255;
        greenValue = value;
    }
    else {
        greenValue = 255;
        redValue = 255 - (value - 255);
    }

    /*if (value < 255) {
        redValue = 255;
        greenValue = std::sqrt(value) * 16;
        greenValue = std::round(greenValue);
    } else {
        greenValue = 255;
        value = value - 255;
        redValue = 255 - (value * value / 255);
        redValue = std::round(redValue);
    }*/

    return QColor((int) redValue, (int) greenValue, 0);

}

QString SearchWidget::entriesToHtml (const QList<Entry*>& entries) {

    QString backgroundColor = QApplication::palette().base().color().name();
    QString textColor = QApplication::palette().text().color().name();
    QString hoverColor = QApplication::palette().alternateBase().color().name();

    std::stringstream htmlStream;
    htmlStream << "<!DOCTYPE html>"
               << "<html>"
               << "<head>"
               << "<title>Entry List</title>"
               //<< "<script type=\"text/javascript\" src=\"https://getfirebug.com/firebug-lite.js\"></script>"
               << "<style>"

               << "* {"
               << "    margin: 0px;"
               << "    padding: 0px;"
               << "}"

               << "body {"
               << "    background-color: " << backgroundColor.toStdString() << ";"
               << "    color: " << textColor.toStdString() << ";"
               << "}"

               << ".kanji {"
               << "    font-size: 2em;"
               << "}"

               << ".kana {"
               << "    font-size: 1em;"
               << "}"

               << ".definition {"
               << "    margin-left: 6px;"
               << "    margin-top: 6px;"
               << "}"

               << ".entry {"
               << "    padding-bottom: 10px;"
               << "    padding-top: 6px;"
               << "    padding-left: 6px;"
               << "    padding-right: 6px;"

               << "    border-left-style: solid;"
               << "    border-left-width: 10px;"
               << "}"

               << ".entry:hover {"
               << "    background-color: " << hoverColor.toStdString() << ";"
               << "}"

               << "a {"
               << "    color: inherit;"
               << "    text-decoration: inherit;"
               << "    cursor: default;"
               << "}"

               << "</style>"
               << "</head>"
               << "<body>";

    for (int q = 0; q < entries.size(); q++) {
        Entry* entry = entries[q];
        htmlStream << "<a href=\""
                   << "tetsudai:item/"
                   << q
                   << "\">"
                   << "<div class=\"entry\" style=\"border-left-color: "
                   << freqToColor(entry->frequency()).name().toStdString()
                   << "\" title=\""
                   << entry->frequency().value
                   << "\">"
                   << "<div class=\"term\">";
        if (entry->forms().empty()) {
            // no k_eles, so draw reading entries big
            for (decltype(entry->pronunciations().size()) i = 0; i < entry->pronunciations().size(); i++) {
               htmlStream << "<span class=\"kanji\">"
                          << entry->pronunciations()[i].pronunciation.toStdString()
                          << "</span>";
               if (i != entry->pronunciations().size() - 1) {
                   htmlStream << "<span class=\"kanji\">"
                              << "・"
                              << "</span>";
               }
           }
       } else {
            htmlStream << "<span class=\"kanji\">";
            // there are k_eles, so must draw each big with readings smaller
            // on the side
            for (decltype(entry->forms().size()) i = 0; i < entry->forms().size(); i++) {
                htmlStream << entry->forms()[i].toStdString();
                if (i != entry->forms().size() - 1) {
                    htmlStream << "・";
                }
            }
            htmlStream << "</span>";

            htmlStream << "<span class=\"kana\">(";
            for (decltype(entry->pronunciations().size()) i = 0; i < entry->pronunciations().size(); i++) {
                //htmlStream << entry->pronunciations()[i].forms.size() << std::endl;
                for (decltype(entry->pronunciations()[i].forms.size()) j = 0; j < entry->pronunciations()[i].forms.size(); j++) {
                    htmlStream << entry->pronunciations()[i].forms[j].toStdString();
                    if (j != entry->pronunciations()[i].forms.size() - 1) {
                        htmlStream << "、";
                    }
                    if (j == entry->pronunciations()[i].forms.size() - 1) {
                        htmlStream << "：";
                    }
                }



                htmlStream << entry->pronunciations()[i].pronunciation.toStdString();
                if (i != entry->pronunciations().size() - 1) {
                    htmlStream << "・";
                }
            }
            htmlStream << ")</span>";
       }
       htmlStream << "</div>"
                  << "<div class=\"definition\">"
                  << entry->definitions().toStdString()
                  << "</div>"
                  << "</div>"
                  << "</a>";
   }

   htmlStream << "</body>"
              << "</html>";
   //std::cout << htmlStream.str() << std::endl;
   return QString::fromStdString(htmlStream.str());

}

void SearchWidget::activateScreenCapture() {

    for (int screenId = 0; screenId < QApplication::screens().length(); screenId++) {
        OverlayWindow* screenCaptureWindow = new OverlayWindow(screenId);
        screenCaptureWindow->setAttribute(Qt::WA_DeleteOnClose);
        screenCaptureWindow->setWindowFlags(Qt::Dialog | Qt::WindowDoesNotAcceptFocus);

        connect(screenCaptureWindow, SIGNAL(screenCaptured(QImage, QRect)),
                &m_ocrEngine, SLOT(startRecognition(QImage, QRect)));
        connect(&m_ocrEngine, SIGNAL(recognitionFinished(QString)),
                m_ui->searchBar, SLOT(setText(QString)));
        connect(screenCaptureWindow, SIGNAL(closeSignaled()),
                this, SLOT(closeScreenCaptureWindows()));

        m_screenCaptureWindows.push_back(screenCaptureWindow);

        screenCaptureWindow->show();

        auto screenObject = QApplication::screens().at(screenId);
        auto screenGeometry = screenObject->geometry();
        screenCaptureWindow->move(screenGeometry.x(),
                                  screenGeometry.y());

        screenCaptureWindow->hide();
        screenCaptureWindow->showFullScreen();
    }

}

void SearchWidget::closeScreenCaptureWindows() {
    for (auto screenCaptureWindow : m_screenCaptureWindows) {
        screenCaptureWindow->close();
    }
    m_screenCaptureWindows.clear();
}
