#include "mypaint.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QAction>
#include <QPalette>
#include <QDebug>
#include <QPicture>
void MyPaint::SavePic()
{
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), "new.jpg", "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(fileName);//不包含工具栏
    }

}
void MyPaint::SavePic2()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.auto", "Image (*.auto)");
    qDebug()<<fileName;
    QString fileName2="pic.png";
    if (fileName2.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(fileName2);//不包含工具栏
        QPixmap pix;
        pix.load(fileName2);
        QPainter r(&_pic);
        r.drawPixmap(0,0,pix);
        r.end();
        _pic.save(fileName);
    }
}
void MyPaint::OpenPic()
{
    //弹出文件打开对话框
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开图片"),"","Image Files(*.jpg *.png)");
    if(!picPath.isEmpty())//用户选择了文件
    {
        QPixmap pix;
        pix.load(picPath);//加载图片
        QPainter p(&_pixmap);
        p.drawPixmap(33,33,pix);//添加工具栏的空间
        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}
void MyPaint::OpenPic2()
{
    //弹出文件打开对话框
    QString picPath2 = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.auto)");
    QPicture pick;
    pick.load(picPath2);

    if (picPath2.length() > 0)
    {
        QPainter p(&_pixmap);

        p.drawPicture(33,33,pick);//添加工具栏的空间

        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}

void MyPaint::keyPressEvent(QKeyEvent *e) //按键事件
{
    //Ctrl+Z撤销
    if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
    {
        if(_shape.size()>0)
        {
            switch(_shape.last())
            {
            case 1: _lines.pop_back();
                break;
            case 2: _rects.pop_back();
                break;
            case 3: _ellipse.pop_back();
                break;
            case 4: _line.pop_back();
                break;
            case 0:_eraser.pop_back();
                break;
            }
            _shape.pop_back();
            _drag = 0;//设置为非拖拽模式
            update();
        }
    }
    else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)//保存
    {
        SavePic();//Ctrl+S保存
    }
}
void MyPaint::Rotate()
{
    if(_openflag!=0)
    {
        QString filename="pic.png";
        QPixmap temppix;
        QImage tempimage,tempimage2;
        _pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(filename);
        temppix.load(filename);
        tempimage=temppix.toImage();
        tempimage2=tempimage;
        for(int i=0;i<tempimage.width();i++)
        {
            for(int j=0;j<tempimage.height();j++)
            {
                if(tempimage.height()-j<tempimage2.width()&&i<tempimage2.height())
                   tempimage2.setPixel(tempimage.height()-j,i,tempimage.pixel(i,j));
            }
        }
        for(int i=tempimage.height();i<tempimage.width();i++)
        {
            for(int j=0;j<tempimage.height();j++)
            {
                tempimage2.setPixel(i,j,qRgb(255,255,255));
            }
        }
        tempimage2.save(filename);
        temppix.load(filename);//加载图片
        QPainter p(&_pixmap);
        p.drawPixmap(33,33,temppix);
    }

    QPoint temp,temp2;
    QVector<QPoint> templine;
    QRect temprect;
    for(int i=0;i<_shape.size();i++)
    {
        if(_shape.at(i)==1)
        {
            for(int j=0;j<_lines.count();j++)
            {
                for(int k=0;k<_lines.at(j).count();k++)
                {
                    temp=_lines.at(j).at(k).transposed();
                    temp.setX(this->height()-temp.x());
                    templine.append(temp);
                }
                _lines.replace(j,templine);
            }
        }
        else if(_shape.at(i)==2)
        {
            for(int j=0;j<_rects.count();j++)
            {
                temp=_rects.at(j).topLeft().transposed();
                temp.setX(this->height()-temp.x());
                temp2=_rects.at(j).bottomRight().transposed();
                temp2.setX(this->height()-temp2.x());
                temprect=QRect(temp,temp2);
                _rects.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==3)
        {
            for(int j=0;j<_ellipse.count();j++)
            {
                temp=_ellipse.at(j).topLeft().transposed();
                temp.setX(this->height()-temp.x());
                temp2=_ellipse.at(j).bottomRight().transposed();
                temp2.setX(this->height()-temp2.x());
                temprect=QRect(temp,temp2);
                _ellipse.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==4)
        {
            for(int j=0;j<_line.count();j++)
            {
                temp=_line.at(j).topLeft().transposed();
                temp.setX(this->height()-temp.x());
                temp2=_line.at(j).bottomRight().transposed();
                temp2.setX(this->height()-temp2.x());
                temprect=QRect(temp,temp2);
                _line.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==5)
        {
            for(int j=0;j<_tpoint.count();j++)
            {
                temp=_tpoint.at(j).transposed();
                temp.setX(this->height()-temp.x());
                _tpoint.replace(j,temp);
            }
        }
        else if(_shape.at(i)==0)
        {
            for(int j=0;j<_eraser.count();j++)
            {
                for(int k=0;k<_eraser.at(j).count();k++)
                {
                    temp=_eraser.at(j).at(k).transposed();
                    temp.setX(this->height()-temp.x());
                    templine.append(temp);
                }
                _eraser.replace(j,templine);
            }
        }
    }
    update();
}
void MyPaint::Vertical_flip()
{
    if(_openflag!=0)
    {
    QString filename="pic.png";
    QPixmap temppix;
    QImage tempimage,tempimage2;
    _pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(filename);
    temppix.load(filename);
    tempimage=temppix.toImage();
    tempimage2=tempimage;
    for(int i=0;i<tempimage.width();i++)
    {
        for(int j=0;j<tempimage.height();j++)
        {
            tempimage2.setPixel(i,tempimage.height()-j-1,tempimage.pixel(i,j));
        }
    }
    tempimage2.save(filename);
    temppix.load(filename);//加载图片
    QPainter p(&_pixmap);
    p.drawPixmap(33,33,temppix);
    }

    QPoint temp,temp2;
    QVector<QPoint> templine;
    QRect temprect;
    for(int i=0;i<_shape.size();i++)
    {
        if(_shape.at(i)==1)
        {
            for(int j=0;j<_lines.count();j++)
            {
                for(int k=0;k<_lines.at(j).count();k++)
                {
                    temp=_lines.at(j).at(k);
                    temp.setY(this->height()-temp.y());
                    templine.append(temp);
                }
                _lines.replace(j,templine);
            }
        }
        else if(_shape.at(i)==2)
        {
            for(int j=0;j<_rects.count();j++)
            {
                temp=_rects.at(j).topLeft();
                temp.setY(this->height()-temp.y());
                temp2=_rects.at(j).bottomRight();
                temp2.setY(this->height()-temp2.y());
                temprect=QRect(temp,temp2);
                _rects.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==3)
        {
            for(int j=0;j<_ellipse.count();j++)
            {
                temp=_ellipse.at(j).topLeft();
                temp.setY(this->height()-temp.y());
                temp2=_ellipse.at(j).bottomRight();
                temp2.setY(this->height()-temp2.y());
                temprect=QRect(temp,temp2);
                _ellipse.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==4)
        {
            for(int j=0;j<_line.count();j++)
            {
                temp=_line.at(j).topLeft();
                temp.setY(this->height()-temp.y());
                temp2=_line.at(j).bottomRight();
                temp2.setY(this->height()-temp2.y());
                temprect=QRect(temp,temp2);
                _line.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==5)
        {
            for(int j=0;j<_tpoint.count();j++)
            {
                temp=_tpoint.at(j);
                temp.setY(this->height()-temp.y());
                _tpoint.replace(j,temp);
            }
        }
        else if(_shape.at(i)==0)
        {
            for(int j=0;j<_eraser.count();j++)
            {
                for(int k=0;k<_eraser.at(j).count();k++)
                {
                    temp=_eraser.at(j).at(k);
                    temp.setY(this->height()-temp.y());
                    templine.append(temp);
                }
                _eraser.replace(j,templine);
            }
        }
    }
    update();
}
void MyPaint::Horizontal_flip()
{
    if(_openflag!=0)
    {
    QString filename="pic.png";
    QPixmap temppix;
    QImage tempimage,tempimage2;
    _pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(filename);
    temppix.load(filename);
    tempimage=temppix.toImage();
    tempimage2=tempimage;
    for(int i=0;i<tempimage.width();i++)
    {
        for(int j=0;j<tempimage.height();j++)
        {
            tempimage2.setPixel(tempimage.width()-i-1,j,tempimage.pixel(i,j));
        }
    }
    tempimage2.save(filename);
    temppix.load(filename);//加载图片
    QPainter p(&_pixmap);
    p.drawPixmap(33,33,temppix);
    }

    QPoint temp,temp2;
    QVector<QPoint> templine;
    QRect temprect;
    for(int i=0;i<_shape.size();i++)
    {
        if(_shape.at(i)==1)
        {
            for(int j=0;j<_lines.count();j++)
            {
                for(int k=0;k<_lines.at(j).count();k++)
                {
                    temp=_lines.at(j).at(k);
                    temp.setX(this->width()-temp.x());
                    templine.append(temp);
                }
                _lines.replace(j,templine);
            }
        }
        else if(_shape.at(i)==2)
        {
            for(int j=0;j<_rects.count();j++)
            {
                temp=_rects.at(j).topLeft();
                temp.setX(this->width()-temp.x());
                temp2=_rects.at(j).bottomRight();
                temp2.setX(this->width()-temp2.x());
                temprect=QRect(temp,temp2);
                _rects.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==3)
        {
            for(int j=0;j<_ellipse.count();j++)
            {
                temp=_ellipse.at(j).topLeft();
                temp.setX(this->width()-temp.x());
                temp2=_ellipse.at(j).bottomRight();
                temp2.setX(this->width()-temp2.x());
                temprect=QRect(temp,temp2);
                _ellipse.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==4)
        {
            for(int j=0;j<_line.count();j++)
            {
                temp=_line.at(j).topLeft();
                temp.setX(this->width()-temp.x());
                temp2=_line.at(j).bottomRight();
                temp2.setX(this->width()-temp2.x());
                temprect=QRect(temp,temp2);
                _line.replace(j,temprect);
            }
        }
        else if(_shape.at(i)==5)
        {
            for(int j=0;j<_tpoint.count();j++)
            {
                temp=_tpoint.at(j);
                temp.setX(this->width()-temp.x());
                _tpoint.replace(j,temp);
            }
        }
        else if(_shape.at(i)==0)
        {
            for(int j=0;j<_eraser.count();j++)
            {
                for(int k=0;k<_eraser.at(j).count();k++)
                {
                    temp=_eraser.at(j).at(k);
                    temp.setX(this->width()-temp.x());
                    templine.append(temp);
                }
                _eraser.replace(j,templine);
            }
        }
    }
    update();
}
void MyPaint::Eraser()
{
    _drawType=0;
}
void MyPaint::Cls()
{
    _pixmap = QPixmap(size());//新建pixmap
    _pixmap.fill(Qt::white);//背景色填充为白色

    //删除所有数据点就可以实现清空画板
    _lines.clear();
    _rects.clear();
    _ellipse.clear();
    _line.clear();
    _text.clear();
    _tpoint.clear();
    _shape.clear();
    _eraser.clear();


    _openflag = 0;
    update();
}

//用于选择并移动图片的函数
void MyPaint::Select()
{
    QString file="pic.png";
    if (file.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(file);//不包含工具栏
        QPixmap pix;
        pix.load(file);
        QPainter r(&_pic);
        r.drawPixmap(0,0,pix);
        r.end();
    }//先保存为一整张图片，再读取到界面上

    Cls();

    QString position = "pic.png";
    img.load(position);
    _openflag = 1;

    update();
    _drawType = 6;

}

void MyPaint::Copy()
{
    QString file="pic.png";
    if (file.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(33,33,size().width(),size().height()-30)).save(file);//不包含工具栏
        QPixmap pix;
        pix.load(file);
        QPainter r(&_pic);
        r.drawPixmap(0,0,pix);
        r.end();
    }//先保存为一整张图片，再读取到界面上

    Cls();
    QString position = "pic.png";
    img.load(position);
    _openflag = 1;

    update();
    _drawType = 8;
}

void MyPaint::Paste()
{
    is_paste = true;
    qDebug() << "粘贴";

    if(r_down && is_paste)
    {
        _drawType = 9;//这行代码可以使粘贴之后的图片不可以移动
        show_x = tem_x;
        show_y = tem_y;
        update();
        r_down = false;
        is_paste = false;
    }
}
