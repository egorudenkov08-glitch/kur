#ifndef TEST_WEATHERWIDGET_H
#define TEST_WEATHERWIDGET_H

#include <QObject>
#include <QTest>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../src/mainwindow.hpp"

class TestMainWidget : public QObject
{
    Q_OBJECT

private slots:

    void testInputValidation();


    void testDataFormatting();


     void testUnitSystemToggle();


    void runAllTests();

    MainWindow* createTestWidget();
    void cleanupTestWidget(MainWindow* widget);
    void printTestResult(const QString& testName, bool passed, const QString& details = "");


};

#endif
