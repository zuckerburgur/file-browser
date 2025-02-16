#include "errorwindow.h"
#include "ui_errorwindow.h"
#include <QDebug>

ErrorWindow::ErrorWindow(QWidget *parent, QWidget *mainWindow) :
    QDialog(parent),
    ui(new Ui::ErrorWindow),
    mainWindow(mainWindow),
    parentWindow(parent)
{
    ui->setupUi(this);

    // Connect navigation buttons to their respective slots
    connect(ui->backButton1, &QPushButton::clicked, this, &ErrorWindow::onBackButtonClicked);
    connect(ui->forwardButton1, &QPushButton::clicked, this, &ErrorWindow::onForwardButtonClicked);
    connect(ui->homeButton1, &QPushButton::clicked, this, &ErrorWindow::onHomeButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::close);

}



ErrorWindow::~ErrorWindow()
{
    delete ui;
}


void ErrorWindow::searchError(const QString &errorText)
{
    ui->urlText1->setText(errorText);
}

void ErrorWindow::on_closeButton_clicked()
{
    // Close the dialog
    close();
}

void ErrorWindow::onBackButtonClicked()
{
    // Close this window and show the parent window
    this->close();
    if (parentWindow) {
        parentWindow->show();
    } else {
        qDebug() << "Parent window is not available";
    }
    qDebug() << "Back button clicked";
}

void ErrorWindow::onHomeButtonClicked()
{
    // Close this window and go back to the main window
    this->close();
    if (mainWindow) {
        mainWindow->show();
    } else {
        qDebug() << "Main window is not available";
    }
    qDebug() << "Home button clicked";
}

void ErrorWindow::onForwardButtonClicked()
{
    // Placeholder for forward button functionality
    qDebug() << "Forward button clicked";
}
