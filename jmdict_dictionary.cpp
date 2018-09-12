#include "jmdict_dictionary.h"
#include "jmdict_entry.h"
#include "jmdict_kanjielement.h"
#include "jmdict_readingelement.h"
#include "jmdict_sense.h"

#include <iostream>
#include <QFile>
#include <QString>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>
#include <QtXmlPatterns/QXmlItem>
#include <QtXmlPatterns/QXmlNodeModelIndex>
#include <QVariant>
#include <QCoreApplication>
#include <cstring>
#include <QtSql/QSqlQuery>
#include <ctime>
#include <vector>
#include <QSet>
#include <QIODevice>
#include <QTextStream>

using std::experimental::optional;
using std::experimental::nullopt;
using namespace JMdict;

Dictionary::Dictionary() :
    mDefinitionDb(QSqlDatabase::addDatabase(QString("QSQLITE")))
{
    QFile entitiesFile(QString(QCoreApplication::applicationDirPath()
                             + "/res/entities.tsv"));
    if (entitiesFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&entitiesFile);
        while (!in.atEnd()) {
            QStringList line = in.readLine().split("\t");
            entities.insert(line[0], Entity {
                line[2],
                optional<QString>(line[1]),
                optional<QString>(line[3])});
        }
        entitiesFile.close();
    }

    mDefinitionDb.setDatabaseName(QString(QCoreApplication::applicationDirPath()
                                          + "/res/JMdict_e_TEST.sqlite"));
    mDefinitionDb.open();
}

Dictionary::~Dictionary() {
    mDefinitionDb.close();
}

QList<JMdict::Entry> Dictionary::getEntries (const QString& term) const {

    if (!term.isEmpty()) {
        std::clock_t start = clock();

        std::cout << "検索する前: " << ((double) clock() - start)/CLOCKS_PER_SEC
                  << std::endl;

        // get ent_seqs of matching entries
        QSet<int> ent_seqs = ent_seqsFromTermInexact(term);

        std::cout << "検索する後: " << ((double) clock() - start)/CLOCKS_PER_SEC
                  << std::endl;

        QList<Entry> entries;
        // turn ent_seqs into full Entrys
        for (int ent_seq : ent_seqs) {
            entries.push_back(entryFromEnt_seq(ent_seq));
        }

        std::cout << "Entryを作る後: " << ((double) clock() - start)/CLOCKS_PER_SEC
                  << std::endl;

        for (int ent_seq : ent_seqs) {
            std::cout << ent_seq << std::endl;
        }

        return entries;

    } else {
        QList<Entry> entries;
        return entries;
    }

}

