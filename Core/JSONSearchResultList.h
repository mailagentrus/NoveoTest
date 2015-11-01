#ifndef JSONSearchResultList_H
#define JSONSearchResultList_H

#include <JSONParserAbstract.h>
#include <JSONSearchResult.h>
#include <QString>
#include <QUrl>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QDebug)

class JSONSearchResultList : public JSONParserAbstract {
public:
    /*
     * This constructor creates an object based on the data contained in the
     * given string that is in valid JSON format.
     */
    explicit JSONSearchResultList(const QString &jsonString);

    /*
    * Returns the result count.
    */
    quint64 resultCount() const;

    /*
    * Returns the result list.
    */
    QList<JSONSearchResult> results() const;
};

QDebug operator<<(QDebug debug, const JSONSearchResultList &info);

#endif // JSONSearchResultList_H
