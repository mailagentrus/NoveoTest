#include <ITunesSearchEngine.h>
#include <ITunesSearchEngine_p.h>
#include <JSONSearchResultList.h>

#include <ModelInfo.h>
#include <ModelUserView.h>

ITunesSearchEnginePrivate::ITunesSearchEnginePrivate() :
        m_lastReplyId(0),
        m_apiURL("https://itunes.apple.com/search"),
        m_apiVersion(2),
        m_searchLimitSize(50),
        m_mediaType(ITunesSearchEngine::all) {

}

ITunesSearchEnginePrivate::~ITunesSearchEnginePrivate() {

}

void ITunesSearchEnginePrivate::init() {
    Q_Q(ITunesSearchEngine);
    m_conManager.reset(new QNetworkAccessManager(q));
    QObject::connect(m_conManager.data(), SIGNAL(finished(QNetworkReply * )), q,
                     SLOT(networkReplyFinished(QNetworkReply * )), Qt::QueuedConnection);
}

void ITunesSearchEnginePrivate::requestFinished(int requestNumber, QNetworkReply *rply) {
    Q_Q(ITunesSearchEngine);

    QString response = QString(rply->readAll());

    if (rply->error() != QNetworkReply::NoError) {
        emit q->errorOccured();
        return;
    }

    // ignore connection requests
    if (m_requestMap[requestNumber].type == REQ_CONNECT)
        return;

    if (rply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 302) {
        // redirection handling
        QUrl newlocation(rply->header(QNetworkRequest::LocationHeader).toString(), QUrl::StrictMode);

        int oldRequestNumber = requestNumber;
        requestNumber = sendRequest(newlocation, m_requestMap[requestNumber].method, 0,
                                    m_requestMap[requestNumber].host);
        m_requestMap[requestNumber].type = REQ_REDIREC;
        m_requestMap[requestNumber].linked = oldRequestNumber;
        return;
    }
    else {
        if (m_requestMap[requestNumber].type == REQ_REDIREC) {
            // change values if this is the answert to a redirect
            ITunesAPIRequest redir = m_requestMap[requestNumber];
            ITunesAPIRequest orig = m_requestMap[redir.linked];
            m_requestMap[requestNumber] = orig;
            requestNumber = redir.linked;
        }

        // standard handling depending on message type
        switch (m_requestMap[requestNumber].type) {
            case REQ_CONNECT:
                // was only a connect request - so drop it
                break;
            case REQ_SEARCH_BY_KEYWORDS:
                parseResponse(rply->url().toString(), response);
                break;
            default:
                emit q->errorOccured();
                break;
        }
    }
}

void ITunesSearchEnginePrivate::parseResponse(const QString &query, const QString &response) {
    Q_Q(ITunesSearchEngine);

    JSONSearchResultList parser(response);

    // Check response for valid
    if (!parser.isValid()) {
        emit q->errorOccured();
        return;
    }

    auto data = ModelInfo::instance().getRecord(query);
    if (data == Q_NULLPTR) {
        ModelInfo::instance().addUpdate(0, query, QDate::currentDate(), response);
    }
    else {
        data->RESPONSE = response;
        ModelInfo::instance().save(data);
    }

    ModelUserView::instance().clear();

    for (const auto &it : parser.results())
        ModelUserView::instance().addUpdate(0, it.artistName(), it.collectionName(), it.artworkUrl100().toString(),
                                            it.trackId());

    emit q->infoReceived();
}

void ITunesSearchEnginePrivate::networkReplyFinished(QNetworkReply *rply) {
    if (m_replyNumberMap.contains(rply)) {
        int reqnr = m_replyNumberMap[rply];
        requestFinished(reqnr, rply);
        m_replyNumberMap.remove(rply);
    }

    rply->deleteLater(); // release memory
}

int ITunesSearchEnginePrivate::sendRequest(QUrl request, QString type, QByteArray postdata, QString host) {
    Q_Q(ITunesSearchEngine);

    if (!host.trimmed().compare(""))
        host = m_apiURL;

    QString req_str = request.toString(QUrl::RemoveAuthority | QUrl::RemoveScheme);
    if (!req_str.startsWith("/"))
        req_str = QString("/%1").arg(req_str);

    QNetworkRequest rq(request);
    QNetworkReply *rply;

    if (type == QString("GET"))
        rply = m_conManager->get(rq);
    else if (type == QString("POST")) {
        rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        rply = m_conManager->post(rq, postdata);
    }
    else
        return -1;

    m_replyNumberMap[rply] = ++m_lastReplyId;

    m_requestMap[m_lastReplyId].method = type;
    m_requestMap[m_lastReplyId].host = host;

    return m_lastReplyId;
}

void ITunesSearchEnginePrivate::fillTermQuery(QUrlQuery &query) {
    const int listSize = m_keywordList.size();

    QString termQuery;
    for (auto it = m_keywordList.cbegin(); it != m_keywordList.cend(); ++it) {
        termQuery += "\"" + *it + "\"";
        if (listSize > 1 && (it != m_keywordList.cend() - 1))
            termQuery += "+";
    }

    query.addQueryItem("term", termQuery);
}

