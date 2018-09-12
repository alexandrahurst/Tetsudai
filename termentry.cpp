#include "termentry.h"

#include <iostream>

TermEntry::TermEntry (JMdict::Entry jmdictEntry, Scale<int> frequency) :
    m_jmdictEntry(jmdictEntry), m_frequency(frequency) { }

TermEntry::~TermEntry () { }

Scale<int> TermEntry::frequency () const {
    return m_frequency;
}

QList<QString> TermEntry::forms() const {
    QList<QString> forms;

    for (const auto& kEle : m_jmdictEntry.kEle) {
        std::stringstream stream;
        stream << "<span title=\"";
        for (int i = 0; i < kEle.keInf.size(); i++) {
            stream << kEle.keInf[i].full.toStdString();
            if (i != kEle.keInf.size() - 1) {
                stream << "; ";
            }
        }
        stream << "\">"
               << kEle.keb.toStdString()
               << "</span>";

        forms.push_back(QString::fromStdString(stream.str()));
    }

    return forms;
}

QList<Entry::Pronunciation> TermEntry::pronunciations () const {
    QList<Entry::Pronunciation> pronunciations;

    for (const auto& rEle : m_jmdictEntry.rEle) {
        std::stringstream stream;
        stream << "<span title=\"";
        for (int i = 0; i < rEle.reInf.size(); i++) {
            stream << rEle.reInf[i].full.toStdString();
            if (i != rEle.reInf.size() - 1) {
                stream << "; ";
            }
        }
        stream << "\">"
               << rEle.reb.toStdString()
               << "</span>";

        QString pronunciation(QString::fromStdString(stream.str()));

        QList<QString> forms;
        for (QList<JMdict::KanjiElement>::size_type form : rEle.reRestr) {
            forms.push_back(m_jmdictEntry.kEle[form].keb);
        }

        pronunciations.push_back(Pronunciation {pronunciation, forms});
    }

    return pronunciations;
}

QString TermEntry::definitions () const {
    std::stringstream stream;

    for (decltype(m_jmdictEntry.sense.size()) i = 0;
         i < m_jmdictEntry.sense.size(); i++) {
        if(!m_jmdictEntry.sense[i].pos.empty()) {
            // part of speech has changed; add part of speech in parentheses
            stream << "(";
            for (decltype(m_jmdictEntry.sense[i].pos.size()) j = 0;
                 j < m_jmdictEntry.sense[i].pos.size();
                 j++) {
                stream << "<span title=\""
                       << m_jmdictEntry.sense[i].pos[j]
                          .full.toStdString()
                       << ": "
                       << m_jmdictEntry.sense[i].pos[j]
                          .description.value_or("").toStdString()
                       << "\">"
                       << m_jmdictEntry.sense[i].pos[j]
                          .abbreviation.value_or("").toStdString()
                       << "</span>";

                if (j != m_jmdictEntry.sense[i].pos.size() - 1) {
                    stream << ", ";
                }
            }

            stream << ") ";
        }
        stream << "("
               << i + 1
               << ") ";
        if(!m_jmdictEntry.sense[i].gloss.empty()) {
            // there are glosses, add them, separated by slash
            for (decltype(m_jmdictEntry.sense[i].gloss.size()) j = 0;
                 j < m_jmdictEntry.sense[i].gloss.size(); j++) {
                stream << m_jmdictEntry.sense[i].gloss[j].term.toStdString();
                if (j != m_jmdictEntry.sense[i].gloss.size() - 1) {
                    stream << "/";
                }
            }
        }

        if (i != m_jmdictEntry.sense.size() - 1) {
            stream << "; ";
        }
    }

    return QString::fromStdString(stream.str());
}

