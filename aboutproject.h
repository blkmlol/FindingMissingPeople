#ifndef ABOUTPROJECT_H
#define ABOUTPROJECT_H

#include <QWidget>

namespace Ui {
class AboutProject;
}

class AboutProject : public QWidget
{
    Q_OBJECT

public:
    explicit AboutProject(QWidget *parent = nullptr);
    ~AboutProject();

private slots:
    void on_exitButton_clicked();

private:
    Ui::AboutProject *ui;
};

#endif // ABOUTPROJECT_H
