#include <QApplication>
#include "mainwindow.h"
#include "recognizeprocess.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::unique_ptr<const RecognizeProcess> rec = std::make_unique<const RecognizeProcess>();
    w.show();
    return a.exec();
}
