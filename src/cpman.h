#ifndef CPMAN_H
#define CPMAN_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QClipboard>
#include <QProcess>

class QLineEdit;
class QListWidget;
class QListWidgetItem;

class Cpman : public QWidget
{
    Q_OBJECT

    public:
        explicit Cpman(QWidget *parent = 0);
        ~Cpman();

    private slots:
        void addToClipboard();
        void filterItems();

    private:
        void createSystemTray();
        void createMainUI();
        QSystemTrayIcon *trayIcon_;
        QMenu *trayMenu_;
        QAction *showFullAction;
        QAction *quitAction;

        QClipboard *clipboard_;
        QLineEdit *lineEdit_;
        QListWidget *list_;
        QList<QListWidgetItem*> items;
};

#endif