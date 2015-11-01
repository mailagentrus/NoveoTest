#include <QApplication>
#include <QTextCodec>

#include <MainWindow.h>

int main(int argc, char *argv[]) {
    /*
     * Set UTF-8 codec for application
     */
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);

    /*
     * Crete main window
     */
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    mainWindow->exec();

    delete mainWindow;

    a.exit(0);

    return 0;
}
