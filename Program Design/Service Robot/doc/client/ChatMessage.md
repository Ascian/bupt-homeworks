# ChatMessage Class : QWidget

Inherits: [QWidget](https://doc.qt.io/qt-6/qwidget.html)

文本显示窗口部件

example:

    //首先需要设置窗口部件宽度
    message->setFixedWidth(width);
    //然后设置文本内容和窗口发布时间
    message->setText(text, QDateTime::currentDateTime());

    //若需要重新调整窗口部件宽度，只需要先设置窗口部件宽度
    message->setFixedWidth(width);
    //然后重新调整窗口大小
    message->resizeMessage();

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [void setText(const QString\& text, const QDateTime\& time)](#void-settextconst-qstring-text-const-qdatetime-time)
  - [const QString\& getText()](#const-qstring-gettext)
  - [const QDateTime\& getTime()](#const-qdatetime-gettime)
  - [const QSize\& getSize()](#const-qsize-getsize)
  - [const QSize resizeMessage()](#const-qsize-resizemessage)

---

## Properties

|Type|Name|Description|
|---|---|---|
|const int|kMinHeight|文本内容最小高度|
|const int|kIconSideLen|头像边长|
|const int|kIconBoundarySpaceWidth|头像与左右边界的空隙宽度|
|const int|kIconBoundarySpaceHeight|头像与上边界的空隙宽度|
|const int|kIconTextSpaceWidth|头像文本框之间空隙宽度|
|const int|kTextBoxSpaceWidth|文本与文本框之间空隙宽度|
|QString|message|文本内容|
|QDateTime|time|窗口部件发布时间|
|QSize|size|窗口部件大小|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ChatMessage(QWidget *parent = nullptr)||
|void|setText(const QString& text, const QDateTime& time)|设置文本内容和窗口发布时间|
|const QString&|getText()|获取文本内容|
|const QDateTime&|getTime()|获取窗口部件发布时间|
|const QSize&|getSize()|获取窗口部件大小|
|const QSize|resizeMessage()|重新调整窗口大小|

---
### void setText(const QString& text, const QDateTime& time)

设置文本内容和窗口发布时间，同时依据文本大小调整窗口部件大小

#### parameter

* const QString& text - 文本内容
* const QDateTime& time - 窗口发布时间

---
### const QString& getText()

获取文本内容

---
### const QDateTime& getTime()

获取窗口部件发布时间

---
### const QSize& getSize()

获取窗口部件大小

---
### const QSize resizeMessage()

重新调整窗口大小。依据文本大小和调整后的窗口大小限制，重新计算窗口大小。

#### return

返回重新调整后的窗口大小