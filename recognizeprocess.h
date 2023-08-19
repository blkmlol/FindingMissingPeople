#ifndef RECOGNIZEPROCESS_H
#define RECOGNIZEPROCESS_H

#include <QProcess>
#include <QFile>
#include <filesystem>

class RecognizeProcess
{
private:
    QProcess process;
    std::filesystem::path exePath;
    std::filesystem::path workingDir;
    std::unique_ptr<QFile> stopper;

public:
    RecognizeProcess();
    ~RecognizeProcess();

    const QProcess& getProcess() { return process; }
    const QFile& getStopper() { return *stopper; }
    bool CreateStopper();

    RecognizeProcess(const RecognizeProcess&) = delete;
    RecognizeProcess& operator=(const RecognizeProcess&) = delete;
};

#endif // RECOGNIZEPROCESS_H
