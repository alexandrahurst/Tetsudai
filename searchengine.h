#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "entry.h"
#include "termentry.h"
#include "jmdict_dictionary.h"
#include "frequency_dictionary.h"
#include <QList>
#include <QString>

class SearchEngine
{
public:
    SearchEngine();
    SearchEngine (const SearchEngine& other) = delete;
    SearchEngine& operator= (const SearchEngine& other) = delete;
    ~SearchEngine();

    QString deconjugate (const QString& term) const;
    QList<Entry*> search (const QString& term) const;

private:
    JMdict::Dictionary m_jmdictDict;
    Frequency::Dictionary m_frequencyDict;

    Scale<int> frequencyForEntry (const JMdict::Entry& entry) const;
};

#endif // SEARCHENGINE_H
