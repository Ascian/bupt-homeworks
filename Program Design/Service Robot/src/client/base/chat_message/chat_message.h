#ifndef CHAT_MESSAGE_H_
#define CHAT_MESSAGE_H_

#include <QWidget>
#include <QDateTime>

//文本显示窗口部件
//example:
//    首先需要设置窗口部件宽度
//    message->setFixedWidth(width);
//    然后设置文本内容和窗口发布时间
//    message->setText(text, QDateTime::currentDateTime());
//
//    若需要重新调整窗口部件宽度，只需要先设置窗口部件宽度
//    message->setFixedWidth(width);
//    然后重新调整窗口大小
//    message->resizeMessage();
class ChatMessage : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessage(QWidget *parent = nullptr);

    //设置文本内容和窗口发布时间
    //parameter:
    //    const QString& text - 文本内容
    //    const QDateTime& time - 窗口发布时间
    void setText(const QString& text, const QDateTime& time);

    inline const QString& getText() { return message; }
    
    inline const QDateTime& getTime() { return time; }

    inline const QSize& getSize() { return size; }

    //重新调整窗口部件大小
    //return:
    //    返回重新调整后的窗口大小
    const QSize resizeMessage();
    
protected:
    //文本内容最小高度
    const int kMinHeight = 30;
    //头像边长
    const int kIconSideLen = 40;
    //头像与左右边界的空隙宽度
    const int kIconBoundarySpaceWidth = 20;
    //头像与上边界的空隙宽度
    const int kIconBoundarySpaceHeight = 10;
    //头像文本框之间空隙宽度
    const int kIconTextSpaceWidth = 5;
    //文本与文本框之间空隙宽度
    const int kTextBoxSpaceWidth = 12;

    //文本内容
    QString message;
    //窗口部件发布时间
    QDateTime time;
    //窗口部件大小
    QSize size;
};

#endif // CHAT_MESSAGE_H_
