#ifndef JMDICT_DICTIONARY_H
#define JMDICT_DICTIONARY_H

#include <array>
#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <sstream>
#include <utility>

#include "jmdict_entry.h"

namespace JMdict {

class Dictionary {
public:
    Dictionary();
    Dictionary (const Dictionary& other) = delete;
    Dictionary& operator= (const Dictionary& other) = delete;
    ~Dictionary();

    QList<JMdict::Entry> getEntries (const QString& term) const;

private:
    QMap<QString, Entity> entities;
    QSqlDatabase mDefinitionDb;

    const Entity convertEntity (const QString& entity) const;
    Entry entryFromEnt_seq (int ent_seq) const;
    QSet<int> ent_seqsFromTermInexact (const QString& term) const;


    template<int N>
    QList<std::array<QVariant, N>> selectValues (
            const std::array<QString, N>& columns, const QString& table,
            const QString& whereColumn, const QVariant& whereValue) const {
        QList<std::array<QVariant, N>> values;

        QSqlQuery query;
        query.setForwardOnly(true);

        std::stringstream statement;
        statement << "SELECT ";
        for (decltype(columns.size()) i = 0; i < columns.size(); i++) {
            statement << columns[i].toStdString();
            if (i != columns.size() - 1) {
                statement << ", ";
            } else {
                statement << " ";
            }
        }
        statement << "FROM " << table.toStdString() << " "
                  << "WHERE " << whereColumn.toStdString() << "=?";

        query.prepare(QString::fromStdString(statement.str()));
        query.addBindValue(whereValue);
        query.exec();
        while (query.next()) {
            values.push_back(std::array<QVariant, N>());
            for (int i = 0; i < N; i++) {
                values.back()[i] = query.value(i);
            }
        }

        return values;
    }

};

}

#endif // JMDICT_DICTIONARY_H
