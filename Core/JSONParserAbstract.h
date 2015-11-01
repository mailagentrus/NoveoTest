#ifndef JSONParserAbstract_H
#define JSONParserAbstract_H

#include <QJsonDocument>
#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QString)

class JSONParserAbstract {
public:
    /*
     * This constructor creates an object based on the data contained in the
     * given string that is in valid JSON format.
     */
    explicit JSONParserAbstract(const QString &jsonString);

    void setJSONString(const QString &jsonString);

    /*
     * Return string in JSON format
     */
    QString strContent() const;

    /*
     * Return true, if JSON data is valid
     */
    bool isValid() const;

    /*
     * Clear internal data
     */
    void clear();

protected:
    QJsonDocument m_doc;

    template<typename T>
    T parseKey(const char *key) const {
        const QJsonObject &jsonObject = m_doc.object();
        return parseKey<T>(jsonObject, key);
    }

    template<typename T>
    T parseObjectKey(const QJsonObject &jsonObject, const char *key) const {
        return parseKey<T>(jsonObject, key);
    }

    template<typename T>
    T parseKey(const QJsonObject &jsonObject, const QString &key) const;
};

#endif // JSONParserAbstract_H
