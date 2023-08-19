#include "recognizeprocess.h"
#include <QDebug>

RecognizeProcess::RecognizeProcess()
    : process{QProcess()}
    , exePath{std::filesystem::current_path() / "recognizer.exe"}
    , workingDir{std::filesystem::current_path()}
    , stopper{std::make_unique<QFile>(QString::fromStdString((workingDir / "stopper.stp").generic_string()))}
{
    if (!CreateStopper()) {
        throw std::runtime_error("Failed to create stopper file");
    }
    process.setWorkingDirectory(QString::fromStdWString(workingDir.wstring()));
    process.setProgram(QString::fromStdString(exePath.generic_string()));
    process.start();
}

RecognizeProcess::~RecognizeProcess()
{
    stopper->remove();
    process.terminate();
    process.waitForFinished();
}

bool RecognizeProcess::CreateStopper()
{
    if (!stopper->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create stopper file";
        return false;
    }
    stopper->close();
    return true;
}
