#include <ResultViewWidget.h>
#include <ui_ResultViewWidget.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtWidgets>

ResultViewWidget::ResultViewWidget(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::ResultViewWidget),
        m_networkManager(new QNetworkAccessManager()) {
    ui->setupUi(this);

    connect(m_networkManager.data(), &QNetworkAccessManager::finished, this, &ResultViewWidget::pixmapReceived);
}

ResultViewWidget::~ResultViewWidget() {

}

void ResultViewWidget::setArtistName(const QString &value) {
    ui->artistTextEdit->setPlainText(value);
}

void ResultViewWidget::setCollectionName(const QString &value) {
    ui->collectionTextEdit->setPlainText(value);
}

void ResultViewWidget::setPixmapURL(const QString &value) {
    /*
     * Send GET request to obtain the desired picture
     */
    m_networkManager->get(QNetworkRequest(value));
}

void ResultViewWidget::setTrackId(const quint64 value) {
    ui->trackIdlabel->setText(QString::number(value));
}

void ResultViewWidget::pixmapReceived(QNetworkReply *reply) {
    if (QNetworkReply::NoError != reply->error()) {
        reply->deleteLater();
        return;
    }

    const QByteArray data(reply->readAll());
    if (data.isEmpty())
        return;

    QPixmap pixmap;
    pixmap.loadFromData(data);
    ui->pixmapLabel->setPixmap(pixmap);

    reply->deleteLater();
}