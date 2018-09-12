#ifndef JMDICT_ENTRY_H
#define JMDICT_ENTRY_H

#include <QList>
#include <QMetaType>

#include "jmdict_kanjielement.h"
#include "jmdict_readingelement.h"
#include "jmdict_sense.h"

namespace JMdict {

struct Entry {
    int entSeq;
    QList<KanjiElement> kEle;
    QList<ReadingElement> rEle;
    QList<Sense> sense;
};

}

Q_DECLARE_METATYPE(JMdict::Entry)

#endif // JMDICT_ENTRY_H

