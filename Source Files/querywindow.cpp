#include "querywindow.h"
#include "ui_querywindow.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>

querywindow::querywindow(searchEngine* engine,const QString& searchQuery, QWidget *parent, QWidget *mainWindow, QWidget *rootWindow) :
    sEngine(engine),
    QDialog(parent),
    ui(new Ui::querywindow),
    mainWindow(mainWindow),
    parentWindow(parent),
    rootWindow(rootWindow)
{
    ui->setupUi(this);
    QPixmap pix("purple.jpg");
    ui->bg->setPixmap(pix);
    ui->searchText2->setText(searchQuery);

    // Initialize the results layout within the resultsPlaceholder widget
    resultsLayout = new QVBoxLayout(ui->resultsPlaceholder);

    connect(ui->backButton2, &QPushButton::clicked, this, &querywindow::onBackButtonClicked);
    connect(ui->refreshButton2, &QPushButton::clicked, this, &querywindow::onHomeButtonClicked);
    connect(ui->forwardButton2, &QPushButton::clicked, this, &querywindow::onForwardButtonClicked);
}

querywindow::~querywindow()
{
    delete ui;
}

void querywindow::displayResults(std::priority_queue<Node, std::vector<Node>, GreaterCount> results)
{
    // Clear previous results
    QLayoutItem *child;
    while ((child = resultsLayout->takeAt(0)) != nullptr) {
        QWidget* widget = child->widget();
        if (widget) {
            widget->deleteLater();
        }
        delete child;
    }

    // Display new results
    while (!results.empty()) {
        Node n = results.top();
        results.pop();
        QString docInfo = QString("%1 | %2").arg(QString::fromStdString(n.docName)).arg(n.count);
        QLabel *label = new QLabel(docInfo);
        label->setStyleSheet(
            "QLabel {"
            "  color: white;"              // White text color
            "  background: transparent;"   // No background
            "  font-family: 'Agency FB';"  // Agency FB font
            "  font-size: 18px;"           // Adjust font size as needed
            "font-weight: bold;"
            "}"
            );
        resultsLayout->addWidget(label);

        QPushButton *linkButton = new QPushButton("Open Document");
        linkButton->setProperty("docPath", QString::fromStdString(n.docName));
        linkButton->setStyleSheet(
            "QPushButton {"
                "color: white;"
                "border: 1px solid white;"
                    "background-color: transparent;"
                    "border-radius: 20px; /* Adjust the value as needed */"
            "}"

            "QPushButton:hover {"
                "color: rgb(103, 55, 197);"
                "border: 1px solid transparent;"
                    "background-color: white;"
            "}"
            );
        connect(linkButton, &QPushButton::clicked, this, &querywindow::onLinkButtonClicked);
        resultsLayout->addWidget(linkButton);
    }

    // Refresh the layout to ensure it is updated correctly
    resultsLayout->update();
}


string querywindow::returnWorkingDirectory(string FileName)
{
    string retPath = " ";
    for(graphNode doc : sEngine->documentGraph)
    {
        if(doc.document == FileName)
        {
            retPath = doc.docPath;
            break;
        }
    }

    if(retPath == " ")
    {

        return "C:\\Users\\rijaa\\Desktop\\Sem 4 to be sorted\\projectnew\\projectnew";
    }

    string processedPath = "";
    for (char c : retPath) {
        if (c == '\\') {
            processedPath += "\\\\";
        } else {
            processedPath += c;
        }
    }

    size_t lastBackslashPos = processedPath.find_last_of('\\');
    string processedPath2;
    // If a backslash is found, remove it along with the fileName
    if (lastBackslashPos != std::string::npos) {
        processedPath2 = processedPath.substr(0, lastBackslashPos);
    }

    cout<<"ys: "<<processedPath2<<endl;
    return processedPath2;
}


void querywindow::onLinkButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString docPath = button->property("docPath").toString();

    //cout<<"dosos: "<< docPath.toStdString()<<endl;
    // Set the working directory to the directory of the document
    QFileInfo fileInfo(docPath);
    string strDoc = docPath.toStdString();
    string dir = returnWorkingDirectory(strDoc);
    QString qdir = QString::fromStdString(dir);


    QString newWorkingDirectory = qdir;
    if (!QDir::setCurrent(newWorkingDirectory)) {
        qDebug() << "Failed to set working directory to:" << newWorkingDirectory;
        return;
    }

    // Debugging: Check if file exists and is accessible
    if (!fileInfo.exists()) {
        qDebug() << "File does not exist:" << docPath;
        qDebug() << "Current Working Directory:" << QDir::currentPath();
        return;
    }

    if (!fileInfo.isReadable()) {
        qDebug() << "File is not readable:" << docPath;
        return;
    }

    // Read the content of the file
    QFile file(docPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << docPath;
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Create and show the WebpageDisplay window
    WebpageDisplay *webpageDisplay = new WebpageDisplay(this, mainWindow, rootWindow); // Set this as parent
    webpageDisplay->setDocumentName(QFileInfo(docPath).baseName());
    webpageDisplay->setWebPageContent(content);

    // Add the WebpageDisplay window to the list
    webpageWindows.append(webpageDisplay);

    // Show the WebpageDisplay window and bring it to the front
    webpageDisplay->show();
    webpageDisplay->raise();
    webpageDisplay->activateWindow();

    qDebug() << "Displaying document:" << docPath;
    qDebug() << "New Working Directory:" << QDir::currentPath();
}

void querywindow::closeEvent(QCloseEvent *event)
{
    // Close all WebpageDisplay windows
    for (WebpageDisplay *webpageWindow : webpageWindows) {
        if (webpageWindow) {
            webpageWindow->close();
        }
    }

    // Accept the close event
    event->accept();
}

void querywindow::onBackButtonClicked()
{
    // Close this window and go back to the parent window
    this->close();

    QWidget *parentWidget = qobject_cast<QWidget*>(this->parent());
    if (parentWidget) {
        parentWidget->show();
    } else {
        qDebug() << "Parent widget is not available";
    }

    qDebug() << "Back button clicked";
}

void querywindow::onHomeButtonClicked()
{
    this->close();
    if (rootWindow) {
        rootWindow->show();
    } else {
        qDebug() << "Root window is not available";
    }
    qDebug() << "Home button clicked";
}

void querywindow::onForwardButtonClicked()
{
    // Implement forward button functionality
    qDebug() << "Forward button clicked";
}
