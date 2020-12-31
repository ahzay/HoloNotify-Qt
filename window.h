#ifndef WINDOW_H
#define WINDOW_H
#include <QLabel>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QSettings>
#include "tubers.h"
QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE
class GenTab : public QWidget {
    Q_OBJECT

	public:
        explicit GenTab(int num, QString names[], QWidget *parent = nullptr);
        tuber ** getTuberList();
        ~GenTab();
    private:
		int _num;
        tuber ** tuberList;
};
class infoWidget : public QWidget{
    Q_OBJECT

    public:
        explicit infoWidget(QWidget *parent = nullptr);
    private:
        QLabel *name_en, *name_jp, *twitter, *youtube, *ytName,
                *debutDate, *bday, *Vheight, *fanName, *illust, *imgFBLabel;
        std::string holoInfo;
        rapidjson::Document document;
    public slots:
        void displayWhenHovered(QString name);
};
class TabDialog : public QDialog {
    Q_OBJECT

    public:
        explicit TabDialog(QWidget *parent = nullptr);
        QSystemTrayIcon * getTrayIcon();
        QMenu * getTrayIconMenu();
        QSettings * getSettings();
        //~TabDialog();
	private:
         QTabWidget *tabWidget;
         QSystemTrayIcon *trayIcon;
         infoWidget * info;
         QMenu *trayIconMenu;
         QSettings *mySettings;
    protected:
         virtual void closeEvent(QCloseEvent *event)override;
    private slots:
         void iconActivated(QSystemTrayIcon::ActivationReason reason);
         //void setSettings();
};





#endif // WINDOW_H