QString TermEntry::displayHtml () const {

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

               << "@font-face {"
               << "    font-family: 'KanjiStrokeOrders';"
               << "    src: url('file://"
               << QCoreApplication::applicationDirPath().toStdString()
               << "/res/KanjiStrokeOrders_v3.001.ttf') format('truetype');"
               << "}"

               << "body {"
               << "    background-color: " << backgroundColor.toStdString() << ";"
               << "    color: " << textColor.toStdString() << ";"
               << "    margin: 0px;"
               << "    padding: 6px;"
               << "}"

               << "table {"
               << "    border: 1px solid " << textColor.toStdString() << ";"
               << "    border-collapse: collapse;"
               << "    margin-bottom: 6px;"
               << "    width: 100%;"
               << "}"

               << "td {"
               << "    border: 1px solid " << textColor.toStdString() << ";"
               << "    padding: 6px;"
               << "}"

               << ".kanji {"
               << "    font-size: 2em;"
               << "}"

               << ".variant {"
               << "    margin-bottom: 6px;"
               << "}"

               << ".kana {"
               << "    font-size: 1em;"
               << "}"

               << ".stroke-diagram {"
               << "    font-family: 'KanjiStrokeOrders';"
               << "    font-size: 7em;"
               << "}"

               << ".reading-meaning-column {"
               << "    width: 99%;"
               << "}"

               << "ul {"
               << "    list-style-type: circle;"
               << "    margin: 10px;"
               << "}"

               << "li {"
               << "    list-style-type: circle;"
               << "}"

               << "</style>"
               << "</head>"
               << "<body>";

    auto forms = m_jmdictEntry.kEle;
    auto pronunciations = m_jmdictEntry.rEle;
    for (decltype(forms.size()) i = 0; i < forms.size(); i++) {
        htmlStream << "<div class=\"variant\">"
                   << "<span class=\"kanji\">"
                   << forms[i].keb.toStdString()
                   << "</span>"

                   << "<span class=\"kana\">（";
        bool prevPronunciation = false;
        for (decltype(pronunciations.size()) j = 0;
             j < pronunciations.size(); j++) {

            //if (!pronunciations[j].forms.empty()) {
                std::cout << "***" << pronunciations[j].reb.toStdString() << std::endl;
            //};
            QList<QString> formsForPronunciation;
            for (QList<JMdict::KanjiElement>::size_type form : pronunciations[j].reRestr) {
                formsForPronunciation.push_back(m_jmdictEntry.kEle[form].keb);
            }
            if (formsForPronunciation.empty()
                    || formsForPronunciation.contains(forms[i].keb)) {
                if (prevPronunciation) {
                    htmlStream << "・";
                }
                prevPronunciation = true;
                htmlStream << pronunciations[j].reb.toStdString();
            }
        }
        htmlStream << "）</span>"
                   << "</div>";

        for (QString character : forms[i].keb) {
            if (isKanji(character)) {

                std::experimental::optional<Kanjidic2::Entry> kanjiEntry =
                        Kanjidic2::Dictionary::getInstance().getEntry(character);
                if (kanjiEntry) {
                    htmlStream << "<table>"
                               << "<tr>"
                               << "<td rowspan=\"2\" class=\"stroke-diagram\">"
                               << kanjiEntry.value().literal.toStdString()
                               << "</td>"
                               << "<td class=\"reading-meaning-column\">"
                               << kanjiEntry.value().reading.join("・").toStdString()
                               << "</td>"
                               << "</tr>"
                               << "<tr>"
                               << "<td>"
                               << kanjiEntry.value().meaning.join(", ").toStdString()
                               << "</td>"
                               << "</tr>"
                               << "</table>";
                }
            }
        }



        // @@@ Table @@@

        // TODO insert table
        // TODO finish term view
        // TODO fix re_restr (list view and term view)
        // TODO fix back/forward navigation
        // TODO integrate outher input methods
        // TODO add term recording
    }

    for (decltype(m_jmdictEntry.sense.size()) i = 0;
         i < m_jmdictEntry.sense.size();
         i++) {
        std::cout << "hi tomo" << std::endl;
        const JMdict::Sense& sense = m_jmdictEntry.sense[i];
        if (!sense.pos.empty()) {
            if (i != 0) {
                htmlStream << "</ul>";
            }

            QStringList posFullList;
            std::transform(sense.pos.begin(),
                           sense.pos.end(),
                           std::back_inserter(posFullList),
                           [](const JMdict::Entity& pos){ return pos.full; });
            htmlStream << "<div class=\"pos\">"
                       << posFullList.join(", ").toStdString()
                       << "</div>"

                       << "<ul>";
        }


        QStringList glossTermList;
        std::transform(sense.gloss.begin(),
                       sense.gloss.end(),
                       std::back_inserter(glossTermList),
                       [](const JMdict::Sense::Gloss& gloss){ return gloss.term; });
        htmlStream << "<li class=\"definition\">"
                   << glossTermList.join(", ").toStdString()
                   << "</li>";

    }
    htmlStream << "</ul>";

    htmlStream << "</body>"
               << "</html>";


    return QString::fromStdString(htmlStream.str());;
}

bool TermEntry::isKanji (const QString& str) const {
   std::cout << str.toStdString() << std::endl;
   QRegExp re("^[\u4e00-\u9faf]$");
   return re.exactMatch(str);
}
