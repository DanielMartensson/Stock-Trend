#include "addstockwindow.h"
#include "ui_addstockwindow.h"

/* QT libraires */
#include <QMessageBox>

AddStockWindow::AddStockWindow(QWidget *parent, QTableWidget *stockListTableWidget, bool modify) :
    QWidget(parent),
    ui(new Ui::AddStockWindow),
    stockListTableWidget(stockListTableWidget),
    modify(modify)
{
    ui->setupUi(this);

    // Connect to yahoo finance
    connect(&yahooFinance, &YahooFinance::transmitExistResult, this, &AddStockWindow::receiveResult);

    // Set the name index
    ui->selectedIndexComboBox->addItems({"Oil & Gas", "Cars", "Houses", "Green energy", "Manufacturing", "Transport", "Health", "Food", "Technology", "Travel", "Communication", "Banks"});

    // If we are going to modify, then display the selected name in the text box
    if(modify){
        QString nameOfSelectedStock = stockListTableWidget->selectionModel()->selectedRows().at(0).data().toString();
        QString nameOfSelectedIndexStock = stockListTableWidget->selectionModel()->selectedRows(1).at(0).data().toString();
        ui->stockNameLineEdit->setText(nameOfSelectedStock);
        ui->selectedIndexComboBox->setCurrentText(nameOfSelectedIndexStock);

        // Disable these
        ui->stockNameLineEdit->setEnabled(false);
        ui->addToStockListPushButton->setEnabled(false);

        // And enable
        ui->updateStockPushButton->setEnabled(true);
    }else{
        ui->updateStockPushButton->setEnabled(false);
    }

}

AddStockWindow::~AddStockWindow()
{
    delete ui;
}

void AddStockWindow::on_addToStockListPushButton_clicked()
{
    // Check if this exist
    stockName = ui->stockNameLineEdit->text().toUpper();
    stockIndexName = ui->selectedIndexComboBox->currentText();
    yahooFinance.stockNameExist(stockName);
}

void AddStockWindow::on_updateStockPushButton_clicked()
{
    // Update the stock
    int row = stockListTableWidget->currentRow();
    stockIndexName = ui->selectedIndexComboBox->currentText();
    stockListTableWidget->setItem(row, 1, new QTableWidgetItem(stockIndexName));
}

void AddStockWindow::receiveResult(int result){
    // If status error is above 0, then we got an error
    if(result > 0){
        QMessageBox::warning(this, "Stock name", "No connection to that stock");
    }else{

        // Check if the stock name exist
        int stockAdded = stockListTableWidget->rowCount();
        bool exist = false;
        for(int i = 0; i < stockAdded; i++){
            if(stockName == stockListTableWidget->item(i, 0)->text()){
                exist = true;
                break;
            }
        }

        // Add new row
        if(!exist){
            stockListTableWidget->setRowCount(stockAdded + 1);
            stockListTableWidget->setItem(stockAdded, 0, new QTableWidgetItem(stockName));
            stockListTableWidget->setItem(stockAdded, 1, new QTableWidgetItem(stockIndexName));
        }else{
            QMessageBox::information(this, "Stock name", "Already exist");
        }
    }
}


