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
        void addToClipboard();
        void filterItems();

    private:
        void createSystemTray();
        void createMainUI();
        void toggleVisibility();

        QSystemTrayIcon *trayIcon_;
        QMenu *trayMenu_;
        QAction *showAction;
        QAction *filterAction;
        QAction *quitAction;

        QClipboard *clipboard_;
        QLineEdit *lineEdit_;
        QListWidget *list_;
        QList<QListWidgetItem*> items;

        QPoint mpos;

    protected:
        void mousePressEvent(QMouseEvent *event) override
        {
            mpos = event->pos();
        }

        void mouseMoveEvent(QMouseEvent *event) override
        {
            if(event->buttons() & Qt::LeftButton)
            {
                QPoint diff = event->pos() - mpos;
                QPoint newpos = this->pos() + diff;

                this->move(newpos);
            }
        }
};

#endif