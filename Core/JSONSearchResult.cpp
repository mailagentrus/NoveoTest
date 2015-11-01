#include <JSONSearchResult.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

JSONSearchResult::JSONSearchResult(const QString &jsonString) :
        JSONParserAbstract(jsonString) {

}

QString  JSONSearchResult::wrapperType() const {
    return parseKey<QString>("wrapperType");
}

QString JSONSearchResult::kind() const {
    return parseKey<QString>("kind");
}

quint64 JSONSearchResult::artistId() const {
    return parseKey<quint64>("artistId");
}

quint64 JSONSearchResult::collectionId() const {
    return parseKey<quint64>("collectionId");
}

quint64 JSONSearchResult::trackId() const {
    return parseKey<quint64>("trackId");
}

quint64 JSONSearchResult::collectionArtistId() const {
    return parseKey<quint64>("collectionArtistId");
}

QString JSONSearchResult::artistName() const {
    return parseKey<QString>("artistName");
}

QString JSONSearchResult::collectionName() const {
    return parseKey<QString>("collectionName");
}

QString JSONSearchResult::trackName() const {
    return parseKey<QString>("trackName");
}

QString JSONSearchResult::collectionCensoredName() const {
    return parseKey<QString>("collectionCensoredName");
}

QString JSONSearchResult::trackCensoredName() const {
    return parseKey<QString>("trackCensoredName");
}

QUrl JSONSearchResult::artistViewUrl() const {
    return parseKey<QUrl>("artistViewUrl");
}

QUrl JSONSearchResult::collectionViewUrl() const {
    return parseKey<QUrl>("collectionViewUrl");
}

QUrl JSONSearchResult::trackViewUrl() const {
    return parseKey<QUrl>("trackViewUrl");
}

QUrl JSONSearchResult::previewUrl() const {
    return parseKey<QUrl>("previewUrl");
}

QUrl JSONSearchResult::artworkUrl30() const {
    return parseKey<QUrl>("artworkUrl30");
}

QUrl JSONSearchResult::artworkUrl60() const {
    return parseKey<QUrl>("artworkUrl60");
}

QUrl JSONSearchResult::artworkUrl100() const {
    return parseKey<QUrl>("artworkUrl100");
}

bool JSONSearchResult::isStreamable() const {
    return parseKey<bool>("isStreamable");
}

QString JSONSearchResult::country() const {
    return parseKey<QString>("country");
}

QDateTime JSONSearchResult::releaseDate() const {
    return parseKey<QDateTime>("releaseDate");
}

QDebug operator<<(QDebug debug, const JSONSearchResult &info) {
    debug.nospace() << "isStreamable: " << info.isStreamable() << "\n";
    debug.nospace() << "previewUrl: " << info.previewUrl() << "\n";
    debug.nospace() << "country: " << info.country() << "\n";
    debug.nospace() << "kind: " << info.kind() << "\n";
    debug.nospace() << "wrapperType: " << info.wrapperType() << "\n";
    debug.nospace() << "releaseDate: " << info.releaseDate().toString("dd.MM.yyyy hh:mm:ss") << "\n";

    debug.nospace() << "artistId: " << info.artistId() << "\n";
    debug.nospace() << "artistName: " << info.artistName() << "\n";
    debug.nospace() << "artistViewUrl: " << info.artistViewUrl() << "\n";
    debug.nospace() << "artworkUrl100: " << info.artworkUrl100() << "\n";
    debug.nospace() << "artworkUrl30: " << info.artworkUrl30() << "\n";
    debug.nospace() << "artworkUrl60: " << info.artworkUrl60() << "\n";
    debug.nospace() << "collectionId: " << info.collectionId() << "\n";
    debug.nospace() << "collectionName: " << info.collectionName() << "\n";
    debug.nospace() << "collectionCensoredName: " << info.collectionCensoredName() << "\n";
    debug.nospace() << "collectionViewUrl: " << info.collectionViewUrl() << "\n";

    debug.nospace() << "collectionArtistId: " << info.collectionArtistId() << "\n";

    debug.nospace() << "trackId: " << info.trackId() << "\n";
    debug.nospace() << "trackName: " << info.trackName() << "\n";
    debug.nospace() << "trackCensoredName: " << info.trackCensoredName() << "\n";
    debug.nospace() << "trackViewUrl: " << info.trackViewUrl() << "\n";
    debug.nospace() << "\n";

    return debug;
}