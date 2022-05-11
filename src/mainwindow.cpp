#include "mainwindow.h"
#include "ui_mainwindow.h"

static QString SELECTED = "";
static QString SELECTED2 = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    toast = new QMessageBox(this);

    empty = new QWidget();

    countries = new QStringList();
    regions = new QStringList();
    subdivisions = new QStringList();
    languages = new QStringList();
    categories = new QStringList();
    id_con = new QStringList();
    icon_con = new QStringList();

    label = new QLabel(this);

    objlist = {
        countries,
        regions,
        subdivisions,
        languages,
        categories,
    };

    sortby = new QComboBox(this);
    connect(sortby, SIGNAL(currentIndexChanged(int)), this, SLOT(sortby_changed(int)));

    sortbyopt = new QComboBox(this);
    sortbyopt->setMinimumWidth(120);
    connect(sortbyopt, SIGNAL(currentIndexChanged(int)), this, SLOT(sortbyopt_changed(int)));
    sortbyopt->addItem("Subcategory");

    channels = new QComboBox(this);
    connect(channels, SIGNAL(currentIndexChanged(int)), this, SLOT(channels_changed(int)));
    channels->addItem("Channels");

    slider = new QSlider(Qt::Horizontal, this);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slider_changed(int)));

    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->ddcontainer->addWidget(sortby);
    ui->ddcontainer->addWidget(sortbyopt);
    ui->ddcontainer->addWidget(channels);
    ui->ddcontainer->addSeparator();
    ui->ddcontainer->addWidget(label);
    ui->mainToolBar->addWidget(empty);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(slider);
    ui->mainToolBar->addSeparator();

    sortby->addItems(list);

    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    channels->setFixedWidth(150);
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);
    this->setWindowTitle("IPTV Player");

    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    slider->setValue(player->volume());
    if(sortbyopt->count() <= 1){
        ui->actionprevious->setEnabled(false);
        ui->actionnext->setEnabled(false);
        ui->actionplay->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete sortby;
    delete channels;
    delete sortbyopt;
    delete countries;
    delete languages;
    delete subdivisions;
    delete categories;
    delete regions;
    delete id_con;
    delete icon_con;
    delete label;
    delete player;
    delete vw;
    delete toast;
    delete ui;
}

void MainWindow::fetch_sorters_onfinish0(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonObject obj1;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();

    sortbyopt->clear();
    countries->clear();


    foreach(QJsonValue obj, jsonArray){
        obj1 = obj.toObject();
        sortbyopt->addItem(obj1.value("name").toString() + " " + obj1.value("flag").toString());
        countries->append(obj1.value("code").toString());
    }
}

void MainWindow::fetch_sorters_onfinish1(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();
    sortbyopt->clear();
    regions->clear();

    foreach(QJsonValue value, jsonArray){
        foreach(QJsonValue subvalue, value.toObject().value("countries").toArray()){
            sortbyopt->addItem(value.toObject().value("name").toString() + " (" + subvalue.toString() + ")");
            regions->append(subvalue.toString());
        }
    }
    // qDebug() << regions->toStdList();
}

void MainWindow::fetch_sorters_onfinish2(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonObject sampleobj;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();

    sortbyopt->clear();
    subdivisions->clear();

    foreach(QJsonValue value, jsonArray){
        sampleobj = value.toObject();
        sortbyopt->addItem(sampleobj.value("name").toString() + " (" + sampleobj.value("country").toString() + ")");
        subdivisions->append(sampleobj.value("code").toString());
    }
}

void MainWindow::fetch_sorters_onfinish3(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonObject sampleobj;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();

    sortbyopt->clear();
    languages->clear();

    foreach(QJsonValue value, jsonArray){
        sampleobj = value.toObject();
        sortbyopt->addItem(sampleobj.value("name").toString());
        languages->append(sampleobj.value("code").toString());
    }
}

void MainWindow::fetch_sorters_onfinish4(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonObject sampleobj;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();

    sortbyopt->clear();
    categories->clear();

    foreach(QJsonValue value, jsonArray){
        sampleobj = value.toObject();
        sortbyopt->addItem(sampleobj.value("name").toString());
        categories->append(sampleobj.value("id").toString());
    }
}

void MainWindow::fetch_sorters_onfinish5(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts), temp;
    QJsonObject sampleobj;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();
    QJsonArray arrobj = {};
    QJsonValue v3;

    id_con->clear();
    channels->clear();

    foreach(QJsonValue value, jsonArray){
        sampleobj = value.toObject();
        temp = Subdir[sortby->currentIndex()-1];
        if(temp == "regions") return;
        if(temp == "countries"){
            temp = "country";
            v3 = sampleobj.value(temp);
            if(v3.toString() == SELECTED){
                channels->addItem(sampleobj.value("name").toString());
                id_con->append(sampleobj.value("id").toString());
                icon_con->append(sampleobj.value("logo").toString());
            }
            continue;
        }

        if(temp == "subdivisions"){
            temp = "subdivision";
            v3 = sampleobj.value(temp);
            if(v3.toString() == SELECTED){
                channels->addItem(sampleobj.value("name").toString());
                id_con->append(sampleobj.value("id").toString());
                icon_con->append(sampleobj.value("logo").toString());
            }
            continue;
        }

        arrobj = sampleobj.value(temp).toArray();
        foreach(QJsonValue subvalue, arrobj){
            //qDebug() << subvalue;
            if(subvalue.toString() == SELECTED){
                channels->addItem(sampleobj.value("name").toString());
                id_con->append(sampleobj.value("id").toString());
                icon_con->append(sampleobj.value("logo").toString());
            }
        }
    }
}

