#include "mypaint.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QAction>
#include <QPalette>
#include <QDebug>
#include <QPicture>
void MyPaint::paintEvent(QPaintEvent *)
{
    if(_openflag == 0)//不是打开图片的，每一次新建一个空白的画布
    {
        _pixmap = QPixmap(size());//新建pixmap
        _pixmap.fill(Qt::white);//背景色填充为白色
    }
    QPixmap pix = _pixmap;//以_pixmap作为画布
    QPainter p(&pix);
    //    QPicture pic = _pic;



    p.drawImage(33,33, img);
    p.drawImage(show_x, show_y, img2);
    p.drawEllipse(show_x - 2, show_y - 2, 2, 2);
    p.drawEllipse(show_x - 2 + arr[1].x() - arr[0].x(), show_y - 2 + arr[1].y() - arr[0].y(), 2, 2);

    QPen penn;
    penn.setColor(QColor(255,255,255));
    penn.setWidth(6);
    p.setPen(pen);
    pen.setColor(color);
    if(fontchange)
    {
        _tEdit->setGeometry(textx-6,texty-17,font.pointSize()*10,font.pointSize()*5);//设置文本框的位置、大小

        _tEdit->setFont(font);
        fontchange=false;
    }
//    r.setPen(qRgb(255,255,255));
    //    QPainter r(&pic);
    unsigned int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0;//各种图形的索引


    for(int c = 0;c<_shape.size();++c)//控制用户当前所绘图形总数
    {
        if(_shape.at(c) == 1)//线条
        {
            const QVector<QPoint>& line = _lines.at(i1++);//取出一条线条
            for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
            {
                p.drawLine(line.at(j), line.at(j+1));
                //                  r.drawLine(line.at(j), line.at(j+1));
            }
        }
        else if(_shape.at(c) == 2)//矩形
        {
            p.drawRect(_rects.at(i2++));
            //             r.drawRect(_rects.at(i2++));
        }
        else if(_shape.at(c) == 3)//椭圆
        {
            p.drawEllipse(_ellipse.at(i3++));
            //            r.drawEllipse(_ellipse.at(i3++));
        }
        else if(_shape.at(c) == 4)//直线
        {
            p.drawLine(_line.at(i4).topLeft(),_line.at(i4).bottomRight());
            //            r.drawLine(_line.at(i4).topLeft(),_line.at(i4).bottomRight());

            i4++;
        }
        else if(_shape.at(c) == 5)//文本
        {
            p.drawText(_tpoint.at(i5),_text.at(i5));
            //            r.drawText(_tpoint.at(i5),_text.at(i5));
            i5++;
        }
        else if(_shape.at(c)==0)//橡皮擦
        {
            p.end();
            QPainter r(&pix);
            r.setPen(penn);
            const QVector<QPoint>& line = _eraser.at(i6++);//取出一条线条
            for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
            {
                r.drawLine(line.at(j), line.at(j+1));
            }
            r.end();
            p.begin(&pix);
        }


    }
    p.end();

    p.begin(this);//将当前窗体作为画布
    p.drawPixmap(0,0, pix);//将pixmap画到窗体
}
void MyPaint::AddTexts()//当输入框文本改变时调用
{
    QString& last = _text.last();//拿到最后一个文本
    last = _tEdit->toPlainText();//将输入框文本作为文本
}
void MyPaint::Lines()
{
    _drawType = 1;//铅笔
    _tEdit->hide();//文本框隐藏
}

void MyPaint::Line()
{
    _drawType = 4;//直线
    _tEdit->hide();
}

void MyPaint::Rects()
{
    _drawType = 2;//矩形
    _tEdit->hide();

}

void MyPaint::Ellipses()
{
    _drawType = 3;//椭圆
    _tEdit->hide();
}

void MyPaint::Texts()
{
    _drawType = 5;//文字
}
