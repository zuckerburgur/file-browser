#ifndef GUI1_H
#define GUI1_H
#include <QDir>
#include <QMainWindow>
#include "searchengine.h" // Include your search engine header



QT_BEGIN_NAMESPACE
namespace Ui {
class GUI1;
}
QT_END_NAMESPACE

class GUI1 : public QMainWindow
{
    Q_OBJECT

public:
    GUI1(QWidget *parent = nullptr);
    ~GUI1();

    //QStringList searchFiles(const QString& searchText, const QDir& dir);

signals:
    void searchInput(const QString &text); //gets input from the search box
    void searchInpError(const QString &text);
    void searchQuery(const QString &text);
    void showResults(); //calls the next window
    void showError(); //calls error window
    void showQuery(); //call squery window

private slots:
    void on_refreshButton_clicked();
    void on_searchButton1_clicked();
    void on_goButton1_clicked();

    void on_searchWebButton_clicked();

    void on_searchOnGoogle_clicked();

    void on_graphButton_clicked();

private:
    Ui::GUI1 *ui;
    searchEngine s1; // Add the search engine instance
    void refreshItems1();

};
#endif // GUI1_H





