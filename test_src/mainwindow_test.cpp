#include "mainwindow_test.hpp"
#include <QDebug>
#include <QApplication>
#include <QRegularExpression>


 MainWindow* TestMainWidget::createTestWidget()
{
    MainWindow* widget = new MainWindow();
    widget->show();
    QTest::qWait(100);
    return widget;
}

void TestMainWidget::cleanupTestWidget(MainWindow* widget)
{
    if (widget) {
        widget->close();
        delete widget;
    }
}


void TestMainWidget::testInputValidation()
{
    qDebug() << "\n=== Тестирование валидации ввода ===";

   MainWindow* widget = createTestWidget();
    int passed = 0;
    int total = 3;


    QLineEdit* cityInput = widget->findChild<QLineEdit*>();
    QPushButton* searchButton = widget->findChild<QPushButton*>();

    if (cityInput && searchButton) {

        cityInput->clear();
        QTest::mouseClick(searchButton, Qt::LeftButton);
        QTest::qWait(100);


        bool testPassed = cityInput->text().isEmpty();
        if (testPassed) passed++;

      
        cityInput->setText("   ");
        QTest::mouseClick(searchButton, Qt::LeftButton);
        QTest::qWait(100);

    cleanupTestWidget(widget);
    qDebug() << QString("Результат: %1/%2 тестов пройдено").arg(passed).arg(total);
}
}

void TestMainWidget::testDataFormatting()
{
    qDebug() << "\n=== Тестирование форматирования данных ===";

   MainWindow widget;
    widget.show();
    QTest::qWait(100);

    int passed = 0;
    int total = 4;


    QString tempMetric = widget.formatTemperature(22.5, true);
    bool testPassed = tempMetric.contains("°C") && tempMetric.contains("23");
    printTestResult("Форматирование температуры (метрика)", testPassed,
                    "Результат: " + tempMetric);
    if (testPassed) passed++;


    QString tempImperial = widget.formatTemperature(72.5, false);
    testPassed = tempImperial.contains("°F") && tempImperial.contains("73");
    printTestResult("Форматирование температуры (имперская)", testPassed,
                    "Результат: " + tempImperial);
    if (testPassed) passed++;


    QString pressureMetric = widget.formatPressure(1015.3, true);
    testPassed = pressureMetric.contains("hPa") && pressureMetric.contains("1015");
    printTestResult("Форматирование давления (метрика)", testPassed,
                    "Результат: " + pressureMetric);
    if (testPassed) passed++;


    QString windMetric = widget.formatWindSpeed(3.49, true);
    testPassed = windMetric.contains("м/с") && windMetric.contains("3.5");
    printTestResult("Форматирование скорости ветра", testPassed,
                    "Результат: " + windMetric);
    if (testPassed) passed++;

    widget.close();
    qDebug() << QString("Результат: %1/%2 тестов пройдено").arg(passed).arg(total);
}


void TestMainWidget::testUnitSystemToggle()
{
    qDebug() << "\n=== Тестирование переключения систем измерения ===";

   MainWindow* widget = createTestWidget();
    int passed = 0;
    int total = 3;


    QList<QRadioButton*> radioButtons = widget->findChildren<QRadioButton*>();
    QRadioButton* metricRadio = nullptr;
    QRadioButton* imperialRadio = nullptr;

    for (auto radio : radioButtons) {
        if (radio->text().contains("Метрическая")) {
            metricRadio = radio;
        } else if (radio->text().contains("Имперская")) {
            imperialRadio = radio;
        }
    }

    if (metricRadio && imperialRadio) {

        bool testPassed = metricRadio->isChecked() && !imperialRadio->isChecked();

        if (testPassed) passed++;

        imperialRadio->setChecked(true);
        QTest::qWait(50);

        testPassed = !metricRadio->isChecked() && imperialRadio->isChecked();
        printTestResult("Переключение на имперскую систему", testPassed);
        if (testPassed) passed++;


        metricRadio->setChecked(true);
        QTest::qWait(50);

        testPassed = metricRadio->isChecked() && !imperialRadio->isChecked();
        printTestResult("Возврат к метрической системе", testPassed);
        if (testPassed) passed++;
    }

    cleanupTestWidget(widget);
    qDebug() << QString("Результат: %1/%2 тестов пройдено").arg(passed).arg(total);
}




void TestMainWidget::runAllTests()
{
    qDebug() << "\n" << QString(60, '=');
    qDebug() << " ЗАПУСК ТЕСТОВMainWindow";
    qDebug() << QString(60, '=');

    testInputValidation();
    testDataFormatting();
    testUnitSystemToggle();


    qDebug() << "\n" << QString(60, '=');
    qDebug() << " ВСЕ ТЕСТЫ ВЫПОЛНЕНЫ";
    qDebug() << QString(60, '=');
}
