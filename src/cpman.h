#ifndef CPMAN_H
#define CPMAN_H

#include <QtWidgets>
#include <QString>
#include <QHotkey>

class QSystemTrayIcon;
class QMenu;
class QLineEdit;
class QListWidget;
class QListWidgetItem;

class Cpman : public QWidget
{
    Q_OBJECT

    public:
        explicit Cpman(QWidget *parent = 0);
        ~Cpman();

        void registerHotkeys(QApplication &a);

    private slots:
        //void pasteItem();
        void addToClipboard();
        void filterItems();

    private:
        void createSystemTray();
        void createMainUI();

        QSystemTrayIcon *trayIcon_;
        QMenu *trayMenu_;
        QAction *showAction;
        QAction *filterAction;
        QAction *quitAction;

        QClipboard *clipboard_;
        QLineEdit *lineEdit_;
        QListWidget *list_;
        QList<QListWidgetItem*> items;
};

#endif