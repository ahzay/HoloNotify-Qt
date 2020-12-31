#include "window.h"
GenTab::GenTab(int num, QString* names,QWidget *parent) :
	QWidget(parent){
        tuberList =(tuber**)(malloc(sizeof(tuber*)*num));
		_num = num;
		int buttonWidth = 100;
		int separator = 10;
        setFixedSize(separator+(buttonWidth+separator)*(num), buttonWidth + 2*separator);
		for(int i=0;i<num;i++){
            tuberList[i] = new tuber(names[i]);
            tuberList[i]->createButton(separator+(buttonWidth+separator)*i,separator,buttonWidth,buttonWidth,this);
		}
	}
tuber ** GenTab::getTuberList(){
    return tuberList;
}
GenTab::~GenTab(){
	for(int i=0;i<_num;i++){
        delete tuberList[i];
	}
}

infoWidget::infoWidget(QWidget *parent) :
    QWidget(parent){
        //read holoinfo.json into holoInfo string
        std::ifstream t("/home/user/Documents/QtProjects/HoloNotify-Qt/hololive.json");
        std::stringstream buffer;
        buffer << t.rdbuf();
        holoInfo = buffer.str();
        // parse into document
        document.Parse(holoInfo.c_str());
        //
        name_en = new QLabel(this);
        name_en ->setTextInteractionFlags(Qt::TextSelectableByMouse);
        name_jp = new QLabel(this);
        name_jp->setTextInteractionFlags(Qt::TextSelectableByMouse);
        twitter = new QLabel(this);
        twitter->setTextInteractionFlags(Qt::TextSelectableByMouse);
        youtube = new QLabel(this);
        youtube->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ytName = new QLabel(this);
        ytName->setTextInteractionFlags(Qt::TextSelectableByMouse);
        debutDate = new QLabel(this);
        debutDate->setTextInteractionFlags(Qt::TextSelectableByMouse);
        bday = new QLabel(this);
        bday->setTextInteractionFlags(Qt::TextSelectableByMouse);
        Vheight = new QLabel(this);
        Vheight->setTextInteractionFlags(Qt::TextSelectableByMouse);
        fanName = new QLabel(this);
        fanName->setTextInteractionFlags(Qt::TextSelectableByMouse);
        illust = new QLabel(this);
        illust->setTextInteractionFlags(Qt::TextSelectableByMouse);
        QVBoxLayout *infoLayout = new QVBoxLayout;
        infoLayout->addWidget(name_en);
        infoLayout->addWidget(name_jp);
        infoLayout->addWidget(twitter);
        infoLayout->addWidget(youtube);
        infoLayout->addWidget(ytName);
        infoLayout->addWidget(debutDate);
        infoLayout->addWidget(bday);
        infoLayout->addWidget(Vheight);
        infoLayout->addWidget(fanName);
        infoLayout->addWidget(illust);
        infoLayout->setAlignment(Qt::AlignTop);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addLayout(infoLayout);
        imgFBLabel = new QLabel;
        layout ->addWidget(imgFBLabel);
        imgFBLabel->setFixedSize(220,540);
        //imgFBLabel->setAlignment(Qt::AlignHCenter);
        imgFBLabel->setAlignment(Qt::AlignBottom);
        //layout->addSpacing(50);
        setLayout(layout);
        name_en->setText("Hover to display information");
}
void infoWidget::displayWhenHovered(QString name){
    QString toSet = name;
    toSet.prepend("Name (English): ").replace('_',' ');
    name_en->setText(toSet);
    toSet = name;
    const rapidjson::Value& channels = document["channels"];
    assert(channels.IsArray());
    for (rapidjson::Value::ConstValueIterator itr = channels.Begin(); itr != channels.End(); ++itr){
        const rapidjson::Value& channel = *itr;
        /*assert(channel.IsObject());
        assert(channel["name_en"].IsString());
        assert(channel["name_jp"].IsString());
        assert(channel["twitter"].IsString());
        assert(channel["youtube"].IsString());
        assert(channel["ytName"].IsString());
        assert(channel["bio"].IsObject());
        assert(channel["bio"]["debutDate"].IsString());
        assert(channel["bio"]["bday"].IsString());
        assert(channel["bio"]["height"].IsString());
        assert(channel["bio"]["fanName"].IsString());
        assert(channel["bio"]["illust"].IsString());*/
        if(toSet.replace('_',' ').compare(channel["name_en"].GetString())==0){
            toSet = channel["name_jp"].GetString();
            name_jp->setText(toSet.prepend("Name (Japanese): "));
            toSet = channel["twitter"].GetString();
            twitter->setText(toSet.prepend("Twitter Handle: "));
            toSet = channel["youtube"].GetString();
            youtube->setText(toSet.prepend("Youtube Channel ID: "));
            toSet = channel["ytName"].GetString();
            ytName->setText(toSet.prepend("Channel Name: "));
            toSet = channel["bio"]["debutDate"].GetString();
            debutDate->setText(toSet.prepend("Debut Date: "));
            toSet = channel["bio"]["bday"].GetString();
            bday->setText(toSet.prepend("Birthday: "));
            toSet = channel["bio"]["height"].GetString();
            Vheight->setText(toSet.prepend("Height: "));
            toSet = channel["bio"]["fanName"].GetString();
            fanName->setText(toSet.prepend("Fan Name: "));
            toSet = channel["bio"]["illust"].GetString();
            illust->setText(toSet.prepend("Illustrator: "));
            QString  path("/home/user/Documents/QtProjects/HoloNotify-Qt/FB/");
            path.append(name);
            path.append("_FB.png");
            QPixmap * pixmap = new QPixmap(path);
            //*pixmap = pixmap->scaled(300,800,Qt::KeepAspectRatio);
            imgFBLabel->setPixmap(*pixmap);
            break;
        }

    }

}

