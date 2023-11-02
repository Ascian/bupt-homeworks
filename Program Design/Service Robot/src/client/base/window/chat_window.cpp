#include "chat_window.h"

#include <string>

#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

#include "../chat_message/user_message.h"
#include "../chat_message/time_message.h"

//用户输入大小限制
extern const int kMaxInputLength;

ChatWindow::ChatWindow(QWidget *parent)
        :QMainWindow(parent),
          ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    loginDialog = new LoginDialog(this);
    chatProcessor = new ChatProcessor();

    QObject::connect(this, SIGNAL(beginChat()), chatProcessor, SLOT(chat()));
    QObject::connect(loginDialog, SIGNAL(login(QString)), chatProcessor, SLOT(login(QString)));
    QObject::connect(chatProcessor, SIGNAL(loginSucceed()), loginDialog, SLOT(loginSucceed()));
    QObject::connect(chatProcessor, SIGNAL(loginFail(QString)), loginDialog, SLOT(loginFail(QString)));
    QObject::connect(chatProcessor, SIGNAL(forbidInput()), this, SLOT(forbidInput()));
    QObject::connect(chatProcessor, SIGNAL(allowInput()), this, SLOT(allowInput()));
    QObject::connect(chatProcessor, SIGNAL(end()), this, SLOT(end()));
    QObject::connect(chatProcessor, SIGNAL(serverOutput(QString)), this, SLOT(serverOutput(QString)));
    QObject::connect(chatProcessor, SIGNAL(error(QString)), this, SLOT(error(QString)));

    //新建线程处理与服务器通信
    chatProcessor->moveToThread(&chatThread);
    chatThread.start();
}

ChatWindow::~ChatWindow()
{
    delete ui;
    chatProcessor->close();
    chatThread.quit();
    chatThread.wait();
}

//获取并显示用户输入文本，将用户输入交给chatProcessor处理
void ChatWindow::on_enterButton_clicked()
{

    QString text = ui->inputText->toPlainText();
    ui->inputText->setText("");
    qInfo() << "add user input:" << text;
    //用户输入过长
    if(text.size() > kMaxInputLength)
    {
        qWarning() << text  << "input is too long";
        QMessageBox::warning(this, tr("输入错误"), tr("输入不应超过")+
                             QString::number(kMaxInputLength) + tr("个字符"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    UserMessage* message = new UserMessage(ui->messageList->parentWidget());
    addText(message, text);

    chatProcessor->addInputBuf(text);
}

void ChatWindow::addText(ChatMessage* message, const QString& text)
{
    addTime();
    //将文本内容放入文本显示窗口部件，并显示在messageList上
    message->setFixedWidth(ui->messageList->width());
    message->setText(text, QDateTime::currentDateTime());
    QListWidgetItem* item = new QListWidgetItem(ui->messageList);
    item->setSizeHint(message->getSize());
    ui->messageList->setItemWidget(item, message);
    ui->messageList->setCurrentRow(ui->messageList->count()-1);
}

void ChatWindow::addTime()
{
    bool isShowTime = true;
    QDateTime curTime = QDateTime::currentDateTime();
    if(ui->messageList->count() > 0) {
        QListWidgetItem* lastItem = ui->messageList->item(ui->messageList->count() - 1);
        ChatMessage* message = (ChatMessage*)ui->messageList->itemWidget(lastItem);
        QDateTime lastTime = message->getTime();
        //若距离上一次显示文本内容超过一分钟，添加显示时间
        isShowTime = ((curTime.toSecsSinceEpoch() - lastTime.toSecsSinceEpoch()) > 60); // 两个消息相差大于一分钟
    }
    if(isShowTime) {
        //显示时间信息到messageList上
        TimeMessage* messageTime = new TimeMessage(ui->messageList->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->messageList);
        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curTime.toString("hh:mm"), curTime);
        ui->messageList->setItemWidget(itemTime, messageTime);
    }
}

void ChatWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    //重新调整messageList内每一个窗口部件的大小
    for(int i = 0; i < ui->messageList->count(); i++) {
        QListWidgetItem* item = ui->messageList->item(i);
        ChatMessage* message = (ChatMessage*)ui->messageList->itemWidget(item);
        message->setFixedWidth(ui->messageList->width());
        QSize newSize = message->resizeMessage();
        item->setSizeHint(newSize);
        ui->messageList->setItemWidget(item, message);
    }
}
