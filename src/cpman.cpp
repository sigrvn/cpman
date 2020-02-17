#include <QtWidgets>
#include <QString>
#include "cpman.h"

// TODO: add global hotkey to open/close cpman window
Cpman::Cpman(QWidget *parent) : QWidget(parent)
{
    createSystemTray();
    createMainUI();

    clipboard_ = QGuiApplication::clipboard();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(lineEdit_, 0, 0);
    mainLayout->addWidget(list_, 1, 0);
    mainLayout->addWidget(label_, 2, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("cpman"));

    connect(trayIcon_, &QSystemTrayIcon::activated, this, [=]() { trayMenu_->show(); } );

    // NOTE: Due to a Chromium bug, text copied from Chromium address bars fire dataChanged signal twice
    connect(clipboard_, &QClipboard::dataChanged, this, &Cpman::addToClipboard);
    connect(lineEdit_, &QLineEdit::textChanged, this, &Cpman::filterItems);
    
    connect(list_, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) { 
        clipboard_->setText(item->text());
        label_->setText("Currently copied: " + item->text()); 
    });
}

Cpman::~Cpman()
{
    delete lineEdit_;
    delete list_;
}

// SLOTS

void Cpman::addToClipboard()
{
    QString str = clipboard_->text().toLatin1();
    qDebug().nospace() << "copied " << qPrintable(str);

    // Don't add repeats
    for (int i = 0; i < items.size(); ++i)
    {
        if (items.at(i)->text() == str) return;
    }

    QListWidgetItem *newItem = new QListWidgetItem(str);

    items.push_back(newItem);
    list_->addItem(newItem);

    label_->setText("Currently copied: " + str); 
}

void Cpman::filterItems()
{
    QString query = lineEdit_->text();
    if (query.isEmpty() && list_->count() > 0)
    {
        qDebug().nospace() << "refreshing list";
        for (auto &item : items)
        {
            item->setHidden(false);
        }
        return;
    }

    auto filteredList = list_->findItems(query, Qt::MatchContains);

    for (int i = 0; i < filteredList.size(); ++i)
    {
        qDebug().nospace() << filteredList.at(i)->text();
        for (int j = 0; j < items.size(); ++j)
        {
            if (filteredList.at(i)->text() != items.at(j)->text())
            {
                qDebug().nospace() << "setting " << items.at(j)->text() << " to hidden";
                items.at(j)->setHidden(true);
            }
        }
    }
}

// Private

void Cpman::createSystemTray()
{
    trayIcon_ = new QSystemTrayIcon();
    trayIcon_->setIcon(QIcon("../assets/appicon-16x16.png"));
    trayIcon_->setVisible(true);
    trayIcon_->show();

    // Menu Actions for SysTray
    trayMenu_ = new QMenu();

    showFullAction = new QAction(tr("Show Full Clipboard"));
    showFullAction->setShortcut(Qt::CTRL + Qt::Key_F8);
    connect(showFullAction, &QAction::triggered, this, [=]() { Cpman::show(); } );

    quitAction = new QAction(tr("Quit cpman"));
    quitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(quitAction, &QAction::triggered, this, [=]() { qApp->exit(); } );

    trayMenu_->addAction(showFullAction);
    trayMenu_->addAction(quitAction);

    trayIcon_->setContextMenu(trayMenu_);
}

void Cpman::createMainUI()
{
    // Anything initialized in here should be deleted in Destructor
    lineEdit_ = new QLineEdit(this);
    lineEdit_->setPlaceholderText("Filter");

    list_ = new QListWidget(this);

    label_ = new QLabel(this);
}
