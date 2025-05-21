#include "mypaint.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QAction>
#include <QPalette>
#include <QDebug>
#include <QPicture>
void MyPaint::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        if(_drawType == 1)//线条(铅笔)
        {
            _lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            _lines.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = _lines.last();//拿到新线条
            lastLine.append(e->pos()-QPoint(0,0));//记录鼠标的坐标(新线条的开始坐标)
            _shape.append(1);
        }
        else if(_drawType == 2)//矩形
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，矩形开始
                _rects.append(rect);//将新矩形添加到矩形集合
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setTopLeft(e->pos()-QPoint(0,0));//记录鼠标的坐标(新矩形的左上角坐标)
                _shape.append(2);
            }
            else if(_rects.last().contains(e->pos()-QPoint(0,0)))//拖拽模式、如果在矩形内按下
            {
                _begin = e->pos()-QPoint(0,0);//记录拖拽开始的坐标位置,方便计算位移

            }

        }
        else if(_drawType == 3)//椭圆
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，椭圆开始
                _ellipse.append(rect);//将新椭圆添加到椭圆集合
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setTopLeft(e->pos()-QPoint(0,0));//记录鼠标的坐标(新椭圆的左上角坐标)
                _shape.append(3);
            }
            else if(_ellipse.last().contains(e->pos()-QPoint(0,0)))//如果在椭圆内按下
            {
                _begin = e->pos()-QPoint(0,0);//记录拖拽开始的坐标位置

            }
        }
        else if(_drawType == 4)//直线
        {
            _lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            _line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setTopLeft(e->pos()-QPoint(0,0));//记录鼠标的坐标(新直线开始一端坐标)
            _shape.append(4);
        }
        else if(_drawType == 5)//文字
        {
            update();//触发窗体重绘
            QPoint p;//鼠标按下，文字开始
            _tpoint.append(p);//将文字坐标添加到文字位置集合
            QPoint& lastp = _tpoint.last();//拿到新文字
            lastp = e->pos()-QPoint(0,0);//记录鼠标的坐标
            textx=lastp.x();
            texty=lastp.y();
            _tEdit->setGeometry(lastp.x()-6,lastp.y()-17,font.pointSize()*10,font.pointSize()*5);//设置文本框的位置、大小

            _tEdit->show();//显示文本输入框
            _text.append("");//添加文本到文本集合
            _tEdit->clear();//因为全局只有一个，所以使用之前要清空
            _shape.append(5);
        }
        else if(_drawType == 0)//橡皮擦
        {
            _lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            _eraser.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = _eraser.last();//拿到新线条
            lastLine.append(e->pos()-QPoint(0,0));//记录鼠标的坐标(新线条的开始坐标)
            _shape.append(0);
        }
        else if(_drawType == 6)//选中区域并进行剪切
        {
            if(count == 1)
            {
                arr[0].setX(e->pos().x());
                arr[0].setY(e->pos().y());
                count = 2;
                qDebug() << arr[0].x();
                qDebug() << arr[0].y();
            }
            else
            {
                arr[1].setX(e->pos().x());
                arr[1].setY(e->pos().y());
                count = 1;

                show_x = arr[0].x();
                show_y = arr[0].y();
                img2 = img.copy(QRect(arr[0].x() - 33, arr[0].y() - 33, arr[1].x() - arr[0].x(), arr[1].y() - arr[0].y()));

                for(int i = arr[0].x() - 33; i<=arr[1].x() - 33; i++)
                {
                    for(int j = arr[0].y() - 33; j<=arr[1].y() - 33; j++)
                    {
                        QRgb value = qRgb(255, 255, 255);
                        img.setPixel(i, j, value);
                    }
                }
                update();
                _drawType = 7;
            }
        }
        else if(_drawType == 8)//选中区域并进行复制（不剪切）
        {
            if(count == 1)
            {
                arr[0].setX(e->pos().x());
                arr[0].setY(e->pos().y());
                count = 2;
                qDebug() << arr[0].x();
                qDebug() << arr[0].y();
            }
            else
            {
                arr[1].setX(e->pos().x());
                arr[1].setY(e->pos().y());
                count = 1;


                show_x = arr[0].x();
                show_y = arr[0].y();
                img2 = img.copy(QRect(arr[0].x() - 33, arr[0].y() - 33, arr[1].x() - arr[0].x(), arr[1].y() - arr[0].y()));

                update();
                _drawType = 7;
            }
        }
    }

    if(e->button() == Qt::RightButton)//鼠标右键按下后的操作
    {
        r_down = true;
        tem_x = e->pos().x();
        tem_y = e->pos().y();
        qDebug() << tem_x;
        qDebug() << tem_y;
    }

}



void MyPaint::mouseMoveEvent(QMouseEvent *e)
{
    if(_drag && ((_drawType == 2 && _rects.last().contains(e->pos()))
                  || (_drawType == 3 && _ellipse.last().contains(e->pos()) )))
    {
        setCursor(Qt::SizeAllCursor);//拖拽模式下，并且在拖拽图形中，将光标形状改为十字
    }
    else
    {
        setCursor(Qt::ArrowCursor);//恢复原始光标形状
        _drag = 0;
    }

    if(_lpress)
    {
        if(_drawType == 1)//铅笔画线
        {
            if(_lines.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            update();//触发窗体重绘
        }
        else if(_drawType == 2)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标
            }
            else//拖拽模式
            {
                QRect& lastRect = _rects.last();//拿到最后添加的矩形
                if(lastRect.contains(e->pos()))//在矩形的内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastRect = lastRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
                    _begin = e->pos();//刷新拖拽点起始坐标
                }

            }
            update();//触发窗体重绘

        }
        else if(_drawType == 3)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标)

            }
            else//拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到最后添加的矩形
                if(lastEllipse.contains(e->pos()))//在椭圆内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);
                    _begin = e->pos();//刷新拖拽点起始坐标
                }

            }
            update();//触发窗体重绘
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线另一端)
            update();//触发窗体重绘
        }
        else if(_drawType == 0)//铅笔画线
        {
            if(_eraser.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = _eraser.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            update();//触发窗体重绘
        }
    }
    //用于拖拽被选中的区域
    if(_drawType == 7)
    {
        if(e->buttons() & Qt::LeftButton)
        {
            setCursor(Qt::SizeAllCursor);
            show_x = e->pos().x();
            show_y = e->pos().y();

            update();
        }
    }


}

void MyPaint::mouseReleaseEvent(QMouseEvent *e)
{
    if(_lpress)
    {
        if(_drawType == 1)
        {
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            _lpress = false;//标志左键释放
        }
        else if(_drawType == 2)
        {
            QRect& lastRect = _rects.last();//拿到新矩形
            if(!_drag)
            {
                lastRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
                //刚画完矩形，将光标设置到新矩形的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
                _drag = 1;

            }
            _lpress = false;

        }
        else if(_drawType == 3)
        {
            QRect& lastEllipse = _ellipse.last();//拿到新椭圆
            if(!_drag)
            {
                lastEllipse.setBottomRight(e->pos());//不是拖拽时，更新椭圆的右下角坐标)
                //刚画完椭圆，将光标设置到新椭圆的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
                _drag = 1;

            }
            _lpress = false;
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新矩形
            lastLine.setBottomRight(e->pos());//更新矩形的右下角坐标)
            _lpress = false;

        }
        else if(_drawType == 0)
        {
            QVector<QPoint>& lastLine = _eraser.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            _lpress = false;//标志左键释放
        }

    }
}
