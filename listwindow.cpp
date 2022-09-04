#include "listwindow.h"
#include "ui_listwindow.h"


#include <QColor>
const int dayJumps[] = {1, 5, 5*4, 5*4*3, 5*4*6, 5*4*12, 5*4*12*5}; // 1d, 1w, 1m, 3m, 6m, 1y, 5y
const int maxJumps = sizeof(dayJumps)/sizeof(dayJumps[0]);


ListWindow::ListWindow(QWidget *parent, QTableWidget *stockListTableWidget, qint64 fromDate, qint64 toDate) :
    QWidget(parent),
    ui(new Ui::ListWindow),
    stockListTableWidget(stockListTableWidget),
    fromDate(fromDate),
    toDate(toDate)
{
    ui->setupUi(this);

    // Set up the table
    clearTable();

    // Set the limit
    ui->limitComboBox->addItems({"1D %", "1W %", "1M %", "3M %", "6M %", "1Y %", "5Y %"});

    // Connect to yahoo finance
    connect(&yahooFinance, &YahooFinance::transmitDownloadResult, this, &ListWindow::receiveResult);
}

ListWindow::~ListWindow()
{
    delete ui;
}

void ListWindow::receiveResult(QByteArray data){  
    // Split the data
    QString text = QString(data);
    QStringList textRows = text.split('\n');

    // Check if we have more than two lines = available data
    int rowLength = textRows.length();
    if(rowLength > 1){
        // Prepare rows
        QString stockName = stockListTableWidget->item(receivedResults, 0)->text();
        QString stockIndexName = stockListTableWidget->item(receivedResults, 1)->text();
        ui->tableWidget->setRowCount(receivedResults+1);

        // Read all lines
        int dayIndex = 0;
        QList<float> priceList;
        for(int i = rowLength; i >= 1; i--){
            // Jump to 1d, 1w, 1m, 3m, 6m, 1y, 5y
            i -= dayJumps[dayIndex++];

            // If data is not available
            if(i < 0)
                break;

            // Split row
            QStringList row = textRows.at(i).split(',');

            // Add the last price
            if(dayIndex == 1){
                float lastPrice = row.at(4).toFloat(); // This is the close price
                float closePriceYeasterDay = textRows.at(i-1).split(',').at(4).toFloat();
                priceList.append(lastPrice);
                priceList.append(closePriceYeasterDay);
            }else{
                // Add close price
                float closePrice = row.at(4).toFloat();
                priceList.append(closePrice);
            }




            // This is important so we don't over indexing
            if(dayIndex >= maxJumps || dayIndex > ui->limitComboBox->currentIndex())
                break;
        }

        // Set the stock name
        QTableWidgetItem *stockNameItem = new QTableWidgetItem(stockName);
        stockNameItem->setBackground(Qt::yellow);
        ui->tableWidget->setItem(receivedResults, 0, stockNameItem);

        // Set sthe stock index name
        QTableWidgetItem *stockIndexkNameItem = new QTableWidgetItem(stockIndexName);
        stockIndexkNameItem->setBackground(Qt::gray);
        ui->tableWidget->setItem(receivedResults, 1, stockIndexkNameItem);

        // Convert to percent
        for(int i = 1; i < priceList.length(); i++){
            // Get the
            float lastPrice = priceList.at(0);
            float closePrice = priceList.at(i);
            float percent = (lastPrice / closePrice - 1)*100;

            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, percent);
            if(percent > 0)
                item->setBackground(Qt::green);
            else if(percent < 0)
                item->setBackground(Qt::red);
            else
                item->setBackground(Qt::white);
            ui->tableWidget->setItem(receivedResults, i+1, item);

        }

    }

    // Count
    receivedResults++;

    // Check
    int rows = stockListTableWidget->rowCount();
    if(rows > receivedResults){
        QString stockName = stockListTableWidget->item(receivedResults, 0)->text();
        yahooFinance.stockCSVDownload(stockName, fromDate, toDate); // When this is called, then this function receiveResult will be called too
    }
}

void ListWindow::on_sortPushButton_clicked()
{
    // Reset
    clearTable();
    receivedResults = 0;

    // This will trigger the download
    QString stockName = stockListTableWidget->item(0, 0)->text();
    yahooFinance.stockCSVDownload(stockName, fromDate, toDate);
}

void ListWindow::clearTable(){
    ui->tableWidget->clear();
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setColumnCount(maxJumps + 1); // + 1 because the stock name column
    ui->tableWidget->setHorizontalHeaderLabels({"Stock name", "Index name", "1D %", "1W %", "1M %", "3M %", "6M %", "1Y %", "5Y %"});
}

