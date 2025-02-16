#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H

#include "searchengine.h"
#include "gui1.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QList>
#include <QTextStream>
#include "webpagedisplay.h"  // Include the WebpageDisplay header
#include <queue>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

using namespace std;

namespace Ui {
class querywindow;
}

class querywindow : public QDialog
{
    Q_OBJECT

public:
    explicit querywindow(searchEngine* engine,const QString& searchQuery, QWidget *parent = nullptr, QWidget *mainWindow = nullptr, QWidget *rootWindow = nullptr);
    ~querywindow();

    void displayResults(priority_queue<Node, vector<Node>, GreaterCount> results);

protected:
    void closeEvent(QCloseEvent *event) override; // Override closeEvent

private slots:
    void onLinkButtonClicked();
    void onBackButtonClicked();
    void onHomeButtonClicked();
    void onForwardButtonClicked();
    string returnWorkingDirectory(string fileName);

private:
    Ui::querywindow *ui;
    QVBoxLayout *resultsLayout; // Layout for dynamic results
    QWidget *mainWindow;
    QWidget *parentWindow;
    QWidget *rootWindow;
    QList<WebpageDisplay*> webpageWindows; // List to store WebpageDisplay windows
    searchEngine *sEngine;
};

#endif // QUERYWINDOW_H
