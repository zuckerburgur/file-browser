#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QDialog>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = nullptr, QWidget *mainWindow = nullptr);
    ~ErrorWindow();
    void searchError(const QString &text);

private slots:
    void on_closeButton_clicked();
    void onBackButtonClicked();
    void onHomeButtonClicked();
    void onForwardButtonClicked();

private:
    Ui::ErrorWindow *ui;
    QWidget *mainWindow;
    QWidget *parentWindow;
};

#endif // ERRORWINDOW_H
