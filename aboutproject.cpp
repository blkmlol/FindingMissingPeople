#include "aboutproject.h"
#include "ui_aboutproject.h"

AboutProject::AboutProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutProject)
{
    ui->setupUi(this);
}

AboutProject::~AboutProject()
{
    delete ui;
}

void AboutProject::on_exitButton_clicked()
{
    this->close();
}