void ITunesSearchEnginePrivate::fillLimitQuery(QUrlQuery &query) {
    query.addQueryItem("limit", QString::number(m_searchLimitSize));
}

void ITunesSearchEnginePrivate::fillMediaTypeQuery(QUrlQuery &query) {
    QString mediaTypeStr;
    switch (m_mediaType) {
        case ITunesSearchEngine::movie:
            mediaTypeStr = "movie";
            break;
        case ITunesSearchEngine::podcast:
            mediaTypeStr = "podcast";
            break;
        case ITunesSearchEngine::music:
            mediaTypeStr = "music";
            break;
        case ITunesSearchEngine::musicVideo:
            mediaTypeStr = "musicVideo";
            break;
        case ITunesSearchEngine::audiobook:
            mediaTypeStr = "audiobook";
            break;
        case ITunesSearchEngine::shortFilm:
            mediaTypeStr = "shortFilm";
            break;
        case ITunesSearchEngine::tvShow:
            mediaTypeStr = "tvShow";
            break;
        case ITunesSearchEngine::software:
            mediaTypeStr = "software";
            break;
        case ITunesSearchEngine::ebook:
            mediaTypeStr = "ebook";
            break;
        case ITunesSearchEngine::all:
            mediaTypeStr = "all";
            break;
        default:
            return;
            break;
    }

    query.addQueryItem("media", mediaTypeStr);
}

void ITunesSearchEnginePrivate::fillApiVersionQuery(QUrlQuery &query) {
    query.addQueryItem("version", QString::number(m_apiVersion));
}

void ITunesSearchEnginePrivate::request() {
    QUrl url(m_apiURL);
    QUrlQuery resultUrlQuery;

    fillApiVersionQuery(resultUrlQuery);
    fillTermQuery(resultUrlQuery);
    fillLimitQuery(resultUrlQuery);
    fillMediaTypeQuery(resultUrlQuery);

    // Finalize query
    url.setQuery(resultUrlQuery);

    int reqnr = sendRequest(url);
    m_requestMap[reqnr].type = REQ_SEARCH_BY_KEYWORDS;

    auto data = ModelInfo::instance().getRecord(url.toString());
    if (data == Q_NULLPTR) {
        ModelInfo::instance().addUpdate(0, url.toString());
        return;
    }

    data->RESPONSE.clear();
    ModelInfo::instance().save(data);
}


ITunesSearchEngine::ITunesSearchEngine(QObject *parent) :
        QObject(parent),
        d_ptr(new ITunesSearchEnginePrivate()) {
    Q_D(ITunesSearchEngine);
    d->q_ptr = this;

    d->init();
}

ITunesSearchEngine::ITunesSearchEngine(ITunesSearchEnginePrivate &dd)
        : d_ptr(&dd) {
    Q_D(ITunesSearchEngine);
    d->q_ptr = this;
}

#ifdef Q_COMPILER_RVALUE_REFS

ITunesSearchEngine::ITunesSearchEngine(ITunesSearchEngine &&other) {
    swap(*this, other);
}

#endif

ITunesSearchEngine::~ITunesSearchEngine() {

}

void ITunesSearchEngine::setApiUrl(const QString &url) {
    Q_D(ITunesSearchEngine);
    d->m_apiURL = url;
}

QString ITunesSearchEngine::apiUrl() const {
    Q_D(const ITunesSearchEngine);
    return d->m_apiURL;
}

void ITunesSearchEngine::setApiVersion(const int apiversion) {
    Q_D(ITunesSearchEngine);
    d->m_apiVersion = apiversion;
}

int ITunesSearchEngine::apiVersion() const {
    Q_D(const ITunesSearchEngine);
    return d->m_apiVersion;
}

void ITunesSearchEngine::setSearchLimitSize(const int limitSize) {
    Q_D(ITunesSearchEngine);

    if (limitSize > 0)
        d->m_searchLimitSize = limitSize;
}

int ITunesSearchEngine::searchLimitSize() const {
    Q_D(const ITunesSearchEngine);

    return d->m_searchLimitSize;
}

void ITunesSearchEngine::setMediaType(const MediaType type) {
    Q_D(ITunesSearchEngine);

    d->m_mediaType = type;
}

ITunesSearchEngine::MediaType ITunesSearchEngine::mediaType() const {
    Q_D(const ITunesSearchEngine);

    return d->m_mediaType;
}

void ITunesSearchEngine::setKeyword(const QStringList &keywordList) {
    Q_D(ITunesSearchEngine);

    d->m_keywordList = keywordList;
}

QStringList ITunesSearchEngine::keywordList() const {
    Q_D(const ITunesSearchEngine);

    return d->m_keywordList;
}

void ITunesSearchEngine::request() {
    Q_D(ITunesSearchEngine);
    d->request();
}

#include "moc_ITunesSearchEngine.cpp"