#include <MainWindow.h>
#include <ui_MainWindow.h>

#include <ITunesSearchEngine.h>
#include <ResultViewWidget.h>

#include <ModelInfo.h>
#include <ModelUserView.h>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
        QDialog(parent, Qt::Window),
        ui(new Ui::MainWindow) {
    Q_INIT_RESOURCE(MainWindow);

    ui->setupUi(this);

   /*
    * Prepare database for work
    */
    prepareDB();

    /*
    * App can load info from database
    */
    infoReceived();

    /*
     * Activate search function
     */
    startSearch();
}

MainWindow::~MainWindow() {

}

void MainWindow::prepareDB() {
    m_db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    m_db.setDatabaseName("temp.sqlite");
    if (m_db.open()) {
        /*
         * Force flags
         */
        QSqlQuery("PRAGMA page_size = 65535", m_db);
        QSqlQuery("PRAGMA cache_size = 65535", m_db);
        QSqlQuery("PRAGMA temp_store = MEMORY", m_db);
        QSqlQuery("PRAGMA journal_mode = OFF", m_db);
        QSqlQuery("PRAGMA synchronous = OFF", m_db);
        QSqlQuery("PRAGMA count_changes = OFF", m_db);

        /*
         * Instantiating models
         */
        ModelInfo::instanceDB(&m_db);
        ModelUserView::instanceDB(&m_db);
    }
}

void MainWindow::startSearch() {
    /*
     * Use search engine in other thread
     */
    ITunesSearchEngine *api = new ITunesSearchEngine();
    api->setMediaType(ITunesSearchEngine::music);
    api->setKeyword("Halloween");
    api->setSearchLimitSize(25);

    QThread *thread = new QThread;

    api->moveToThread(thread);

    connect(thread, &QThread::started, api, &ITunesSearchEngine::request);
    connect(api, &ITunesSearchEngine::infoReceived, thread, &QThread::quit);
    connect(api, &ITunesSearchEngine::errorOccured, thread, &QThread::quit);

    connect(api, &ITunesSearchEngine::infoReceived, api, &ITunesSearchEngine::deleteLater);
    connect(api, &ITunesSearchEngine::errorOccured, api, &ITunesSearchEngine::deleteLater);

    connect(api, &ITunesSearchEngine::infoReceived, this, &MainWindow::infoReceived);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void MainWindow::infoReceived() {
    QGridLayout *layout = new QGridLayout();
    layout->setHorizontalSpacing(6);
    layout->setVerticalSpacing(6);

    int widgetCount = 0;

    for (const auto &it : ModelUserView::instance().all()) {
        ResultViewWidget *wid = new ResultViewWidget();
        layout->addWidget(wid, widgetCount, 0);
        widgetCount++;

        wid->setTrackId(it.TRACKID);
        wid->setArtistName(it.ARTISTNAME);
        wid->setCollectionName(it.COLLECTIONNAME);
        wid->setPixmapURL(it.PIXMAPURL);
    }

    QSpacerItem *spacerV = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(spacerV, widgetCount + 1, 0);

    /*
     * Clear layout
     */
    if (ui->scrollArea->widget()->layout())
        delete ui->scrollArea->widget()->layout();

    /*
     * Set new layout with new widgets
     */
    ui->scrollArea->widget()->setLayout(layout);
}