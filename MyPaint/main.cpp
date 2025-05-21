#include <QApplication>
#include "mypaint.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    MyPaint *w = new MyPaint();
    w->setWindowTitle(QStringLiteral("画图"));
    w->setWindowIcon(QIcon(":/resimage/icon.ico"));
    w->show();
    return app.exec();
}
