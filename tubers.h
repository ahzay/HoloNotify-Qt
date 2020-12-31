#ifndef TUBER_H
#define TUBER_H
#include <QWidget>
#include<KNotification>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtDebug>
//json stuff
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"
//
#include <QPushButton>
#include <string.h>

class tuberButton : public QPushButton{
                Q_OBJECT
        public: 
                tuberButton(const QString name,const QString youtube,int x, int y, int w, int h,QWidget *parent = nullptr);
                virtual ~tuberButton();
        private:
                QString _name,_youtube;
                QPixmap *pixmap;
                QIcon *ButtonIcon;
                QString status;
        protected:
                virtual void enterEvent(QEvent * event) override;
        signals:
                void hovered(QString name);
        public slots:
                void streamingNotify(QString youtube, QString newStatus);
};

class tuber : public QObject{
        public:
                tuber(const QString name);
                QString getName();
                const char *getImgPath();
                tuberButton *getButton();
                void createButton(int x,int y,int w,int h,QWidget *parent = nullptr);
        private:
                tuberButton *_button;
                rapidjson::Document document;
                std::string holoInfo;
                QString _name,_youtube;
                const char *_imgPath;

}; 

class apiFetcher : public QObject {
                Q_OBJECT
        public:
                apiFetcher(int timeOut);
                std::string getHoloStatus();
        private:
                int _timeOut;
                std::string holoStatus;
                rapidjson::Document document;
        protected:
                virtual void timerEvent(QTimerEvent *event) override;
        public slots:
                void serviceRequestFinished(QNetworkReply* reply);
        signals:
                void sendStatus(QString youtube, QString newStatus);

};
#endif // TUBER_H

