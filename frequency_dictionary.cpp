#include "frequency_dictionary.h"

using std::experimental::optional;
using std::experimental::nullopt;
using namespace Frequency;

Dictionary::Dictionary() :
    mFrequencyDb(QSqlDatabase::addDatabase(QString("QSQLITE"),QString("freq")))
{

    mFrequencyDb.setDatabaseName(QString(QCoreApplication::applicationDirPath()
                                          + "/res/freq.sqlite"));
    mFrequencyDb.open();

    QSqlQuery minQuery(mFrequencyDb);
    minQuery.prepare("SELECT MIN(freq) "
                     "FROM Dict");
    minQuery.exec();
    minQuery.next();
    mMinValue = minQuery.value(0).toInt();

    QSqlQuery maxQuery(mFrequencyDb);
    maxQuery.prepare("SELECT MAX(freq) "
                     "FROM Dict");
    maxQuery.exec();
    maxQuery.next();
    mMaxValue = maxQuery.value(0).toInt();
}

Dictionary::~Dictionary() {
    mFrequencyDb.close();
}

const optional<int> Dictionary::getFrequency(const QString& term) const {
    QSqlQuery freqQuery(mFrequencyDb);
    freqQuery.prepare("SELECT freq "
                      "FROM Dict "
                      "WHERE expression=?");
    freqQuery.addBindValue(term);
    freqQuery.exec();
    if (freqQuery.next()) {
        return optional<int>(freqQuery.value(0).toInt());
    } else {
        return optional<int>(nullopt);
    }

}

int Dictionary::minValue() const {
    return mMinValue;
}

int Dictionary::maxValue() const {
    return mMaxValue;
}
