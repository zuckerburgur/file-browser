#include "graphwindow.h"
#include "ui_graphwindow.h"

graphWindow::graphWindow(searchEngine* engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphWindow),
    sEngine(engine)
{
    ui->setupUi(this);
    QPixmap pix("purple.jpg");
    ui->bg->setPixmap(pix);
    string graph = sEngine->displayGraph();
    ui->textBrowser->setText(QString::fromStdString(graph));

}

graphWindow::~graphWindow()
{
    delete ui;
}

void graphWindow::on_displayGraphButton_clicked()
{
    sEngine->displayGraph();
    string graph = sEngine->displayGraph();
    ui->textBrowser->setText(QString::fromStdString(graph));
}

void graphWindow::on_findPathButton_clicked()
{
    QString source = ui->sourceInput->text();
    QString destination = ui->destinationInput->text();

    if (source.isEmpty() || destination.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both source and destination.");
        return;
    }
    string path;
    path = sEngine->dijkstra(source.toStdString(), destination.toStdString());

    ui->pathLabel->setText(QString::fromStdString(path));

}
