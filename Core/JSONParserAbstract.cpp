#include <JSONParserAbstract.h>
#include <QJsonArray>
#include <QString>
#include <QLocale>
#include <QUrl>
#include <QDateTime>

JSONParserAbstract::JSONParserAbstract(const QString &jsonString) {
    setJSONString(jsonString);
}

void JSONParserAbstract::setJSONString(const QString &jsonString) {
    m_doc = QJsonDocument::fromJson(jsonString.toUtf8());
}

QString JSONParserAbstract::strContent() const {
    return m_doc.toJson(QJsonDocument::Indented);
}

bool JSONParserAbstract::isValid() const {
    return !m_doc.isNull();
}

void JSONParserAbstract::clear() {
    m_doc = QJsonDocument();
}

template<>
QString JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toString();
}

template<>
QUrl JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return QUrl(jsonObject.value(key).toString(), QUrl::StrictMode);
}

template<>
qint64 JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toVariant().toLongLong();
}

template<>
quint64 JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toVariant().toULongLong();
}

template<>
bool JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toBool();
}

template<>
QJsonArray JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toArray();
}

template<>
QJsonObject JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    return jsonObject.value(key).toObject();
}

template<>
QDateTime JSONParserAbstract::parseKey(const QJsonObject &jsonObject, const QString &key) const {
    /*
     * Based on information from ITunes
     */
    const QString dtFormat = "yyyy-MM-ddThh:mm:ssZ";

    QString str = jsonObject.value(key).toString();
    QDateTime res = QDateTime::fromString(str, dtFormat);

    return res;
}