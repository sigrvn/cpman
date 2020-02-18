#include "cpman.h"

// TODO: add global hotkey to open/close cpman window
Cpman::Cpman(QWidget *parent) : QWidget(parent, Qt::FramelessWindowHint)
{
    createSystemTray();
    createMainUI();

    clipboard_ = QGuiApplication::clipboard();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(lineEdit_, 0, 0);
    mainLayout->addWidget(list_, 1, 0);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("cpman"));

    // Borderless base widget with no margins
    this->setContentsMargins(0,0,0,0);
    this->layout()->setContentsMargins(0,1,0,0);

    connect(trayIcon_, &QSystemTrayIcon::activated, this, [=]() { trayMenu_->show(); });

    connect(clipboard_, &QClipboard::dataChanged, this, &Cpman::addToClipboard);

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
    if (str.isEmpty()) return;

    qDebug().nospace() << "copied " << qPrintable(str);

    // Don't add repeats
    for (int i = 0; i < items.size(); ++i)
    {
        if (items.at(i)->text() == str) return;
    }

    QListWidgetItem *newItem = new QListWidgetItem(str);
    newItem->setSizeHint(QSize(newItem->sizeHint().width(), 20));

    items.push_back(newItem);
    list_->addItem(newItem);
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
    trayIcon_->setIcon(QIcon("./assets/appicon-16x16.png"));
    trayIcon_->setVisible(true);
    trayIcon_->show();

    trayMenu_ = new QMenu();

    showAction = new QAction(tr("Toggle Clipboard"), this);
    connect(showAction, &QAction::triggered, this, [=]() { toggleVisibility(); });
    this->addAction(showAction);
    trayMenu_->addAction(showAction);

    filterAction = new QAction(tr("Filter/Find"), this);
    filterAction->setShortcut(Qt::Key_F | Qt::CTRL);
    connect(filterAction, &QAction::triggered, this, [=]() {
        if (!lineEdit_->isVisible()) lineEdit_->setVisible(true);
        else lineEdit_->setVisible(false);
    });
    this->addAction(filterAction);
    trayMenu_->addAction(filterAction);

    quitAction = new QAction(tr("Quit cpman"), this);
    quitAction->setShortcut(Qt::Key_Q | Qt::CTRL);
    connect(quitAction, &QAction::triggered, this, [=]() { qApp->quit(); });
    this->addAction(quitAction);
    trayMenu_->addAction(quitAction);

    trayIcon_->setContextMenu(trayMenu_);
}

void Cpman::createMainUI()
{
    // Anything initialized in here should be deleted in Destructor
    lineEdit_ = new QLineEdit(this);
    lineEdit_->setPlaceholderText("Filter");
    lineEdit_->setVisible(false);

    connect(lineEdit_, &QLineEdit::textChanged, this, &Cpman::filterItems);

    list_ = new QListWidget(this);

    // Create SignalMapper here
    connect(list_, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) { 
        clipboard_->setText(item->text());
    });

    connect(list_, &QListWidget::itemActivated, this, [=](QListWidgetItem *item) { 
        clipboard_->setText(item->text());
    });

    QFile f("./src/style.qss");
    if (f.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(f.readAll());
        this->setStyleSheet(qss);
    }
}

void Cpman::toggleVisibility()
{
    if (!this->isVisible())
    {
        QPoint globalCursorPos = QCursor::pos();
        this->move(globalCursorPos);
        this->show();
    }

    else 
    {
        this->hide();
    }
}

void Cpman::registerHotkeys(QApplication &a)
{
    auto quitHotkey = new QHotkey(QKeySequence("ctrl+alt+Q"), true, &a);
    connect(quitHotkey, &QHotkey::activated, qApp, [&](){ qApp->quit(); });

    auto toggleHotkey = new QHotkey(QKeySequence("ctrl+alt+A"), true, &a);
    connect(toggleHotkey, &QHotkey::activated, qApp, [&]() { toggleVisibility(); } );
}  