void MainWindow::fetch_sorters_onfinish6(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts), referer, user_agent;
    QJsonObject sampleobj;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray jsonArray = document.array();
    QNetworkRequest request;

    foreach(QJsonValue value, jsonArray){
        sampleobj = value.toObject();
        if(sampleobj.value("channel").toString() == SELECTED2){

            request = QNetworkRequest(QUrl(sampleobj.value("url").toString()));
            referer = sampleobj.value("http_referrer").toString();
            user_agent = sampleobj.value("user_agent").toString();

            if(referer != QJsonValue::Null){
                request.setRawHeader("Referer", QUrl(referer).toEncoded());
            }

            if(user_agent != QJsonValue::Null){
                request.setRawHeader("User-Agent", QUrl(user_agent).toEncoded());
            }

            player->setMedia(QMediaContent(request));

            vw->resize(sampleobj.value("width").toInt(), sampleobj.value("height").toInt());
            player->setPlaybackRate(sampleobj.value("bitrate").toInt());
            label->setText("Status: " + sampleobj.value("status").toString());
            this->setWindowTitle(this->channels->currentText());
            return;
        }
    }
    toast->setText("Not accessible");
    toast->setInformativeText("This channel is not accessible as of now. Please try again later!");
    toast->exec();
    this->setWindowTitle("IPTV Player");
}

void MainWindow::fetch_sorters(QString sd, int tidx){
    QUrl url = QUrl(IPTV_API + "/" + sd + ".json");
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    switch (tidx) {
    case 0:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish0(QNetworkReply*)));
        break;
    case 1:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish1(QNetworkReply*)));
        break;
    case 2:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish2(QNetworkReply*)));
        break;
    case 3:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish3(QNetworkReply*)));
        break;
    case 4:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish4(QNetworkReply*)));
        break;
    case 5:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish5(QNetworkReply*)));
        break;
    case 6:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish6(QNetworkReply*)));
        break;
    default:
        connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(fetch_sorters_onfinish0(QNetworkReply*)));
        break;

    }
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));
    QNetworkRequest request(url);
    mgr->get(request);
}

void MainWindow::sortby_changed(int idx)
{
    if(idx-1 < 0) return;
    fetch_sorters(Subdir[idx-1], idx-1);
    if(channels->count() > 1){
        ui->actionprevious->setEnabled(true);
        ui->actionnext->setEnabled(true);
        ui->actionplay->setEnabled(true);
    }
    else{
        ui->actionprevious->setEnabled(false);
        ui->actionnext->setEnabled(false);
        ui->actionplay->setEnabled(false);
    }
}

void MainWindow::sortbyopt_changed(int idx){
    if(idx < 0 || sortby->currentIndex() <= 0 || sortbyopt->currentIndex() <= 0) return;
    SELECTED =  QStringList::fromStdList(objlist[sortby->currentIndex()-1]->toStdList())[idx];
    fetch_sorters(Subdir[5], 5);
}

void MainWindow::channels_changed(int idx){
    label->clear();
    if(idx < 0 || sortby->currentIndex() <= 0 || sortbyopt->currentIndex() <= 0 || channels->currentIndex() <= 0) return;
    SELECTED2 = QStringList::fromStdList(id_con->toStdList())[idx];
    QString icon = QStringList::fromStdList(icon_con->toStdList())[idx];
    QPixmap pix(icon);
    this->setWindowIcon(QIcon(pix));
    fetch_sorters(Subdir[6], 6);
    if(channels->count() > 1){
        ui->actionprevious->setEnabled(true);
        ui->actionnext->setEnabled(true);
        ui->actionplay->setEnabled(true);
    }
    if(channels->currentIndex() <= 1){
        ui->actionprevious->setEnabled(false);
        idx = 1;
    }
    if (channels->currentIndex() >= channels->count()-1) {
        ui->actionnext->setEnabled(false);
        idx = channels->count();
    }
    channels->setCurrentIndex(idx);
}

void MainWindow::slider_changed(int vol){
    player->setVolume(vol);
}

void MainWindow::on_actionprevious_triggered()
{
    int curidx = channels->currentIndex();
    channels_changed(curidx - 1);
}

void MainWindow::on_actionplay_triggered()
{
    if(ui->actionplay->text()=="play"){
        ui->actionplay->setIcon(QIcon(":/icons/icons8-pause-button-96.png"));
        ui->actionplay->setText("pause");
        player->play();
        return;
    }
    ui->actionplay->setIcon(QIcon(":/icons/play.png"));
    ui->actionplay->setText("play");
    player->pause();
}

void MainWindow::on_actionnext_triggered()
{
    int curidx = channels->currentIndex();
    channels_changed(curidx + 1);
}

void MainWindow::on_actionmute_triggered()
{
    if(ui->actionmute->text()=="mute"){
        ui->actionmute->setIcon(QIcon(":/icons/icons8-audio-64.png"));
        ui->actionmute->setText("unmute");
        player->setMuted(true);
        return;
    }
    ui->actionmute->setIcon(QIcon(":/icons/icons8-mute-64.png"));
    ui->actionmute->setText("mute");
    player->setMuted(false);
}