Entry Dictionary::entryFromEnt_seq(int ent_seq) const {
    // --- k_ele --- //

    QList<KanjiElement> k_eles;
    for (auto& k_eleData :
         selectValues<2>({{"id", "keb"}}, "k_ele", "entry", ent_seq)) {
        // - keb - //

        int id = k_eleData[0].toInt();
        QString keb = k_eleData[1].toString();

        // - ke_inf - //

        QList<Entity> ke_infs;
        for (auto& ke_infData :
             selectValues<1>({{"data"}}, "ke_inf", "k_ele", id)) {
            ke_infs.push_back(convertEntity(ke_infData[0].toString()));
        }

        // - ke_pri - //

        QList<QString> ke_pris;
        for (auto& ke_priData :
             selectValues<1>({{"data"}}, "ke_pri", "k_ele", id)) {
            ke_pris.push_back(ke_priData[0].toString());
        }

        KanjiElement k_ele {
            keb,
            ke_infs,
            ke_pris,
        };
        k_eles.push_back(k_ele);

    }

    // --- r_ele --- //

    QList<ReadingElement> r_eles;
    for (auto& r_eleData :
         selectValues<3>({{"id", "reb", "re_nokanji"}}, "r_ele", "entry",
                         ent_seq)) {
        // - reb - //
        // - re_nokanji - //

        int id = r_eleData[0].toInt();
        QString reb = r_eleData[1].toString();
        bool re_nokanji = r_eleData[2].toBool();

        // - re_restr - //

        QList<QList<KanjiElement>::size_type> re_restrs;
        for (auto& re_restrData :
             selectValues<1>({{"data"}}, "re_restr", "r_ele", id)) {
            QString re_restr = re_restrData[0].toString();
            std::cout << "RE_RESTR: " << re_restr.toStdString() << std::endl;
            for (QList<KanjiElement>::size_type i = 0;
                 i < k_eles.size(); i++) {
                if (k_eles[i].keb.compare(re_restr) == 0) {
                    re_restrs.push_back(i);
                    break;
                }
            }
        }

        // - re_inf - //

        QList<Entity> re_infs;
        for (auto& re_infData :
             selectValues<1>({{"data"}}, "re_inf", "r_ele", id)) {
            re_infs.push_back(convertEntity(re_infData[0].toString()));
        }

        // - re_pri - //

        QList<QString> re_pris;
        for (auto& re_priData :
             selectValues<1>({{"data"}}, "re_pri", "r_ele", id)) {
            re_pris.push_back(re_priData[0].toString());
        }

        ReadingElement r_ele {
            reb,
            re_nokanji,
            re_restrs,
            re_infs,
            re_pris,
        };
        r_eles.push_back(r_ele);

    }

    // --- sense --- //

    QList<Sense> senses;
    for (auto& senseData : selectValues<1>({{"id"}}, "sense", "entry", ent_seq)) {
        int id = senseData[0].toInt();

        // - stagk - //

        QList<decltype(k_eles.size())> stagks;
        for (auto& stagkData : selectValues<1>({{"data"}}, "stagk", "sense", id)) {
            QString stagk = stagkData[0].toString();
            for (decltype(k_eles.size()) i = 0; i < k_eles.size();
                 i++) {
                if (k_eles[i].keb.compare(stagk) == 0) {
                    stagks.push_back(i);
                    break;
                }
            }
        }

        // - stagr - //

        QList<decltype(r_eles.size())> stagrs;
        for (auto& stagrData : selectValues<1>({{"data"}}, "stagr", "sense", id)) {
            QString stagr = stagrData[0].toString();
            for (decltype(r_eles.size()) i = 0; i < r_eles.size();
                 i++) {
                if (r_eles[i].reb.compare(stagr) == 0) {
                    stagrs.push_back(i);
                    break;
                }
            }
        }

        // - pos - //

        QList<Entity> poses;
        for (auto& posData : selectValues<1>({{"data"}}, "pos", "sense", id)) {
            poses.push_back(convertEntity(posData[0].toString()));
        }

        // - xref - //

        QList<Sense::Xref> xrefs;
        for (auto& xrefData : selectValues<1>({{"data"}}, "xref", "sense", id)) {
            xrefs.push_back(parseXref(xrefData[0].toString()));
        }

        // - ant - //

        QList<Sense::Xref> ants;
        for (auto& antData : selectValues<1>({{"data"}}, "ant", "sense", id)) {
            ants.push_back(parseXref(antData[0].toString()));
        }

        // - field - //

        QList<Entity> fields;
        for (auto& fieldData : selectValues<1>({{"data"}}, "field", "sense", id)) {
            fields.push_back(convertEntity(fieldData[0].toString()));
        }

        // - misc - //

        QList<Entity> miscs;
        for (auto& miscData : selectValues<1>({{"data"}}, "misc", "sense", id)) {
            miscs.push_back(convertEntity(miscData[0].toString()));
        }

        // - s_inf - //

        QList<QString> s_infs;
        for (auto& s_infData : selectValues<1>({{"data"}}, "s_inf", "sense", id)) {
            s_infs.push_back(s_infData[0].toString());
        }

        // - lsource - //
        QList<Sense::LoanwordSource> lsources;
        for (auto& lsourceData :
             selectValues<4>({{"lang", "ls_type", "ls_wasei", "term"}},
                             "lsource", "sense", id)) {
            Sense::LoanwordSource lsource {
                lsourceData[0].toString(),
                lsourceData[1].toString() == "part" ?
                            Sense::LoanwordSource::Type::part :
                            Sense::LoanwordSource::Type::full,
                lsourceData[2].toBool(),
                lsourceData[3].isNull() ?
                            optional<QString>(nullopt) :
                            optional<QString>(lsourceData[3].toString()),
            };
            lsources.push_back(lsource);
        }

        // - dial - //
        QList<Entity> dials;
        for (auto& dialData : selectValues<1>({{"data"}}, "dial", "sense", id)) {
            dials.push_back(convertEntity(dialData[0].toString()));
        }

        // - gloss - //
        QList<Sense::Gloss> glosses;
        for (auto& glossData :
             selectValues<4>({{"lang", "g_gend", "pri", "term"}}, "gloss",
                             "sense", id)) {
            Sense::Gloss gloss {
                glossData[0].toString(),
                glossData[1].isNull() ?
                            optional<QString>(nullopt) :
                            optional<QString>(glossData[1].toString()),
                glossData[2].toBool(),
                glossData[3].toString(),
            };
            glosses.push_back(gloss);
        }

        Sense sense {
            stagks,
            stagrs,
            poses,
            xrefs,
            ants,
            fields,
            miscs,
            s_infs,
            lsources,
            dials,
            glosses,
        };
        senses.push_back(sense);
    }


    Entry entry {
        ent_seq,
        k_eles,
        r_eles,
        senses,
    };

    return entry;
}

QSet<int> Dictionary::ent_seqsFromTermInexact (const QString &term) const {
    QSet<int> ent_seqs;

    QSqlQuery k_eleQuery;
    k_eleQuery.prepare("SELECT ent_seq "
                       "FROM entry "
                       "INNER JOIN k_ele "
                       "ON k_ele.entry=entry.ent_seq "
                       "AND k_ele.keb LIKE ?");
    k_eleQuery.addBindValue(term);
    k_eleQuery.exec();
    while (k_eleQuery.next()) {
        ent_seqs.insert(k_eleQuery.value(0).toInt());
    }

    QSqlQuery r_eleQuery;
    r_eleQuery.prepare("SELECT ent_seq "
                       "FROM entry "
                       "INNER JOIN r_ele "
                       "ON r_ele.entry=entry.ent_seq "
                       "AND r_ele.reb LIKE ?");
    r_eleQuery.addBindValue(term);
    r_eleQuery.exec();
    while (r_eleQuery.next()) {
        ent_seqs.insert(r_eleQuery.value(0).toInt());
    }

    QSqlQuery senseQuery;
    senseQuery.prepare("SELECT ent_seq "
                       "FROM entry "
                       "INNER JOIN sense "
                       "ON sense.entry=entry.ent_seq "
                       "INNER JOIN gloss "
                       "ON gloss.sense=sense.id "
                       "AND gloss.term LIKE ?");
    senseQuery.addBindValue(term);
    senseQuery.exec();
    while (senseQuery.next()) {
        ent_seqs.insert(senseQuery.value(0).toInt());
    }

    return ent_seqs;
}



const Entity Dictionary::convertEntity (const QString& entity) const {

    if (entities.contains(entity)) {
        return entities[entity];
    } else {
        return Entity {
            entity,
            optional<QString>(nullopt),
            optional<QString>(nullopt),
        };
    }
}
