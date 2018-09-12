#ifndef JMDICT_READINGELEMENT_H
#define JMDICT_READINGELEMENT_H

#include <QList>
#include <QString>

#include "jmdict_entity.h"
#include "jmdict_kanjielement.h"

namespace JMdict {

struct ReadingElement {

    QString reb;
    bool reNokanji;
    QList<QList<KanjiElement>::size_type> reRestr;
    QList<Entity> reInf;
    QList<QString> rePri;

};

}

#endif // JMDICT_READINGELEMENT_H

