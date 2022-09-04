#ifndef ADDSTOCKWINDOW_H
#define ADDSTOCKWINDOW_H

#include <QWidget>
#include <QTableWidget>

/* Internal libraries */
#include "tools/yahoofinance/yahoofinance.h"

namespace Ui {
class AddStockWindow;
}

class AddStockWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddStockWindow(QWidget *parent = nullptr, QTableWidget *stockListTableWidget = nullptr, bool modify = false);
    ~AddStockWindow();

private:
    YahooFinance yahooFinance;
    QString stockName;
    QString stockIndexName;
    void receiveResult(int result);
private slots:
    void on_addToStockListPushButton_clicked();

    void on_updateStockPushButton_clicked();

private:
    Ui::AddStockWindow *ui;
    QTableWidget *stockListTableWidget;
    bool modify;
};

#endif // ADDSTOCKWINDOW_H
