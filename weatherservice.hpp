#ifndef WEATHERSERVICE_H
#define WEATHERSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct WeatherData {
    QString city;
    QString country;
    double temperature;
    double feelsLike;
    double humidity;
    double pressure;
    double windSpeed;
    QString weatherDescription;
    QString weatherIcon;
    QString dateTime;

    // Для имперской системы
    double temperatureF;
    double pressurePsi;
    double windSpeedMph;
};

class WeatherService : public QObject
{
    Q_OBJECT

public:
    explicit WeatherService(QObject *parent = nullptr);
    ~WeatherService();

    void fetchWeather(const QString &city, const QString &apiKey);
    void fetchWeatherByCoords(double latitude, double longitude, const QString &apiKey);

    // Переключение между системами измерений
    WeatherData getMetricData() const;
    WeatherData getImperialData() const;

signals:
    void weatherDataReceived(const WeatherData &data);
    void errorOccurred(const QString &errorMessage);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    WeatherData currentData;

    void parseWeatherData(const QJsonObject &jsonObject);
    void convertToImperial();
};

#endif // WEATHERSERVICE_H
