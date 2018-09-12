#include "jmdict_sense.h"

using namespace JMdict;

Sense::Xref JMdict::parseXref (const QString& str) {
    QStringList xrefParts = str.split("・");

    if (xrefParts.size() == 3) {
        // _・_・_ => 漢・あ・# => keb, reb, senseNum
        Sense::Xref xref {
            xrefParts[0],
            xrefParts[1],
            xrefParts[2].toInt(),
        };
        return xref;
    } else if (xrefParts.size() == 2) {
        // _・_ => ??? => keb, reb | keb, senseNum | reb, senseNum

        if (isNumber(xrefParts[1])) {
            // 漢|あ・# => ??? => keb, senseNum | reb, senseNum
            if (isReb(xrefParts[0])) {
                // あ・# => reb, senseNum
                Sense::Xref xref {
                    std::experimental::nullopt,
                    xrefParts[0],
                    xrefParts[1].toInt(),
                };
                return xref;
            } else {
                // 漢・# => keb, senseNum
                Sense::Xref xref {
                    xrefParts[0],
                    std::experimental::nullopt,
                    xrefParts[1].toInt(),
                };
                return xref;
            }
        } else {
            // 漢・あ => keb, reb
            Sense::Xref xref {
                xrefParts[0],
                xrefParts[1],
                std::experimental::nullopt,
            };
            return xref;
        }
    } else { // xrefParts.size() == 1
        // _ => ??? => keb | reb
        if (isReb(xrefParts[0])) {
            // あ => reb
            Sense::Xref xref {
                std::experimental::nullopt,
                xrefParts[0],
                std::experimental::nullopt,
            };
            return xref;
        } else {
            // 漢 => keb
            Sense::Xref xref {
                xrefParts[0],
                std::experimental::nullopt,
                std::experimental::nullopt,
            };
            return xref;
        }
    }
}

bool JMdict::isReb (const QString& str) {
    QRegExp re("^[\\u3040-\\u309f\\u30a0-\\u30ff\\uff00-\\uff9f]+$");
    return re.exactMatch(str);
}

bool JMdict::isNumber (const QString& str) {
    bool isNumeric = false;
    str.toInt(&isNumeric, 10);
    return isNumeric;
}