TabDialog::TabDialog(QWidget *parent):
	QDialog(parent){
        mySettings = new QSettings;
        this->setWindowIcon(QIcon("/home/user/Documents/QtProjects/HoloNotify-Qt/HoloNotify.ico"));
        // tray icon
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon("/home/user/Documents/QtProjects/HoloNotify-Qt/HoloNotify.ico"));
        trayIconMenu = new QMenu(this);
        //trayIconMenu->setLayoutDirection(Qt::LayoutDirection::)
        trayIcon->setContextMenu(trayIconMenu);
        connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
        //
        tabWidget = new QTabWidget(this);
        tabWidget->setMaximumHeight(155);
        // gen names
        int jpGenL[7] = {5,5,5,3,5,5,4};
        QString genZeroJpNames[5] = {"Sakura_Miko","Hoshimachi_Suisei","Tokino_Sora","Roboco","AZKi"};
        QString genOneJpNames[5] = {"Yozora_Mel","Shirakami_Fubuki","Natsuiro_Matsuri","Aki_Rosenthal","Akai_Haato"};
        QString genTwoJpNames[5] = {"Minato_Aqua","Murasaki_Shion","Nakiri_Ayame","Yuzuki_Choco","Oozora_Subaru"};
        QString gamersJpNames[3] = {"Ookami_Mio","Nekomata_Okayu","Inugami_Korone"};
        QString genThreeJpNames[5] = {"Usada_Pekora","Uruha_Rushia","Shiranui_Flare","Shirogane_Noel","Houshou_Marine"};
        QString genFourJpNames[5] = {"Amane_Kanata","Kiryu_Coco","Tsunomaki_Watame","Tokoyami_Towa","Himemori_Luna"};
        QString genFiveJpNames[4] = {"Yukihana_Lamy","Momosuzu_Nene","Shishiro_Botan","Omaru_Polka"};
        QString * allJpGenNames[7] = {genZeroJpNames,genOneJpNames,genTwoJpNames,gamersJpNames,genThreeJpNames,genFourJpNames,genFiveJpNames};
        // declare gen tabs
        GenTab * allGenTabs[7];
        for(unsigned long i=0;i<sizeof(allJpGenNames)/sizeof(QString*);i++){
                allGenTabs[i] = new GenTab(jpGenL[i],allJpGenNames[i],parent);
        }
        // adding gen tabs
        tabWidget->addTab(allGenTabs[0], tr("Gen 0"));
        tabWidget->addTab(allGenTabs[1], tr("Gen 1"));
        tabWidget->addTab(allGenTabs[2], tr("Gen 2"));
        tabWidget->addTab(allGenTabs[3], tr("GAMERS"));
        tabWidget->addTab(allGenTabs[4], tr("Gen 3"));
        tabWidget->addTab(allGenTabs[5], tr("Gen 4"));
        tabWidget->addTab(allGenTabs[6], tr("Gen 5"));
        // declare and configure bottom info widget
        info = new infoWidget(this);
        for(unsigned long i=0;i<sizeof(allJpGenNames)/sizeof(QString*);i++)
            for(int j=0;j<jpGenL[i];j++)
                connect(allGenTabs[i]->getTuberList()[j]->getButton(),SIGNAL(hovered(QString)),info,SLOT(displayWhenHovered(QString)));
        // declare and configure api fetching
        apiFetcher * fetcher = new apiFetcher(10000);
        for(unsigned long i=0;i<sizeof(allJpGenNames)/sizeof(QString*);i++)
            for(int j=0;j<jpGenL[i];j++)
                connect(fetcher,SIGNAL(sendStatus(QString , QString)),
                        allGenTabs[i]->getTuberList()[j]->getButton(),SLOT(streamingNotify(QString, QString)));

        // declare and configure layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(tabWidget);
        mainLayout->addWidget(info);
        setLayout(mainLayout);
}
void TabDialog::closeEvent(QCloseEvent *event){
    QMessageBox::information(this, tr("Systray"),
                             tr("The program will keep running in the "
                                "system tray. To terminate the program, "
                                "choose <b>Exit Application</b> in the context menu "
                                "of the system tray entry."));
    hide();
    trayIcon->show();
    event->ignore();
}

void TabDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        TabDialog::show();
        break;
    case QSystemTrayIcon::MiddleClick:
        TabDialog::show();
        break;
    default:
        ;
    }
}

QSystemTrayIcon * TabDialog::getTrayIcon(){
    return trayIcon;
}
QSettings * TabDialog::getSettings(){
    return mySettings;
}
QMenu * TabDialog::getTrayIconMenu(){
    return trayIconMenu;
}

