#ifndef KANJIDIC2_DICTIONARY_H
#define KANJIDIC2_DICTIONARY_H

#include <array>
#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <sstream>
#include <utility>
#include <experimental/optional>
#include <QCoreApplication>

#include "kanjidic2_entry.h"

namespace Kanjidic2 {

class Dictionary {
public:
    Dictionary();
    Dictionary (const Dictionary& other) = delete;
    Dictionary& operator= (const Dictionary& other) = delete;
    ~Dictionary();

    static Dictionary& getInstance();

    std::experimental::optional<Kanjidic2::Entry> getEntry (const QString& kanji) const;

private:
    QSqlDatabase m_database;

};

}

#endif // KANJIDIC2_DICTIONARY_H
