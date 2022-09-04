#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QFile>

/* Internal libraries */
#include "tools/yahoofinance/yahoofinance.h"

namespace Ui {
class ListWindow;
}

class ListWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ListWindow(QWidget *parent = nullptr, QTableWidget *stockListTableWidget = nullptr, qint64 fromDate = 0, qint64 toDate = 0);
    ~ListWindow();

private:
    Ui::ListWindow *ui;
    YahooFinance yahooFinance;
    QTableWidget *stockListTableWidget;
    int receivedResults;
    qint64 fromDate;
    qint64 toDate;
    void clearTable();
private slots:
    void receiveResult(QByteArray data);
    void on_sortPushButton_clicked();
};

#endif // LISTWINDOW_H
