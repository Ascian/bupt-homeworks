# LoginDialog Class : QDialog

Inherits: [QDialog](https://doc.qt.io/qt-6/qdialog.html)

登录会话窗口，获取用户id

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [LoginDialog(QWidget \*parent)](#logindialogqwidget-parent)
  - [~LoginDialog()](#logindialog)
- [Signals:](#signals)
  - [void login(const QString\& id)](#void-loginconst-qstring-id)
- [Private Slots](#private-slots)
  - [void on\_loginButton\_clicked()](#void-on_loginbutton_clicked)
  - [voidon\_exitButton\_clicked()](#voidon_exitbutton_clicked)
  - [void loginSucceed()](#void-loginsucceed)
  - [void loginFail(const QString\& err)](#void-loginfailconst-qstring-err)


---
## Properties

|Type|Name|Description|
|---|---|---|
|Ui::LoginDialog*|ui||

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||LoginDialog(QWidget *parent)||
||~LoginDialog()||

---
### LoginDialog(QWidget *parent)

构建一个登录会话窗口

---
### ~LoginDialog()

析构该登录会话窗口

---
## Signals:

|Return|Statement|Description|
|---|---|---|
|void|login(const QString& id)|表示用户登录请求的信号|

---
### void login(const QString& id)

表示用户登录请求的信号，并传递用户输入id

#### parameter

* const QString& id - 用户输入id

---
## Private Slots

|Return|Statement|Description|
|---|---|---|
|void|on_loginButton_clicked()|登录按键|
|void|on_exitButton_clicked()|退出按键|
|void|loginSucceed()|登录成功|
|void|loginFail(const QString& err)|登录失败|

---
### void on_loginButton_clicked()

登录按键

---
### voidon_exitButton_clicked()

退出按键

---
### void loginSucceed()

登录成功，登录窗口退出并返回`QDialog::Accepted`

---
### void loginFail(const QString& err)

登录失败，显示失败提示，等待用户重新输入

#### parameter

* const QString& err - 失败描述
