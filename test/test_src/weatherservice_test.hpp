#ifndef TEST_WEATHERSERVICE_H
#define TEST_WEATHERSERVICE_H

#include <QObject>
#include <QString>
#include <QTest>
#include <QSignalSpy>
#include "../src/weatherservice.hpp"


class TestWeatherService : public QObject
{
    Q_OBJECT

private slots:

    void testTemperatureConversion();
    void testPressureConversion();
    void testWindSpeedConversion();
    // Запуск всех тестов
    void runAllTests();

private:

    bool compareDoubles(double a, double b, double epsilon = 0.001);
    void printTestResult(const QString& testName, bool passed, const QString& details = "");
};

#endif
