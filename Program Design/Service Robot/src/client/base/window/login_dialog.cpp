#include "login_dialog.h"
#include "ui_login_dialog.h"

#include <QDebug>

//用户id大小限制
extern const int kMaxIdLength;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    QString id = ui->idText->toPlainText();
    ui->idText->setText("");
    qInfo() << "try login, id:" << id;
    //用户输入id过长
    if(id.size() > kMaxIdLength)
    {
        qWarning() << id  << "id is too long";
        loginFail(tr("ID不应超过") + QString::number(kMaxIdLength) + tr("个字符"));
        return;
    }
    //用户输入为空
    if(id.size() == 0)
    {
        qWarning() << id  << "id is empty";
        loginFail(tr("ID为空"));
        return;
    }
    emit login(id);
}

