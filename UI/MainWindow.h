#ifndef MainWindow_H
#define MainWindow_H

#include <QDialog>
#include <QSqlDatabase>

QT_FORWARD_DECLARE_CLASS(ITunesSearchEngine);

namespace Ui {
    QT_FORWARD_DECLARE_CLASS(MainWindow);
}

class MainWindow : public QDialog {
Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

private Q_SLOTS:

    void infoReceived();

private:
    /*
     * Widget UI
     */
    QScopedPointer<Ui::MainWindow> ui;

    /*
     * Temporary database
     */
    QSqlDatabase m_db;

    /*
     * Init Models
     */
    void prepareDB();

    /*
     * Activate search function
     */
    void startSearch();
};

#endif
