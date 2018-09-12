#include "kanjidic2_dictionary.h"

using std::experimental::optional;
using std::experimental::nullopt;
using namespace Kanjidic2;

Dictionary::Dictionary() :
    m_database(QSqlDatabase::addDatabase(QString("QSQLITE"),QString("kanjidic2")))
{

    m_database.setDatabaseName(QString(QCoreApplication::applicationDirPath()
                                          + "/res/kanjidic2.sqlite"));
    m_database.open();
}

Dictionary::~Dictionary() {
    m_database.close();
}

Dictionary& Dictionary::getInstance() {
    static Dictionary instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
    return instance;
}

optional<Entry> Dictionary::getEntry (const QString& kanji) const {
    QSqlQuery kanjiQuery(m_database);
    kanjiQuery.prepare("SELECT id "
                       "FROM kanji "
                       "WHERE literal=?");
    kanjiQuery.addBindValue(kanji);
    kanjiQuery.exec();
    int kanjiId;
    if (kanjiQuery.next()) {
        kanjiId = kanjiQuery.value(0).toInt();
    } else {
        return optional<Entry>(nullopt);
    }

    QSqlQuery readingQuery(m_database);
    readingQuery.prepare("SELECT data "
                         "FROM reading "
                         "WHERE kanji=?");
    readingQuery.addBindValue(kanjiId);
    readingQuery.exec();

    QStringList readings;
    while (readingQuery.next()) {
        readings.push_back(readingQuery.value(0).toString());
    }

    QSqlQuery meaningQuery(m_database);
    meaningQuery.prepare("SELECT data "
                         "FROM meaning "
                         "WHERE kanji=?");
    meaningQuery.addBindValue(kanjiId);
    meaningQuery.exec();

    QStringList meanings;
    while (meaningQuery.next()) {
        meanings.push_back(meaningQuery.value(0).toString());
    }

    Entry entry {
        kanji,
        readings,
        meanings,
    };
    return optional<Entry>(entry);
}
