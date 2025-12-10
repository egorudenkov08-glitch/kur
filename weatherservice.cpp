#include "weatherservice.hpp"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>

WeatherService::WeatherService(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &WeatherService::onReplyFinished);
}

WeatherService::~WeatherService()
{
}

void WeatherService::fetchWeather(const QString &city, const QString &apiKey)
{
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("q", city);
    query.addQueryItem("appid", apiKey);
    query.addQueryItem("units", "metric");
    query.addQueryItem("lang", "ru");
    url.setQuery(query);
    
    QNetworkRequest request(url);
    networkManager->get(request);
}

void WeatherService::fetchWeatherByCoords(double latitude, double longitude, const QString &apiKey)
{
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("lat", QString::number(latitude));
    query.addQueryItem("lon", QString::number(longitude));
    query.addQueryItem("appid", apiKey);
    query.addQueryItem("units", "metric");
    query.addQueryItem("lang", "ru");
    url.setQuery(query);
    
    QNetworkRequest request(url);
    networkManager->get(request);
}

void WeatherService::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        
        parseWeatherData(jsonObj);
        convertToImperial();
        
        emit weatherDataReceived(currentData);
    } else {
        QString error = reply->errorString();
        emit errorOccurred("Ошибка сети: " + error);
    }
    
    reply->deleteLater();
}

void WeatherService::parseWeatherData(const QJsonObject &jsonObject)
{

    if (jsonObject.contains("name")) {
        currentData.city = jsonObject["name"].toString();
    }
    
    if (jsonObject.contains("sys") && jsonObject["sys"].isObject()) {
        QJsonObject sysObj = jsonObject["sys"].toObject();
        if (sysObj.contains("country")) {
            currentData.country = sysObj["country"].toString();
        }
    }
    

    if (jsonObject.contains("main") && jsonObject["main"].isObject()) {
        QJsonObject mainObj = jsonObject["main"].toObject();
        currentData.temperature = mainObj["temp"].toDouble();
        currentData.feelsLike = mainObj["feels_like"].toDouble();
        currentData.pressure = mainObj["pressure"].toDouble();
        currentData.humidity = mainObj["humidity"].toDouble();
    }
    

    if (jsonObject.contains("wind") && jsonObject["wind"].isObject()) {
        QJsonObject windObj = jsonObject["wind"].toObject();
        currentData.windSpeed = windObj["speed"].toDouble();
    }

    if (jsonObject.contains("weather") && jsonObject["weather"].isArray()) {
        QJsonArray weatherArray = jsonObject["weather"].toArray();
        if (!weatherArray.isEmpty()) {
            QJsonObject weatherObj = weatherArray.first().toObject();
            currentData.weatherDescription = weatherObj["description"].toString();
            currentData.weatherIcon = weatherObj["icon"].toString();
        }
    }
    

    if (jsonObject.contains("dt")) {
        qint64 timestamp = jsonObject["dt"].toInt();
        currentData.dateTime = QDateTime::fromSecsSinceEpoch(timestamp).toString("dd.MM.yyyy hh:mm");
    }
}

void WeatherService::convertToImperial()
{

    currentData.temperatureF = (currentData.temperature * 9/5) + 32;
    

    currentData.pressurePsi = currentData.pressure * 0.0145038;
    

    currentData.windSpeedMph = currentData.windSpeed * 2.23694;
}

WeatherData WeatherService::getMetricData() const
{
    return currentData;
}

WeatherData WeatherService::getImperialData() const
{
    WeatherData imperialData = currentData;
    imperialData.temperature = imperialData.temperatureF;
    imperialData.pressure = imperialData.pressurePsi;
    imperialData.windSpeed = imperialData.windSpeedMph;
    return imperialData;
}
