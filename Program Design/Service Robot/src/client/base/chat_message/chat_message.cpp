#include "chat_message.h"

ChatMessage::ChatMessage(QWidget *parent) : QWidget(parent)
{
    //设置文本字体
    QFont font = this->font();
    font.setFamily("MicrosoftYaHei");
    font.setPointSize(12);
    this->setFont(font); 
}

void ChatMessage::setText(const QString &text, const QDateTime &time)
{
    message = text;
    this->time = time;
    size = resizeMessage();  //计算窗口部件大小
}

const QSize ChatMessage::resizeMessage()
{
    QString text = message;
    //文本宽度
    int textWidth = this->width()
            - 2 * (kIconSideLen + kIconBoundarySpaceWidth + kIconTextSpaceWidth)
            - 2 * kTextBoxSpaceWidth;
    
    QFontMetrics fontMetrics(this->font());
    int lineHeight = fontMetrics.lineSpacing();  //单行文本高度
    QStringList lineList = text.split('\n');
    int lineNum = lineList.size();
    int lineMaxWidth = 0;  //最大行宽度

    for(int i = 0; i < lineList.size(); i++)
    {
        int tempWidth = fontMetrics.width(lineList[i]);
        if( tempWidth > lineMaxWidth)
            lineMaxWidth = tempWidth;
        //若行宽度大于文本宽度，将单行拆成多行
        if(lineMaxWidth > textWidth)
        {
            lineMaxWidth = textWidth;
            lineNum += fontMetrics.width(text) / textWidth;
        }
    }

    int height = lineNum * lineHeight + 2 * lineHeight ;  //窗口部件高度预留两行作为空隙
    if(height < kMinHeight)
        height = kMinHeight;
    int width = lineMaxWidth + this->width() - textWidth;  //窗口部件宽度为最长行宽度加上其余空间
    size = QSize(width, height);
    return size;
}

