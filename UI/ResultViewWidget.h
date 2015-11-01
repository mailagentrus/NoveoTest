#ifndef ResultViewWidget_H
#define ResultViewWidget_H

#include <QFrame>
#include <QScopedPointer>

namespace Ui {
    QT_FORWARD_DECLARE_CLASS(ResultViewWidget);
}

QT_FORWARD_DECLARE_CLASS(QNetworkReply);

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager);

class ResultViewWidget : public QFrame {
Q_OBJECT

public:
    ResultViewWidget(QWidget *parent = 0);

    ~ResultViewWidget();

    void setArtistName(const QString &value);

    void setCollectionName(const QString &value);

    void setPixmapURL(const QString &value);

    void setTrackId(const quint64 value);

private Q_SLOTS:
    /*
     * Pixmap loaded from external source
     */
    void pixmapReceived(QNetworkReply *);

private:
    /*
     * Widget UI
     */
    QScopedPointer<Ui::ResultViewWidget> ui;

    /*
     * Network manager for loading images from external source
     */
    QScopedPointer<QNetworkAccessManager> m_networkManager;
};

#endif
