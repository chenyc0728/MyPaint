#ifndef MYPAINT_H
#define MYPAINT_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QToolBar>
#include <QAction>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include <QPicture>

class MyPaint : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyPaint(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);//重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);//重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);//重写鼠标移动事件
    void contextMenuEvent(QContextMenuEvent *);//重写菜单事件
    void keyPressEvent(QKeyEvent *e); //重写按键事件
public:
private:
    QColor color;
    int textx;//文本横坐标
    int texty;//文本纵坐标
    bool fontchange=false;//改变字体
    QFont font;//字体
    QPen pen;//画笔
    int penWidth;//画笔宽度
    int _lpress;//左键按下标志
    int _drag;//拖拽标志
    int _drawType;//描绘类型
    QMenu *_Rmenu;//右键菜单
    int _openflag;//打开图片
    QPixmap _pixmap;//画布图片
    QPicture _pic;//记录操作
    QTextEdit *_tEdit;//文本输入框

    QImage img;//用于选择的图片
    QImage img2;//用于移动的图片
    QPoint arr[2];//存储被选择区域的坐标数组
    int count = 1;//用于选择两个不同的点
    int show_x = 0;//最后img2的位置
    int show_y = 0;//最后img2的位置
    bool is_paste = false;//是否按下粘贴按钮
    bool r_down = false;//是否按下鼠标右键
    int tem_x = 0;//临时存放鼠标右键的坐标
    int tem_y = 0;//临时存放鼠标右键的坐标
public:
    QVector<QVector<QPoint> > _lines;//线条集合(一条线条可包含多个线段)
    QVector<QVector<QPoint> > _eraser;
    QVector<QRect> _rects;//矩形集合
    QVector<QRect> _ellipse;//椭圆集合
    QVector<QRect>  _line;//直线集合
    QVector<QString>  _text;//文字集合
    QVector<QPoint>  _tpoint;//文字位置集合
    QVector<int>  _shape;//图形类型集合，用于撤回功能
    QPoint _begin;//鼠标按下坐标、用于最后一个图形移动
signals:

public slots:
    void Lines();//铅笔画线
    void SavePic();//保存图片
    void SavePic2();//保存为本程序可读
    void Rects();//画矩形
    void Ellipses();//画椭圆
    void Line();//画直线
    void OpenPic();//打开图片
    void OpenPic2();//打开图片
    void Texts();//文字
    void AddTexts();//添加文字
    void Cls();//清除
    void Rotate();//旋转
    void Vertical_flip();//垂直翻转
    void Horizontal_flip();//水平翻转
    void Eraser();//橡皮
    void Select();//选择
    void Copy();//复制
    void Paste();//粘贴
};

#endif // MYPAINT_H
