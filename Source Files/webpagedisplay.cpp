#include "webpagedisplay.h"
#include "ui_webpagedisplay.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include <QRegularExpression>

WebpageDisplay::WebpageDisplay(QWidget *parent, QWidget *mainWindow, QWidget *rootWindow)
    : QDialog(parent)
    , ui(new Ui::WebpageDisplay)
    , mainWindow(mainWindow)
    , parentWindow(parent)
    , rootWindow(rootWindow) // Initialize root window
{
    ui->setupUi(this);

    connect(ui->backButton, &QPushButton::clicked, this, &WebpageDisplay::on_backButton_clicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &WebpageDisplay::on_forwardButton_clicked);
    connect(ui->homeButton, &QPushButton::clicked, this, &WebpageDisplay::on_homeButton_clicked);
}



WebpageDisplay::~WebpageDisplay()
{
    delete ui;
}

void WebpageDisplay::setWebPageContent(const QString &content) {
    QString formattedContent = content;

    QRegularExpression fileRegex("\\b(\\w+\\.txt)\\b"); // Regular expression to match filenames ending with .txt
    formattedContent.replace(fileRegex, "<a href=\"\\1\" style=\"color:blue; text-decoration:underline;\">\\1</a>");

    ui->textBrowser->setHtml(formattedContent);

    if (homePage.isEmpty()) {
        homePage = formattedContent;
    }
}

void WebpageDisplay::setDocumentName(const QString &docName) {
    ui->docNameLabel->setText(docName);
}

void WebpageDisplay::on_backButton_clicked() {
    if (!navigationHistory.isEmpty()) {
        QString previousContent = navigationHistory.pop();
        QString previousDocumentName = documentNames.pop();

        // Push the current state to the forward stack
        forwardHistory.push(ui->textBrowser->toHtml());
        forwardDocumentNames.push(ui->docNameLabel->text());

        this->setWebPageContent(previousContent);
        this->setDocumentName(previousDocumentName);
    } else {
        qDebug() << "No back history available";
    }
    qDebug() << "Back button clicked";
}

void WebpageDisplay::on_forwardButton_clicked() {
    if (!forwardHistory.isEmpty()) {
        QString nextContent = forwardHistory.pop();
        QString nextDocumentName = forwardDocumentNames.pop();

        // Push the current state to the back stack
        navigationHistory.push(ui->textBrowser->toHtml());
        documentNames.push(ui->docNameLabel->text());

        this->setWebPageContent(nextContent);
        this->setDocumentName(nextDocumentName);
    } else {
        qDebug() << "No forward history available";
    }
    qDebug() << "Forward button clicked";
}

void WebpageDisplay::on_homeButton_clicked() {
    this->close();
    if (rootWindow) {
        rootWindow->show();
    } else {
        qDebug() << "Root window is not available";
    }
    qDebug() << "Home button clicked";
}

void WebpageDisplay::on_textBrowser_anchorClicked(const QUrl &url) {
    QString filePath = "C:\\Users\\rijaa\\Desktop\\Sem 4 to be sorted\\projectnew\\projectnew\\" + url.toString();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not open file: ") + filePath);
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Save the current content and document name to the history stack
    navigationHistory.push(ui->textBrowser->toHtml());
    documentNames.push(ui->docNameLabel->text());

    // Clear the forward history since we're navigating to a new page
    forwardHistory.clear();
    forwardDocumentNames.clear();

    // Update the current window with the new content
    QString docName = url.toString();
    if (docName.endsWith(".txt", Qt::CaseInsensitive)) {
        docName.chop(4); // Remove the last 4 characters (".txt")
    }
    this->setDocumentName(docName);
    this->setWebPageContent(content);
}
