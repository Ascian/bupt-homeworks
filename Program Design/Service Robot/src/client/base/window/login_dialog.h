#ifndef LOGIN_DIALOG_H_
#define LOGIN_DIALOG_H_

#include <QDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

//登录会话窗口，获取用户id
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent);
    ~LoginDialog();

signals:
    //表示用户登录请求的信号，传递用户输入id
    //parameter:
    //   const QString& id - 用户输入id
    void login(const QString& id);

private:
    Ui::LoginDialog *ui;

private slots:
    //登录按键
    void on_loginButton_clicked();
    //退出按键
    inline void on_exitButton_clicked() { rejected(); }

    //登录成功
    inline void loginSucceed() { accept(); }

    //登录失败
    //parameter:
    //    const QString& err - 失败原因
    inline void loginFail(const QString& err)
    {
        QMessageBox::information(this, tr("登录失败"), err,
                                 QMessageBox::Ok, QMessageBox::Ok);
    }
};

#endif // LOGIN_DIALOG_H_
