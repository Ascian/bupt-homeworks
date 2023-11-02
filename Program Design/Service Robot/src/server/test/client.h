#include <iostream>
#include <fstream>
#include <functional>

using namespace std;

//客户端模拟测试桩
class Client
{
public:

    Client(const string& inputPath)
    {
        inputFile.open(inputPath, ios::in);
        inputFunc = bind(&Client::input, this, placeholders::_1,
            placeholders::_2, placeholders::_3);
        outputFunc = bind(&Client::output, this, placeholders::_1);
    }

    const function<void(string*, const int&, const int&)>& getInputFunc() const
    {
        return inputFunc;
    }

    const function<void(const string&)>& getOuputFunc() const
    {
        return outputFunc;
    }

private:
    ifstream inputFile;
    function<void(string*, const int&, const int&)> inputFunc;
    function<void(const string&)> outputFunc;

    inline void input(string* str, const int& beginTimer, const int& endTimer)
    {
        getline(inputFile, *str);
    }

    inline void output(const string& str)
    {
        cout << str << endl;
    }
};