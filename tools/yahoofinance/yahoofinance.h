#ifndef YAHOOFINANCE_H
#define YAHOOFINANCE_H

/* Internal libraries */
#include <QString>
#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDateTime>

/* Defines */
#define CHECK_IF_EXIST "https://query1.finance.yahoo.com/v7/finance/download/"

class YahooFinance : public QObject
{
    Q_OBJECT
public:
    YahooFinance();
    void stockNameExist(QString name);
    void stockCSVDownload(QString name, qint64 fromDate, qint64 toDate);

signals:
    void transmitExistResult(int result);
    void transmitDownloadResult(QByteArray data);

private:
    QNetworkAccessManager qNetworkAccessManager;

private slots:
    void existResult(QNetworkReply *reply);
    void downloadResult(QNetworkReply *reply);
};

#endif // YAHOOFINANCE_H
