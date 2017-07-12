/*原文来自网络，添加了 viewPort rotate texture
 * OPENGl中没有摄像机类，是viewMatrix抽象出来的了。
 * 就是在观察矩阵中的变换。

*/
#include <QApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;
    w.show();

    return a.exec();
}
