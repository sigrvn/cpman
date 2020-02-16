#ifndef CPMAN_H
#define CPMAN_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QClipboard>
#include <QProcess>

class QTextBrowser;

class Cpman : public QWidget
{
    Q_OBJECT

    public:
        explicit Cpman(QWidget *parent = 0);
        ~Cpman();

    private slots:
        void addToClipboard();
        void showFullClipboard();
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

    private:
        QSystemTrayIcon *trayIcon_;
        QMenu *trayMenu_;

        QAction *showFullAction;

        QClipboard *clipboard_;
        QTextBrowser *textBrowser_;

        std::vector<QString> items;
};

#endif