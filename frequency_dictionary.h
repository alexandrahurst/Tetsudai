#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <experimental/optional>
#include <QSqlDatabase>
#include <QString>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QVariant>

namespace Frequency {

class Dictionary {
public:
    Dictionary();
    Dictionary (const Dictionary& other) = delete;
    Dictionary& operator= (const Dictionary& other) = delete;
    ~Dictionary();

    int minValue () const;
    int maxValue () const;
    const std::experimental::optional<int> getFrequency (const QString& term) const;

private:
    QSqlDatabase mFrequencyDb;
    int mMinValue;
    int mMaxValue;

};

}

#endif // DICTIONARY_H
