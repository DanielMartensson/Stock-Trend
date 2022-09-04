#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Internal libraries */
#include <QMessageBox>

/* Libraries */
#include "tools/json/json.h"

/* Windows */
#include "addstockwindow.h"
#include "listwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the width
    ui->stockListTableWidget->clear();
    ui->stockListTableWidget->setColumnCount(2);
    ui->stockListTableWidget->setHorizontalHeaderLabels({"Stock name", "Index name"});

    // Load stock list
    Json json;
    QJsonObject object = json.loadObject(STOCK_LIST);
    if(!object.isEmpty()){
        // Get two arrays
        QJsonArray stockNames = object.value(STOCK_NAMES).toArray();
        QJsonArray stockIndexNames = object.value(STOCK_INDEX_NAMES).toArray();

        // Fill the table
        int numberOfListedStocks = stockNames.size();
        ui->stockListTableWidget->setRowCount(numberOfListedStocks);
        for(int i = 0; i < numberOfListedStocks; i++){
            ui->stockListTableWidget->setItem(i, 0, new QTableWidgetItem(stockNames.at(i).toString()));
            ui->stockListTableWidget->setItem(i, 1, new QTableWidgetItem(stockIndexNames.at(i).toString()));
        }
    }

    // Set date
    ui->toDateEdit->setMaximumDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent (QCloseEvent *event){
    Q_UNUSED(event)

    // Save to JSON
    int rows = ui->stockListTableWidget->rowCount();
    Json json;
    QJsonArray stockNames;
    QJsonArray stockIndexNames;
    for(int i = 0; i < rows; i++){
        stockNames.append(ui->stockListTableWidget->item(i, 0)->text());
        stockIndexNames.append(ui->stockListTableWidget->item(i, 1)->text());
    }

    // Create object and save
    QJsonObject object;
    object[STOCK_NAMES] = stockNames;
    object[STOCK_INDEX_NAMES] = stockIndexNames;
    json.saveObject(object, STOCK_LIST);
}

void MainWindow::on_addStockPushButton_clicked()
{
    AddStockWindow *addStockWindow = new AddStockWindow(nullptr, ui->stockListTableWidget);
    addStockWindow->show();
}


void MainWindow::on_editStockPushButton_clicked()
{
    if(ui->stockListTableWidget->selectionModel()->selectedRows().size() == 0){
        QMessageBox::warning(this, "Stocks", "No stock selected");
        return;
    }

    AddStockWindow *addStockWindow = new AddStockWindow(nullptr, ui->stockListTableWidget, true);
    addStockWindow->show();
}


void MainWindow::on_deleteStockPushButton_clicked()
{
    if(ui->stockListTableWidget->selectionModel()->selectedRows().size() == 0){
        QMessageBox::warning(this, "Stocks", "No stock selected");
        return;
    }

    // Ask
    int row = ui->stockListTableWidget->currentRow();
    QString stockName = ui->stockListTableWidget->item(row, 0)->text();
    int status = QMessageBox::question(this, "Remove stock", "Do you want to remove " + stockName + "?");
    if(status == QMessageBox::Yes){
         ui->stockListTableWidget->removeRow(row);
    }
}

void MainWindow::on_fromDateEdit_userDateChanged(const QDate &date)
{
    if(date > ui->toDateEdit->date()){
        ui->fromDateEdit->setDate(ui->toDateEdit->date());
    }

    if(date > QDate::currentDate()){
        ui->fromDateEdit->setDate(QDate::currentDate());
    }
}


void MainWindow::on_toDateEdit_userDateChanged(const QDate &date)
{
    if(date < ui->fromDateEdit->date()){
        ui->toDateEdit->setDate(ui->fromDateEdit->date());
    }

    if(date > QDate::currentDate()){
        ui->toDateEdit->setDate(QDate::currentDate());
    }
}


void MainWindow::on_analyseStockPushButton_clicked()
{
    if(ui->stockListTableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Stocks", "No stock added");
        return;
    }

    // Dates in unix time
    qint64 fromDate = ui->fromDateEdit->dateTime().toSecsSinceEpoch();
    qint64 toDate = ui->toDateEdit->dateTime().toSecsSinceEpoch() + 82800; // 82800 is 24 hours. Important to get the latest day at Yahoo Finance

    // Show list of average of each index
    ListWindow *listWindow = new ListWindow(nullptr, ui->stockListTableWidget, fromDate, toDate);
    listWindow->show();
}

