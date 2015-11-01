#ifndef ITunesSearchEnginePrivate_H
#define ITunesSearchEnginePrivate_H

#include <ITunesSearchEngine.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include <QUrl>
#include <QUrlQuery>

enum RequestType {
    REQ_CONNECT,
    REQ_REDIREC,
    REQ_SEARCH_BY_KEYWORDS,
};

/*
 * Internally used struct to handle network requests sent from ITunesSearchEngine
 * This structure is used internally by ITunesSearchEngine. It is used to connect network
 * requests that are sent to the ITunes API server with the asynchronous queries
 * made to the ITunesSearchEngine.
*/
struct ITunesAPIRequest {
    RequestType type; // Type of the request
    QString method;   // Used method to send the request (POST/GET)
    QString host;     // Host that received the request
    int linked;       // ID of any linked request (for forwarded requests)
};

class ITunesSearchEnginePrivate {
    Q_DECLARE_PUBLIC(ITunesSearchEngine);
    ITunesSearchEngine *q_ptr;

public:
    ITunesSearchEnginePrivate();

    virtual ~ITunesSearchEnginePrivate();

    /*
     * Init and connect signal/slots
     */
    void init();

    /*
     * Request from queue with specified number is finished
     */
    void requestFinished(int requestNumber, QNetworkReply *rply);

    void networkReplyFinished(QNetworkReply *rply);

    /*
     * Data
     */
    int m_lastReplyId;
    QString m_apiURL;
    int m_apiVersion;
    int m_searchLimitSize;
    ITunesSearchEngine::MediaType m_mediaType;
    QStringList m_keywordList;

    /*
     * Connection manager
     */
    QSharedPointer<QNetworkAccessManager> m_conManager;

    QMap<QNetworkReply *, int> m_replyNumberMap;
    QMap<int, ITunesAPIRequest> m_requestMap;

    /*
     * Fill QUrlQuery with specified param
     */
    void fillTermQuery(QUrlQuery &query);

    void fillLimitQuery(QUrlQuery &query);

    void fillMediaTypeQuery(QUrlQuery &query);

    void fillApiVersionQuery(QUrlQuery &query);

    /*
     * Send request with specified type
     */
    int sendRequest(QUrl request,
                    QString type = QString("GET"),
                    QByteArray postdata = QByteArray(),
                    QString host = QString());

    /*
     * Parse response from server in JSON format
     */
    void parseResponse(const QString &query, const QString &response);

    /*
     * Request
     */
    void request();
};

#endif // ITunesSearchEnginePrivate_H
