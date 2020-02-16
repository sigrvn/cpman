#include <QtWidgets>
#include <QString>
#include "cpman.h"

Cpman::Cpman(QWidget *parent) : QWidget(parent)
{
    trayIcon_ = new QSystemTrayIcon();
    trayIcon_->setIcon(QIcon("favicon-16x16.png"));
    trayIcon_->setVisible(true);
    trayIcon_->show();

    trayMenu_ = new QMenu();

    showFullAction = new QAction(tr("Show Full Clipboard"));
    showFullAction->setShortcut(Qt::CTRL + Qt::Key_F8);
    connect(showFullAction, &QAction::triggered, this, &Cpman::showFullClipboard);

    trayMenu_->addAction(showFullAction);
    trayIcon_->setContextMenu(trayMenu_);

    clipboard_ = QGuiApplication::clipboard();
    textBrowser_ = new QTextBrowser();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(textBrowser_, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("cpman"));

    connect(trayIcon_, &QSystemTrayIcon::activated, this, &Cpman::iconActivated);

    // NOTE: Due to a Chromium bug, text copied from Chromium address bars fire dataChanged signal twice
    connect(clipboard_, &QClipboard::dataChanged, this, &Cpman::addToClipboard);
}

Cpman::~Cpman()
{
    delete textBrowser_;
}

// SLOTS

void Cpman::addToClipboard()
{
    QString str = clipboard_->text();
    textBrowser_->append(str);
}

void Cpman::showFullClipboard()
{
    Cpman::show();
}

void Cpman::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug("Reason: %d", reason);
    trayMenu_->show();
}
