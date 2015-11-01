#ifndef JSONSearchResult_H
#define JSONSearchResult_H

#include <JSONParserAbstract.h>
#include <QString>
#include <QUrl>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QDebug)

class JSONSearchResult : public JSONParserAbstract {
public:
    /*
     * This constructor creates an object based on the data contained in the
     * given string that is in valid JSON format.
     */
    explicit JSONSearchResult(const QString &jsonString);

    /*
    * Returns the wrapper type.
    */
    QString wrapperType() const;

    /*
    Returns the kind type.
    */
    QString kind() const;

    /*
     * Returns the artist id.
     */
    quint64 artistId() const;

    /*
     * Returns the collection id.
     */
    quint64 collectionId() const;

    /*
     * Returns the track id.
     */
    quint64 trackId() const;

    /*
     * Returns the collection artist id.
     */
    quint64 collectionArtistId() const;

    /*
     * Returns the artist name.
     */
    QString artistName() const;

    /*
     * Returns the collection name.
     */
    QString collectionName() const;

    /*
     * Returns the track name.
     */
    QString trackName() const;

    /*
     * Returns the collection censored name.
     */
    QString collectionCensoredName() const;

    /*
     * Returns the track censored name.
     */
    QString trackCensoredName() const;

    /*
     * Returns the artist view url.
     */
    QUrl artistViewUrl() const;

    /*
     * Returns the collection view url.
     */
    QUrl collectionViewUrl() const;

    /*
     * Returns the track view url.
     */
    QUrl trackViewUrl() const;

    /*
     * Returns the preview url.
     */
    QUrl previewUrl() const;

    /*
     * Returns the artwork url.
     */
    QUrl artworkUrl30() const;

    /*
     * Returns the artwork url.
     */
    QUrl artworkUrl60() const;

    /*
     * Returns the artwork url.
     */
    QUrl artworkUrl100() const;

    /*
     * Returns is streamable flag.
     */
    bool isStreamable() const;

    /*
     *Returns the country.
     */
    QString country() const;

    /*
     * Returns the release date.
     */
    QDateTime releaseDate() const;
};

QDebug operator<<(QDebug debug, const JSONSearchResult &info);

#endif // JSONSearchResult_H
