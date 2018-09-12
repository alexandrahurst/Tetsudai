#ifndef JMDICT_KANJIELEMENT_H
#define JMDICT_KANJIELEMENT_H

#include "jmdict_entity.h"

#include <QList>
#include <QString>

namespace JMdict {

struct KanjiElement {
    QString keb;
    QList<Entity> keInf;
    QList<QString> kePri;
};

}

#endif // JMDICT_KANJIELEMENT_H

