# TimeMessage Class : ChatMessage

Inherits: [ChatMessage](ChatMessage.md)

时间文本显示窗口部件

- [Public Functions](#public-functions)
  - [TimeMessage(QWidget \*parent = nullptr)](#timemessageqwidget-parent--nullptr)
- [Inherited Public Functions](#inherited-public-functions)
- [Reimplemented Protected Functions](#reimplemented-protected-functions)
  - [virtual void paintEvent(QPaintEvent \*event)](#virtual-void-painteventqpaintevent-event)
  
---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||TimeMessage(QWidget *parent = nullptr)||

---
### TimeMessage(QWidget *parent = nullptr)

构建一个时间文本显示窗口部件

---
## Inherited Public Functions

|Return|Statement|Description|
|---|---|---|
|void|setText(const QString& text, const QDateTime& time)|设置文本内容和窗口发布时间|
|const QString&|getText()|获取文本内容|
|const QDateTime&|getTime()|获取窗口部件发布时间|
|const QSize&|getSize()|获取窗口部件大小|
|const QSize|resizeMessage()|重新调整窗口大小|

---
## Reimplemented Protected Functions

|Return|Statement|Description|
|---|---|---|
|virtual void|paintEvent(QPaintEvent *event)|绘制窗口部件|

---
### virtual void paintEvent(QPaintEvent *event)

Reimplements: QWidget::paintEvent(QPaintEvent *event)

接受绘制窗口部件的请求，绘制窗口部件