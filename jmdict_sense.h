#ifndef SENSE_H
#define SENSE_H

#include <experimental/optional>
#include <QList>
#include <QRegExp>
#include <QString>
#include <QStringList>

#include "jmdict_entity.h"
#include "jmdict_kanjielement.h"
#include "jmdict_readingelement.h"

namespace JMdict {

struct Sense {

    struct Xref {
        std::experimental::optional<QString> keb;
        std::experimental::optional<QString> reb;
        std::experimental::optional<int> senseNum;
    };

    struct LoanwordSource {
        enum class Type {
            full,
            part,
        };

        QString lang;
        Type lsType;
        bool lsWasei;
        std::experimental::optional<QString> term;
    };

    struct Gloss {
        QString lang;
        std::experimental::optional<QString> gGend;
        bool pri;
        QString term;
    };

    QList<QList<KanjiElement>::size_type> stagk;
    QList<QList<ReadingElement>::size_type> stagr;
    QList<Entity> pos;
    QList<Xref> xref;
    QList<Xref> ant;
    QList<Entity> field;
    QList<Entity> misc;
    QList<QString> sInf;
    QList<LoanwordSource> lsource;
    QList<Entity> dial;
    QList<Gloss> gloss;
};

Sense::Xref parseXref (const QString& str);
bool isReb (const QString& str);
bool isNumber (const QString& str);

}

#endif // SENSE_H

