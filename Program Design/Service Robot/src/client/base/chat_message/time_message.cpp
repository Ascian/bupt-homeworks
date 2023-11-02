#include "time_message.h"

#include <QPainter>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QTextOption>

void TimeMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));
    painter.setPen(QPen(QColor(153,153,153)));

    //文本内容绘制
    QTextOption option(Qt::AlignCenter);  //居中放置时间
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    QFont font = this->font();
    font.setFamily("MicrosoftYaHei");
    font.setPointSize(10);
    painter.setFont(font);
    painter.drawText(this->rect(), message, option);
}
