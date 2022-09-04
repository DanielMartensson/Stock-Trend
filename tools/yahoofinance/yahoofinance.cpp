#include "yahoofinance.h"

YahooFinance::YahooFinance()
{
    connect(&qNetworkAccessManager, &QNetworkAccessManager::finished, this, &YahooFinance::downloadResult);
}

    //disconnect(&qNetworkAccessManager, &QNetworkAccessManager::finished, this, &YahooFinance::downloadResult);


void YahooFinance::stockNameExist(QString name){
    connect(&qNetworkAccessManager, &QNetworkAccessManager::finished, this, &YahooFinance::existResult);
    qNetworkAccessManager.get(QNetworkRequest(QUrl(CHECK_IF_EXIST + name)));
}

void YahooFinance::stockCSVDownload(QString name, qint64 fromDate, qint64 toDate){

    // Unix time
    QString period1 = QString::number(fromDate);
    QString period2 = QString::number(toDate);

    // Request
    QString downloadUrl = QString(CHECK_IF_EXIST) + name + "?period1=" + period1 + "&period2=" + period2 + "&interval=1d&events=history&includeAdjustedClose=true";
    qNetworkAccessManager.get(QNetworkRequest(QUrl(downloadUrl)));
}

void YahooFinance::existResult(QNetworkReply *reply){
    emit transmitExistResult(reply->error());
    disconnect(&qNetworkAccessManager, &QNetworkAccessManager::finished, this, &YahooFinance::existResult);
}

void YahooFinance::downloadResult(QNetworkReply *reply){
    emit transmitDownloadResult(reply->readAll());
}

