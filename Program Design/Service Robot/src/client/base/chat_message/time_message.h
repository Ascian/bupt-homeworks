#ifndef TIME_MESSAGE_H_
#define TIME_MESSAGE_H_

#include <QWidget>
#include <QPaintEvent>

#include "chat_message.h"

//时间文本显示窗口部件
class TimeMessage : public ChatMessage
{
public:
    TimeMessage(QWidget *parent = nullptr) : ChatMessage(parent) {}

protected:
    //Reimplements: QWidget::paintEvent(QPaintEvent *event)
    //接受绘制窗口部件的请求，绘制窗口部件
    virtual void paintEvent(QPaintEvent *event);
};

#endif // TIME_MESSAGE_H_
