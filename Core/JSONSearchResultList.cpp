#include <JSONSearchResultList.h>
#include <QJsonArray>
#include <QDebug>

JSONSearchResultList::JSONSearchResultList(const QString &jsonString) :
        JSONParserAbstract(jsonString) {

}

quint64 JSONSearchResultList::resultCount() const {
    return parseKey<quint64>("resultCount");
}

QList<JSONSearchResult> JSONSearchResultList::results() const {
    QList<JSONSearchResult> lst;
        foreach(const QJsonValue &value, parseKey<QJsonArray>("results")) {
        QJsonDocument newDoc(value.toObject());
        QString strJson(newDoc.toJson(QJsonDocument::Indented));

        lst << JSONSearchResult(strJson);
    }

    return lst;
}

QDebug operator<<(QDebug debug, const JSONSearchResultList &info) {
    debug.nospace() << "resultCount: " << info.resultCount() << "\n";

    for (const auto &it : info.results()) {
        debug.nospace() << it << "\n";
    }

    return debug;
}