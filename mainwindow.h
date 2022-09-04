#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

/* Defines */
#define STOCK_INDEX_NAMES "stock_index_names"
#define STOCK_NAMES "stock_names"
#define STOCK_LIST "stock_list.json"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addStockPushButton_clicked();

    void on_editStockPushButton_clicked();

    void on_deleteStockPushButton_clicked();

    void on_fromDateEdit_userDateChanged(const QDate &date);

    void on_toDateEdit_userDateChanged(const QDate &date);

    void on_analyseStockPushButton_clicked();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
