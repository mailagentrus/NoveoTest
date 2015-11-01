#ifndef ITunesSearchEngine_H
#define ITunesSearchEngine_H

#include <QObject>
#include <QString>
#include <QStringList>

QT_FORWARD_DECLARE_CLASS(QNetworkReply);

QT_FORWARD_DECLARE_CLASS(ITunesSearchEnginePrivate);

class ITunesSearchEngine : public QObject {
Q_OBJECT

    Q_ENUMS(MediaType)

public:
    enum MediaType {
        movie, podcast, music, musicVideo, audiobook, shortFilm, tvShow, software, ebook, all,
    };

    explicit ITunesSearchEngine(QObject *parent = 0);

    ~ITunesSearchEngine();

    inline void swap(ITunesSearchEngine &first, ITunesSearchEngine &second) /* nothrow */ {
        std::swap(first.d_ptr, second.d_ptr);
    }

    void setApiUrl(const QString &url);

    QString apiUrl() const;

    void setApiVersion(const int apiversion);

    int apiVersion() const;

    void setSearchLimitSize(const int limitSize);

    int searchLimitSize() const;

    void setMediaType(const MediaType type);

    MediaType mediaType() const;

    inline void setKeyword(const QString &keyword) {
        QStringList lst;
        lst << keyword;
        setKeyword(lst);
    }

    void setKeyword(const QStringList &keywordList);

    QStringList keywordList() const;

public Q_SLOTS:

    void request();

Q_SIGNALS:

    /*!
    This signal is emitted whenever an error occurs.

    \param errorcode The occured error.
    */
    void errorOccured();

    /*!
      Emitted when search information was received.
     */
    void infoReceived();

protected:
    QScopedPointer<ITunesSearchEnginePrivate> d_ptr;

    ITunesSearchEngine(ITunesSearchEnginePrivate &dd);

#ifdef Q_COMPILER_RVALUE_REFS

    ITunesSearchEngine(ITunesSearchEngine &&other);

#endif

private:
    Q_DECLARE_PRIVATE(ITunesSearchEngine);
    Q_DISABLE_COPY(ITunesSearchEngine);

    Q_PRIVATE_SLOT(d_func(), void requestFinished(int requestNumber, QNetworkReply *rply));
    Q_PRIVATE_SLOT(d_func(), void networkReplyFinished(QNetworkReply *rply));
};

#endif // SearchApi_H
