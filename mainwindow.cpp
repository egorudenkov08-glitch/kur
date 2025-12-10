#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QToolButton>
#include <QStyle>
#include <QTimer>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , weatherService(new WeatherService(this))
    , updateTimer(new QTimer(this))
    , isImperial(false)
    , cityLineEdit(nullptr)
    , cityLabel(nullptr)
    , dateTimeLabel(nullptr)
    , tempValueLabel(nullptr)
    , tempIconLabel(nullptr)
    , weatherDescLabel(nullptr)
    , feelsLikeLabel(nullptr)
    , humidityLabel(nullptr)
    , pressureLabel(nullptr)
    , windLabel(nullptr)
    , metricRadio(nullptr)
    , imperialRadio(nullptr)
    , detailsGroup(nullptr)
    , tempLayout(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Weather Widget");
    setMinimumSize(400, 500);

    setupUI();

    connect(weatherService, &WeatherService::weatherDataReceived,
            this, &MainWindow::onWeatherDataReceived);
    connect(weatherService, &WeatherService::errorOccurred,
            this, &MainWindow::onErrorOccurred);


    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    updateTimer->start(1000);

    if (cityLineEdit) {
        cityLineEdit->setText("Moscow");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityLineEdit = new QLineEdit();
    cityLineEdit->setPlaceholderText("Введите город (например: Moscow, London)...");
    cityLineEdit->setMinimumWidth(150);

    QPushButton *searchButton = new QPushButton("Поиск");
    searchButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    QPushButton *refreshButton = new QPushButton("Обновить");
    refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);

    controlLayout->addWidget(cityLineEdit);
    controlLayout->addWidget(searchButton);

    controlLayout->addWidget(refreshButton);


    QGroupBox *weatherGroup = new QGroupBox("Текущая погода");
    QVBoxLayout *weatherLayout = new QVBoxLayout(weatherGroup);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    cityLabel = new QLabel("Город не выбран");
    cityLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    dateTimeLabel = new QLabel();
    dateTimeLabel->setStyleSheet("font-size: 12px; color: gray;");
    headerLayout->addWidget(cityLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(dateTimeLabel);

    tempLayout = new QHBoxLayout();
    tempIconLabel = new QLabel();
    tempValueLabel = new QLabel();
    tempValueLabel->setStyleSheet("font-size: 48px; font-weight: bold;");
    tempLayout->addWidget(tempIconLabel);
    tempLayout->addWidget(tempValueLabel);
    tempLayout->addStretch();


    weatherDescLabel = new QLabel();
    weatherDescLabel->setStyleSheet("font-size: 14px; font-style: italic;");
    weatherDescLabel->setAlignment(Qt::AlignCenter);


    detailsGroup = new QGroupBox("Детали погоды");
    QGridLayout *detailsLayout = new QGridLayout(detailsGroup);

    feelsLikeLabel = new QLabel("Ощущается как: --");
    humidityLabel = new QLabel("Влажность: --");
    pressureLabel = new QLabel("Давление: --");
    windLabel = new QLabel("Ветер: --");

    detailsLayout->addWidget(feelsLikeLabel, 0, 0);
    detailsLayout->addWidget(humidityLabel, 0, 1);
    detailsLayout->addWidget(pressureLabel, 1, 0);
    detailsLayout->addWidget(windLabel, 1, 1);

    QHBoxLayout *unitsLayout = new QHBoxLayout();
    unitsLayout->addStretch();
    QLabel *unitsLabel = new QLabel("Система измерений:");
    metricRadio = new QRadioButton("Метрическая (C, hPa, m/s)");
    imperialRadio = new QRadioButton("Имперская (F, psi, mph)");
    metricRadio->setChecked(true);

    connect(metricRadio, &QRadioButton::toggled, this, &MainWindow::onUnitsChanged);

    unitsLayout->addWidget(unitsLabel);
    unitsLayout->addWidget(metricRadio);
    unitsLayout->addWidget(imperialRadio);


    weatherLayout->addLayout(headerLayout);
    weatherLayout->addLayout(tempLayout);
    weatherLayout->addWidget(weatherDescLabel);
    weatherLayout->addWidget(detailsGroup);
    weatherLayout->addStretch();

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(weatherGroup);
    mainLayout->addLayout(unitsLayout);

    setCentralWidget(centralWidget);


    clearWeatherData();
}

void MainWindow::onSearchClicked()
{
    if (!cityLineEdit) {
        return;
    }

    QString city = cityLineEdit->text().trimmed();
    if (city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название города");
        return;
    }

    QString apiKey =  "cda7d6273caa03df8e362b4dc314c1b0";

    weatherService->fetchWeather(city, apiKey);
}



void MainWindow::onRefreshClicked()
{
    onSearchClicked();
}

void MainWindow::onUnitsChanged(bool checked)
{
    isImperial = imperialRadio->isChecked();
    if (cityLabel && !cityLabel->text().isEmpty() && cityLabel->text() != "Город не выбран") {

        onSearchClicked();
    }
}

void MainWindow::onWeatherDataReceived(const WeatherData &data)
{
    displayWeatherData(data);
}

void MainWindow::displayWeatherData(const WeatherData &data)
{
    if (!cityLabel || !tempValueLabel) {
        return;
    }

    cityLabel->setText(data.city + ", " + data.country);

    if (dateTimeLabel) {
        dateTimeLabel->setText(data.dateTime);
    }

    if (isImperial) {
        WeatherData imperialData = weatherService->getImperialData();
        tempValueLabel->setText(QString::number(qRound(imperialData.temperature)) + "°F");

        if (feelsLikeLabel) {
            feelsLikeLabel->setText(QString("Ощущается как: %1°F").arg(qRound(imperialData.feelsLike)));
        }
        if (pressureLabel) {
            pressureLabel->setText(QString("Давление: %1 psi").arg(qRound(imperialData.pressure * 10) / 10.0));
        }
        if (windLabel) {
            windLabel->setText(QString("Ветер: %1 mph").arg(qRound(imperialData.windSpeed * 10) / 10.0));
        }
    } else {
        tempValueLabel->setText(QString::number(qRound(data.temperature)) + "°C");

        if (feelsLikeLabel) {
            feelsLikeLabel->setText(QString("Ощущается как: %1°C").arg(qRound(data.feelsLike)));
        }
        if (pressureLabel) {
            pressureLabel->setText(QString("Давление: %1 hPa").arg(qRound(data.pressure)));
        }
        if (windLabel) {
            windLabel->setText(QString("Ветер: %1 m/s").arg(qRound(data.windSpeed * 10) / 10.0));
        }
    }

    if (weatherDescLabel) {
        weatherDescLabel->setText(data.weatherDescription);
    }

    if (humidityLabel) {
        humidityLabel->setText(QString("Влажность: %1%").arg(qRound(data.humidity)));
    }


    if (tempIconLabel) {
        QString iconText = getWeatherIcon(data.weatherIcon);
        tempIconLabel->setText(iconText);
        tempIconLabel->setStyleSheet("font-size: 64px;");
    }
}

QString MainWindow::getWeatherIcon(const QString &iconCode)
{

    if (iconCode.startsWith("01")) return "☀️"; // clear sky
    if (iconCode.startsWith("02")) return "⛅"; // few clouds
    if (iconCode.startsWith("03") || iconCode.startsWith("04")) return "☁️"; // clouds
    if (iconCode.startsWith("09") || iconCode.startsWith("10")) return "🌧️"; // rain
    if (iconCode.startsWith("11")) return "⛈️"; // thunderstorm
    if (iconCode.startsWith("13")) return "❄️"; // snow
    if (iconCode.startsWith("50")) return "🌫️"; // mist

    return "🌈";
}

void MainWindow::onErrorOccurred(const QString &errorMessage)
{
    QMessageBox::critical(this, "Ошибка", errorMessage);
    clearWeatherData();
}

void MainWindow::updateDateTime()
{
    if (dateTimeLabel) {
        dateTimeLabel->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    }
}

void MainWindow::clearWeatherData()
{
    if (cityLabel) {
        cityLabel->setText("Город не выбран");
    }
    if (tempValueLabel) {
        tempValueLabel->setText("--°C");
    }
    if (weatherDescLabel) {
        weatherDescLabel->setText("");
    }
    if (feelsLikeLabel) {
        feelsLikeLabel->setText("Ощущается как: --");
    }
    if (humidityLabel) {
        humidityLabel->setText("Влажность: --");
    }
    if (pressureLabel) {
        pressureLabel->setText("Давление: --");
    }
    if (windLabel) {
        windLabel->setText("Ветер: --");
    }
    if (tempIconLabel) {
        tempIconLabel->setText("🌍");
        tempIconLabel->setStyleSheet("font-size: 64px;");
    }
}
