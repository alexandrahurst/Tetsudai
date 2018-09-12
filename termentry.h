#ifndef TERMENTRY_H
#define TERMENTRY_H

#include "jmdict_entry.h"
#include "entry.h"
#include "scale.h"
#include "kanjidic2_dictionary.h"

#include <sstream>
#include <QApplication>
#include <QPalette>
#include <QRegExp>
#include <algorithm>

class TermEntry : public Entry {

private:
    JMdict::Entry m_jmdictEntry;
    Scale<int> m_frequency;

public:
    TermEntry (JMdict::Entry jmdictEntry, Scale<int> frequency);
    virtual ~TermEntry ();

    virtual Scale<int> frequency () const;

    virtual QList<QString> forms () const;
    virtual QList<Pronunciation> pronunciations () const;
    virtual QString definitions () const;

    virtual QString displayHtml () const;

private:
    bool isKanji (const QString &str) const;


};

#endif // TERMENTRY_H
