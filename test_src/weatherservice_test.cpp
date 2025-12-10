#include "weatherservice_test.hpp"
#include <QDebug>
#include <QRandomGenerator>
#include <cmath>

bool TestWeatherService::compareDoubles(double a, double b, double epsilon)
{
    return std::abs(a - b) < epsilon;
}

void TestWeatherService::printTestResult(const QString& testName, bool passed, const QString& details)
{
    QString status = passed ? "✓ ПРОЙДЕН" : "✗ ПРОВАЛЕН";
    QString color = passed ? "\033[32m" : "\033[31m";
    qDebug() << color << status << "\033[0m" << testName;
    if (!details.isEmpty()) {
        qDebug() << "   " << details;
    }
}


void TestWeatherService::testTemperatureConversion()
{
    qDebug() << "\n=== Тестирование конвертации температуры ===";

    struct TestCase {
        double celsius;
        double expectedFahrenheit;
        QString description;
    };

    TestCase testCases[] = {
        {-40.0, -40.0, "-40°C = -40°F"},
        {0.0, 32.0, "0°C = 32°F"},
        {20.0, 68.0, "20°C = 68°F"},
        {37.0, 98.6, "37°C ≈ 98.6°F"},
        {100.0, 212.0, "100°C = 212°F"}
    };

    int passed = 0;
    int total = sizeof(testCases) / sizeof(testCases[0]);

    for (const auto& testCase : testCases) {
        double result = WeatherService::celsiusToFahrenheit(testCase.celsius);
        bool testPassed = compareDoubles(result, testCase.expectedFahrenheit, 0.1);

        printTestResult(testCase.description, testPassed,
                        QString("Получено: %1, Ожидалось: %2")
                            .arg(result, 0, 'f', 1)
                            .arg(testCase.expectedFahrenheit, 0, 'f', 1));

        if (testPassed) passed++;
    }

    qDebug() << QString("Результат: %1/%2 тестов пройдено").arg(passed).arg(total);
}


void TestWeatherService::testPressureConversion()
{
    qDebug() << "\n=== Тестирование конвертации давления ===";

   
    double hpa = 1013.25;
    double expectedPsi = 14.6959;

    double result = WeatherService::hpaToPsi(hpa);
    bool testPassed = compareDoubles(result, expectedPsi, 0.001);

    printTestResult("Стандартное давление (1013.25 hPa = 14.6959 psi)",
                    testPassed,
                    QString("Получено: %1, Ожидалось: %2")
                        .arg(result, 0, 'f', 4)
                        .arg(expectedPsi, 0, 'f', 4));

    
    double zeroPsi = WeatherService::hpaToPsi(0);
    testPassed = compareDoubles(zeroPsi, 0.0);
    printTestResult("Нулевое давление (0 hPa = 0 psi)", testPassed);
}


void TestWeatherService::testWindSpeedConversion()
{
    qDebug() << "\n=== Тестирование конвертации скорости ветра ===";

    struct TestCase {
        double ms;
        double expectedMph;
        QString description;
    };

    TestCase testCases[] = {
        {0.0, 0.0, "0 м/с = 0 миль/ч"},
        {1.0, 2.23694, "1 м/с = 2.23694 миль/ч"},
        {10.0, 22.3694, "10 м/с = 22.3694 миль/ч"},
        {20.0, 44.7388, "20 м/с = 44.7388 миль/ч"}
    };

    int passed = 0;
    int total = sizeof(testCases) / sizeof(testCases[0]);

    for (const auto& testCase : testCases) {
        double result = WeatherService::msToMph(testCase.ms);
        bool testPassed = compareDoubles(result, testCase.expectedMph, 0.001);

        printTestResult(testCase.description, testPassed,
                        QString("Получено: %1, Ожидалось: %2")
                            .arg(result, 0, 'f', 4)
                            .arg(testCase.expectedMph, 0, 'f', 4));

        if (testPassed) passed++;
    }

    qDebug() << QString("Результат: %1/%2 тестов пройдено").arg(passed).arg(total);
}



void TestWeatherService::runAllTests()
{
    qDebug() << "\n" << QString(60, '=');
    qDebug() << " ЗАПУСК ТЕСТОВ WEATHERSERVICE";
    qDebug() << QString(60, '=');

    testTemperatureConversion();
    testPressureConversion();
    testWindSpeedConversion();


    qDebug() << "\n" << QString(60, '=');
    qDebug() << " ВСЕ ТЕСТЫ ВЫПОЛНЕНЫ";
    qDebug() << QString(60, '=');
}
