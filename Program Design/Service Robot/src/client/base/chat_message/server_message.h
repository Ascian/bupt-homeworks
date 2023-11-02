#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

#include "chat_message.h"

//服务器文本显示窗口部件
class ServerMessage : public ChatMessage
{
public:
    ServerMessage(QWidget *parent = nullptr) : ChatMessage(parent)
    {
        headPortrait = QPixmap(":/img/Server.png");
    }

protected:
    //Reimplements: QWidget::paintEvent(QPaintEvent *event)
    //接受绘制窗口部件的请求，绘制窗口部件
    virtual void paintEvent(QPaintEvent *event);

private:
    //客服头像
    QPixmap headPortrait;
};

#endif // SERVER_MESSAGE_H_
