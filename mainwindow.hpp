
#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include "weatherservice.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();
    void onUnitsChanged(bool checked);
    void onWeatherDataReceived(const WeatherData &data);
    void onErrorOccurred(const QString &errorMessage);
    void updateDateTime();
    void onRefreshClicked();

private:
    Ui::MainWindow *ui;
    WeatherService *weatherService;
    QTimer *updateTimer;
    bool isImperial;


    QLineEdit *cityLineEdit;
    QLabel *cityLabel;
    QLabel *dateTimeLabel;
    QLabel *tempValueLabel;
    QLabel *tempIconLabel;
    QLabel *weatherDescLabel;
    QLabel *feelsLikeLabel;
    QLabel *humidityLabel;
    QLabel *pressureLabel;
    QLabel *windLabel;
    QRadioButton *metricRadio;
    QRadioButton *imperialRadio;
    QGroupBox *detailsGroup;
    QHBoxLayout *tempLayout;

    void setupUI();
    void displayWeatherData(const WeatherData &data);
    void clearWeatherData();
    QString getWeatherIcon(const QString &iconCode);
};

