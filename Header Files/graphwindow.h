#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "searchengine.h"

namespace Ui {
class graphWindow;
}

class graphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit graphWindow(searchEngine* engine, QWidget *parent = nullptr);
    ~graphWindow();

private slots:
    void on_displayGraphButton_clicked();
    void on_findPathButton_clicked();

private:
    Ui::graphWindow *ui;
    searchEngine *sEngine;
};

#endif // GRAPHWINDOW_H
