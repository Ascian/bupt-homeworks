#ifndef CHAT_WINDOW_H_
#define CHAT_WINDOW_H_

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QString>

#include "ui_chat_window.h"
#include "login_dialog.h"
#include "../chat_message/chat_message.h"
#include "../chat_message/server_message.h"
#include "../chat_processor/chat_processor.h"

namespace Ui {
class ChatWindow;
}

//聊天窗口，用于显示服务器和用户文本，获取用户输入
//example:
//    首先需要用户登录，若登录成功显示聊天窗口并开始与服务器交流，否则退出程序
//    if(chatWindow.login())
//    {
//        chatWindow.show();
//        chatWindow.start();
//        return a.exec();
//    }
//    return 0;
class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    //执行用户登录
    //return:
    //    返回登录是否成功
    inline bool login()
    {
        if(loginDialog->exec() == QDialog::Accepted)
            return true;
        this->close();
        return false;
    }

    //开始与服务器“聊天”，显示服务器和用户文本，获取用户输入
    inline void start() { emit beginChat(); }

signals:
    //表示“聊天”开始的信号
    void beginChat();

protected:
    //Reimplements: QWidget::resizeEvent(QResizeEvent *event)
    //接受改变窗口部件大小的请求，重新计算各窗口部件大小
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::ChatWindow *ui;

    //登录会话窗口
    LoginDialog* loginDialog;
    //处理与服务器通信的线程
    QThread chatThread;
    //处理与服务器通信的对象
    ChatProcessor* chatProcessor;

    //添加显示文本
    //parameter:
    //    ChatMessage* message - 文本显示窗口部件
    //    const QString& text - 文本内容
    void addText(ChatMessage* message, const QString& text);

    //若距离上一次显示文本内容超过一分钟，添加显示时间
    void addTime();

private slots: 
    //输入按键
    void on_enterButton_clicked();

    //禁止用户输入
    inline void forbidInput() { ui->inputWidget->setEnabled(false); }

    //允许用户输入
    inline void allowInput() { ui->inputWidget->setEnabled(true); }

    //显示服务器文本内容
    //parameter:
    //    const QString& text - 服务器文本内容
    inline void serverOutput(const QString& text)
    {
        ServerMessage* message = new ServerMessage(ui->messageList->parentWidget());
        addText(message, text);
    }

    //程序执行出错
    //parameter:
    //    const QString& err - 错误描述
    inline void error(const QString& err)
    {
        QMessageBox::warning(this, tr("错误"), err, QMessageBox::Ok, QMessageBox::Ok);
        this->close();
    }

    //结束与服务器通信
    inline void end()
    {
        QMessageBox::information(this, tr(""), tr("对话结束"), QMessageBox::Ok,
                                 QMessageBox::Ok);
        this->close();
    }

};

#endif // CHAT_WINDOW_H_
