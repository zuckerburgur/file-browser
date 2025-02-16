#ifndef WEBPAGEDISPLAY_H
#define WEBPAGEDISPLAY_H

#include <QDialog>
#include <QStack>

namespace Ui {
class WebpageDisplay;
}

class WebpageDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit WebpageDisplay(QWidget *parent = nullptr, QWidget *mainWindow = nullptr, QWidget *rootWindow = nullptr);
    ~WebpageDisplay();

    void setWebPageContent(const QString &content);
    void setDocumentName(const QString &docName);

private slots:
    void on_backButton_clicked();
    void on_forwardButton_clicked();
    void on_homeButton_clicked();
    void on_textBrowser_anchorClicked(const QUrl &url);

private:
    Ui::WebpageDisplay *ui;
    QWidget *mainWindow;
    QWidget *parentWindow;
    QWidget *rootWindow;
    QString homePage;
    QStack<QString> navigationHistory;  // Stack to keep track of back navigation history
    QStack<QString> documentNames;      // Stack to keep track of document names
    QStack<QString> forwardHistory;     // Stack to keep track of forward navigation history
    QStack<QString> forwardDocumentNames; // Stack to keep track of forward document names
};

#endif // WEBPAGEDISPLAY_H
