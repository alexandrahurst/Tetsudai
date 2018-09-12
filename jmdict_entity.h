#ifndef JMDICT_ENTITY_H
#define JMDICT_ENTITY_H

#include <experimental/optional>
#include <QString>

namespace JMdict {

struct Entity {
    QString full;
    std::experimental::optional<QString> abbreviation;
    std::experimental::optional<QString> description;
};

}

#endif // JMDICT_ENTITY_H
