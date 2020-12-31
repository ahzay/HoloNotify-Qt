#include "tubers.h"
tuberButton::tuberButton(const QString name,const QString youtube,int x, int y, int w, int h,QWidget *parent) :
        QPushButton("", parent){
                _name = name;
                _youtube = youtube;
                status = "past";
                setGeometry(x, y, w, h);
                setMouseTracking(true);
                QString  path("/home/user/Documents/QtProjects/HoloNotify-Qt/portraits/");
                path.append(name);
                path.append("_P.png");
                pixmap = new QPixmap(path);
                ButtonIcon = new QIcon(*pixmap);
                setIcon(*ButtonIcon);
                setIconSize(QSize(w-10,h-10));
        }
tuberButton::~tuberButton(){}
void tuberButton::enterEvent(QEvent *event){
    emit hovered(_name);
    QWidget::enterEvent(event);
}
void tuberButton::streamingNotify(QString youtube, QString newStatus){
    if(isChecked()){
        if(youtube.compare(_youtube)==0){
            if(status.compare(newStatus)!=0){
                status = newStatus;
                std::ostringstream oss;
                oss << "The VTuber "<<_name.toUtf8().constData()<<" has changed status to "<<status.toUtf8().constData()<<"!";
                // pixmap in notification does not work
                KNotification::event( KNotification::Notification,
                                                   oss.str().c_str(),*pixmap,this,KNotification::DefaultEvent);
            }
        }
    }
}
//////////////////////////////////
tuber::tuber(const QString name){
	_name = name;
    // begin timer, check for updates every 5 minutes
    startTimer(5*60000);
    //read holoinfo.json into holoInfo string
    std::ifstream t("/home/user/Documents/QtProjects/HoloNotify-Qt/hololive.json");
    std::stringstream buffer;
    buffer << t.rdbuf();
    holoInfo = buffer.str();
    // parse into document
    document.Parse(holoInfo.c_str());
    const rapidjson::Value& channels = document["channels"];
    assert(channels.IsArray());
    QString toSet = name;
    for (rapidjson::Value::ConstValueIterator itr = channels.Begin(); itr != channels.End(); ++itr){
            const rapidjson::Value& channel = *itr;
            assert(channel.IsObject());
            assert(channel["name_en"].IsString());
            if(toSet.replace('_',' ').compare(channel["name_en"].GetString())==0){
                _youtube=channel["youtube"].GetString();
            }
    }
}
void tuber::createButton(int x,int y,int w,int h,QWidget *parent){
    _button = new tuberButton(_name,_youtube,x,y,w,h,parent);
	_button->setCheckable(true);
	_button->setStyleSheet("QPushButton:checked{background-color:green;}");
	_button->setFocusPolicy(Qt::NoFocus);
    _button->setToolTip(_name.replace('_',' '));
}
const char * tuber::getImgPath(){
	return _imgPath;
}
QString tuber::getName(){
	return _name;
}
tuberButton * tuber::getButton(){
	return _button;
}
/////////////////////////////////////
apiFetcher::apiFetcher(int timeOut){
    _timeOut=timeOut;
    startTimer(_timeOut);
}
void apiFetcher::timerEvent(QTimerEvent *event){
    qDebug()<<event;
    QUrl serviceUrl = QUrl("https://api.holotools.app/v1/live");
    QNetworkAccessManager * manager = new QNetworkAccessManager();
    QNetworkRequest request(serviceUrl);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(serviceRequestFinished(QNetworkReply*)));
    manager->get(request);
}
void apiFetcher::serviceRequestFinished(QNetworkReply* reply){
    QByteArray buffer = reply->readAll();
    holoStatus = buffer.toStdString();
    document.Parse(buffer.toStdString().c_str());
    const rapidjson::Value& liveChannels = document["live"];
    assert(liveChannels.IsArray());
    for (rapidjson::Value::ConstValueIterator itr = liveChannels.Begin(); itr != liveChannels.End(); ++itr){
            const rapidjson::Value& liveChannel = *itr;
            emit sendStatus(liveChannel["channel"]["yt_channel_id"].GetString(),liveChannel["status"].GetString());
    }
    const rapidjson::Value& pastChannels = document["ended"];
    for (rapidjson::Value::ConstValueIterator itr = pastChannels.Begin(); itr != pastChannels.End(); ++itr){
            const rapidjson::Value& pastChannel = *itr;
            emit sendStatus(pastChannel["channel"]["yt_channel_id"].GetString(),pastChannel["status"].GetString());
    }
}




