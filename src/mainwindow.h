#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QNetworkAccessManager>
#include <QMainWindow>
#include <QComboBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QColorDialog>

static const QString IPTV_API = "https://iptv-org.github.io/api";

static const QStringList list = {
    "Sort by",
    "Countries",
    "Regions",
    "Subdivisions",
    "Languages",
    "Categories"
};

static const QStringList Subdir = {
    "countries",
    "regions",
    "subdivisions",
    "languages",
    "categories",
    "channels",
    "streams",
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionprevious_triggered();

    void on_actionplay_triggered();

    void on_actionnext_triggered();

    void on_actionmute_triggered();

    void fetch_sorters_onfinish0(QNetworkReply*);

    void fetch_sorters_onfinish1(QNetworkReply*);

    void fetch_sorters_onfinish2(QNetworkReply*);

    void fetch_sorters_onfinish3(QNetworkReply*);

    void fetch_sorters_onfinish4(QNetworkReply*);

    void fetch_sorters_onfinish5(QNetworkReply*);

    void fetch_sorters_onfinish6(QNetworkReply*);

    //void fetch_sorters_onfinish7(QNetworkReply*);

    void fetch_sorters(QString, int);

    void sortby_changed(int);

    void sortbyopt_changed(int);

    void channels_changed(int);

    void slider_changed(int);
private:
    Ui::MainWindow *ui;
    QComboBox *sortby, *channels, *sortbyopt;
    QStringList *countries, *languages, *subdivisions, *categories, *regions, *id_con, *icon_con;
    QList<QStringList*> objlist;
    QLabel *label;
    QMediaPlayer *player;
    QVideoWidget *vw;
    QMessageBox *toast;
    QSlider *slider;
    QWidget* empty;
};

#endif // MAINWINDOW_H
