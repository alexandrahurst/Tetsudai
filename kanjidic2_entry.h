#ifndef KANJIDIC2_ENTRY_H
#define KANJIDIC2_ENTRY_H

#include <QList>
#include <QMetaType>
#include <QString>
#include <QStringList>

#include "jmdict_kanjielement.h"
#include "jmdict_readingelement.h"
#include "jmdict_sense.h"

namespace Kanjidic2 {

struct Entry {
    QString literal;
    QStringList reading;
    QStringList meaning;
};

}

Q_DECLARE_METATYPE(Kanjidic2::Entry)

#endif // KANJIDIC2_ENTRY_H
