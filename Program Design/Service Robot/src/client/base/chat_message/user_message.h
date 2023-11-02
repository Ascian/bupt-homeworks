#ifndef USER_MESSAGE_H_
#define USER_MESSAGE_H_

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

#include "chat_message.h"

//用户文本显示窗口部件
class UserMessage : public ChatMessage
{
public:
    UserMessage(QWidget *parent = nullptr) : ChatMessage(parent)
    {
        headPortrait = QPixmap(":/img/Customer.png");
    }

protected:
    //Reimplements: QWidget::paintEvent(QPaintEvent *event)
    //接受绘制窗口部件的请求，绘制窗口部件
    virtual void paintEvent(QPaintEvent *event);

private:
    //用户头像
    QPixmap headPortrait;
};

#endif // USER_MESSAGE_H_
