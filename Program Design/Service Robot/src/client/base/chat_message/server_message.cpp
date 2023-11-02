#include "server_message.h"

#include <QPainter>
#include <QRect>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QTextOption>

void ServerMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  //消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    //头像绘制，靠右侧对齐放置
    painter.drawPixmap(QRect(this->width() - kIconBoundarySpaceWidth - kIconSideLen,
                             kIconBoundarySpaceHeight, kIconSideLen, kIconSideLen),
                       headPortrait);

    //文本框绘制
    QRect boxRect;  //文本框放置区域
    //头像总宽度
    int iconWidth = kIconSideLen + kIconBoundarySpaceWidth + kIconTextSpaceWidth;
    //单行高度
    int lineHeight = QFontMetrics(this->font()).lineSpacing();
    //文本框靠右对齐放置
    if(size.width() < this->width())
        boxRect.setRect(this->width() - size.width() + iconWidth, lineHeight / 4 * 3,
                        size.width() - 2 * iconWidth, size.height() - lineHeight);
    else
        boxRect.setRect(iconWidth, lineHeight / 4 * 3, this->width() - 2 * iconWidth,
                        size.height() - lineHeight);
    painter.setBrush(QBrush(QColor(75, 164, 242)));
    painter.drawRoundedRect(boxRect, 4, 4);

    //文本内容绘制
    painter.setPen(QPen(Qt::white));
    QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    painter.setFont(this->font());
    QRect textRect(boxRect.x() + kTextBoxSpaceWidth, boxRect.y()+kIconBoundarySpaceHeight,
                   boxRect.width() - 2 * kTextBoxSpaceWidth,
                   boxRect.height() - 2 * kIconBoundarySpaceHeight);
    painter.drawText(textRect, message, option);
}
