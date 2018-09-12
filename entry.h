#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QList>

#include "scale.h"

class Entry {

public:

    struct Pronunciation {
        QString pronunciation;
        QList<QString> forms;
    };

    virtual ~Entry () {}

    virtual Scale<int> frequency () const = 0;

    // Note: use html <span title=""><span> for hover text
    virtual QList<QString> forms () const = 0;
    virtual QList<Pronunciation> pronunciations () const = 0;
    virtual QString definitions () const = 0;

    virtual QString displayHtml () const = 0;

};

#endif // ENTRY_H
