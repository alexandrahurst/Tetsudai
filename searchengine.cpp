#include "searchengine.h"

SearchEngine::SearchEngine() : m_jmdictDict(), m_frequencyDict() { }

SearchEngine::~SearchEngine() { }

QString SearchEngine::deconjugate (const QString& term) const {
    return term; // TODO
}

QList<Entry*> SearchEngine::search (const QString& term) const {
    QList<Entry*> entries;

    QList<JMdict::Entry> jmdictEntries = m_jmdictDict.getEntries(term);
    for (auto& jmdictEntry : jmdictEntries) {
        entries.push_back(new TermEntry(jmdictEntry,
                                        frequencyForEntry(jmdictEntry)));
    }

    // TODO kanji and name entries

    return entries;
}

Scale<int> SearchEngine::frequencyForEntry (const JMdict::Entry& entry) const {
    int minFreq = m_frequencyDict.minValue();
    int maxFreq = m_frequencyDict.maxValue();
    int value = 0;
    if (entry.kEle.empty()) {
        value = m_frequencyDict.getFrequency(entry.rEle[0].reb)
                .value_or(maxFreq);
    } else {
        bool usuallyKana = false;
        for (auto& sense : entry.sense) {
            for (auto& misc : sense.misc) {
                if (misc.abbreviation.value_or("").compare("uk") == 0) {
                    usuallyKana = true;
                }
            }
        }
        if (usuallyKana) {
            value = m_frequencyDict.getFrequency(entry.rEle[0].reb)
                    .value_or(maxFreq);
        } else {
            value = m_frequencyDict.getFrequency(entry.kEle[0].keb)
                    .value_or(maxFreq);
        }
    }

    return Scale<int>{minFreq, maxFreq, value};
}
