#include <string>

#include <QApplication>

#include "base/window/chat_window.h"
#include "base/window/login_dialog.h"

//服务器ip地址
extern const std::string kServerIp = "127.0.0.1";
//服务器端口号
extern const int kServerPort = 5500;
//用户id大小限制
extern const int kMaxIdLength = 20;
//用户输入大小限制
extern const int kMaxInputLength = 20;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatWindow chatWindow;
    if(chatWindow.login())
    {
        //登录成功
        chatWindow.show();
        chatWindow.start();
        return a.exec();
    }
    return 0;
}

