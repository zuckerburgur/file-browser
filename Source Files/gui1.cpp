#include "gui1.h"
#include "./ui_gui1.h"
#include "webpagedisplay.h"
#include "querywindow.h"
#include "errorwindow.h"
#include "searchengine.h"  // Include the search engine header
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>
#include "graphwindow.h"


GUI1::GUI1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI1)
{
    ui->setupUi(this);
    setFixedSize(size());
    QPixmap pix("purple.jpg");
    ui->label->setPixmap(pix);
    s1.store(); // Initialize the search engine by storing documents
    qDebug() << "GUI1 initialized and search engine stored documents";

}

GUI1::~GUI1()
{
    delete ui;
}

void GUI1::on_refreshButton_clicked()
{
    this->refreshItems1();
}

void GUI1::refreshItems1()
{
    ui->searchText1->setText("");
    ui->urlText1->setText("");
    ui->searchText1->setPlaceholderText("");
    ui->urlText1->setPlaceholderText("");
}



void GUI1::on_searchButton1_clicked()
{
    QString searchText = ui->searchText1->text();
    if (searchText.isEmpty()) {
        ui->searchText1->setPlaceholderText("Please enter text to search for.");
        return;
    }

    // Use the search engine to check if the file exists and read its contents
    string fileContents;
    //searchText = searchText.toUpper();
    if (s1.searchFileByName(searchText.toStdString() + ".txt", fileContents)) {
        cout<<"\nFile found";
        // File exists, display contents in the WebpageDisplay window
        WebpageDisplay *webPageWin = new WebpageDisplay(this, this, this);
        webPageWin->setDocumentName(searchText);  // Set document name as heading
        webPageWin->setWebPageContent(QString::fromStdString(fileContents));
        webPageWin->exec();
    } else {
        cout<<"\nFile not found";
        // File does not exist, show error
        emit searchInpError(ui->searchText1->text());
        emit showError();
    }
}


void GUI1::on_goButton1_clicked()
{
    QString queryText = ui->urlText1->text();
    if (queryText.isEmpty()) {
        ui->urlText1->setPlaceholderText("Please enter text to search for.");
        return;
    }
    // Use the search engine to search for the query
    s1.searchQuery(queryText.toStdString());

    // Check and display the results
    if (s1.searchesFound.empty()) {
        qDebug() << "Search result is empty.";
        emit searchInpError(ui->urlText1->text());
        emit showError();
    } else {
        qDebug() << "Search results found.";
        // Display results in the query window
        querywindow *queryWin = new querywindow(&s1,queryText, this, this, this);
        std::priority_queue<Node, std::vector<Node>, GreaterCount> resultsCopy = s1.searchesFound;

        queryWin->displayResults(resultsCopy);
        queryWin->exec();
    }
}




void GUI1::on_searchWebButton_clicked()
{
    QString link = ui->searchText1->text();
    QUrl url = QUrl::fromUserInput(link);
    if (link.isEmpty() || (!QDesktopServices::openUrl(url) && url.isValid())) {
        // If the link is empty or the openUrl() call fails and the URL is valid, emit the showError signal
        emit showError();
    }

}


void GUI1::on_searchOnGoogle_clicked()
{
    QString link1 = "https://www.google.com/search?q=";
    QString link2 = ui->urlText1->text();
    QString link = link1+link2;
    QUrl url = QUrl::fromUserInput(link);
    if (link.isEmpty() || (!QDesktopServices::openUrl(url) && url.isValid())) {
        // If the link is empty or the openUrl() call fails and the URL is valid, emit the showError signal
        emit showError();
    }
}


void GUI1::on_graphButton_clicked()
{
    graphWindow *gWindow = new graphWindow(&s1, this);
    gWindow->exec();
}


