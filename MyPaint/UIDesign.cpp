#include "mypaint.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QAction>
#include <QPalette>
#include <QDebug>
#include <QPicture>
#include<QInputDialog>
MyPaint::MyPaint(QWidget *parent) :
    QMainWindow(parent)
{
     _lpress = false;//初始鼠标左键未按下
     _drawType = 0;//初始为什么都不画
     _drag = 0;//默认非拖拽模式
     _begin = pos();//拖拽的参考坐标，方便计算位移
     _openflag = 0;//初始不打开图片
     _tEdit = new QTextEdit(this);//初始化文本输入框
     _tEdit->hide();//隐藏
//     this->setGeometry(350,200,600,400);//设置窗体大小、位置
     resize(1250,800);
     setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听
     //设置背景黑色
     //方法一
//     QPalette palt(this->palette());
//     palt.setColor(QPalette::Background, Qt::white);
//     this->setAutoFillBackground(true);
//     this->setPalette(palt);
     //方法二
//       this->setStyleSheet("background-color:white;");
    QLabel * colortext = new QLabel(this);
    QLabel * label = new QLabel("  当前颜色:            ", this);
    QLabel* textguide=new QLabel("  当前字体:  ",this);
    QLabel* textedit=new QLabel("宋体",this);
    QLabel* fontguide=new QLabel("  当前字号:  ",this);
    QLabel* fontedit=new QLabel("9");
    QLabel* blank=new QLabel("  ",this);
    QLabel* blank2=new QLabel("  ",this);
    //colortext->setFont(nullptr);
//    label->setGeometry(QRect(310,-8,60,50));

//    colortext->setText("███");
    colortext->setText("███");
    colortext->setGeometry(QRect(405,-8,100,50));
    //创建工具栏
       QToolBar *tbar = new QToolBar(this);
       addToolBar(Qt::LeftToolBarArea,tbar);
   tbar->setMovable(false);//工具栏不可移动

     QToolBar *tbar2 = new QToolBar(this);
       addToolBar(Qt::TopToolBarArea,tbar2);
       tbar2->setMovable(false);//工具栏不可移动


//    tbar->setIconSize(QSize(26, 26));//设置动作图标的尺寸
//    tbar->setStyleSheet("background-color:rgb(199,237,204)");//背景色

    _Rmenu = new QMenu(this);//创建右键菜单
//    _Rmenu->addAction(tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
//    _Rmenu->addAction(tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
//    _Rmenu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色



    QAction *openasAction = new QAction(tr("&打开"), this);//打开动作
    openasAction->setIcon(QIcon(":/resimage/open.png"));//图标
    openasAction->setShortcut(QKeySequence(tr("Ctrl+O")));//热键
    tbar->addAction(openasAction);//添加到工具栏

    QAction *openAction = new QAction(tr("&打开图片"), this);//打开动作
    openAction->setIcon(QIcon(":/resimage/pic.png"));//图标
    openAction->setShortcut(QKeySequence(tr("Ctrl+ALT+O")));//热键
    tbar->addAction(openAction);//添加到工具栏

    QAction *saveasAction = new QAction(tr("&保存"), this);//保存动作
    saveasAction->setIcon(QIcon(":/resimage/save.png"));//图标
    saveasAction->setShortcut(QKeySequence(tr("Ctrl+S")));//热键
    tbar->addAction(saveasAction);//添加到工具栏
    _Rmenu->addAction(saveasAction);

    QAction *saveAction = new QAction(tr("&另存为"), this);//保存动作
    saveAction->setIcon(QIcon(":/resimage/saveas.png"));//图标
    saveAction->setShortcut(QKeySequence(tr("Ctrl+ALT+S")));//热键
    tbar->addAction(saveAction);//添加到工具栏
    _Rmenu->addAction(saveAction);

    QAction *escAction = new QAction(tr("&退出"), this);//保存动作
    escAction->setIcon(QIcon(":/resimage/esc.png"));//图标
    escAction->setShortcut(QKeySequence(tr("ALT+F4")));//热键
    _Rmenu->addAction(escAction);

    QAction *cancelAction = new QAction(tr("&撤销"),this);
    cancelAction->setIcon(QIcon(":/resimage/cancel.png"));
    saveasAction->setShortcut(QKeySequence(tr("Ctrl+Z")));//热键
    tbar->addAction(cancelAction);
    tbar->addSeparator();

    QAction *copyAction = new QAction(tr("&复制"),this);
    copyAction->setIcon(QIcon(":/resimage/copy.png"));
    copyAction->setShortcut(QKeySequence(tr("Ctrl+C")));//热键
    tbar->addAction(copyAction);
    _Rmenu->addSeparator();
    _Rmenu->addAction(copyAction);
    QAction *shearAction = new QAction(tr("&剪切"),this);
    shearAction->setShortcut(QKeySequence(tr("Ctrl+X")));//热键
    shearAction->setIcon(QIcon(":/resimage/shear.png"));
    tbar->addAction(shearAction);
    _Rmenu->addAction(shearAction);
    QAction *pasteAction = new QAction(tr("&粘贴"),this);
    pasteAction->setShortcut(QKeySequence(tr("Ctrl+V")));//热键
    pasteAction->setIcon(QIcon(":/resimage/paste.png"));
    _Rmenu->addAction(pasteAction);
    tbar->addSeparator();


    QAction *selectAction = new QAction(tr("&选择"),this);
    selectAction->setIcon(QIcon(":/resimage/select.png"));
    tbar->addAction(selectAction);
    tbar->addSeparator();

    QAction *rotateAction = new QAction(tr("&旋转"),this);
    rotateAction->setIcon(QIcon(":/resimage/rotate.png"));
    tbar->addAction(rotateAction);
    QAction *verticalflipAction = new QAction(tr("&垂直翻转"),this);
    verticalflipAction->setIcon(QIcon(":/resimage/vertical_flip.png"));
    tbar->addAction(verticalflipAction);
    QAction *horizontalflipAction = new QAction(tr("&水平翻转"),this);
    horizontalflipAction->setIcon(QIcon(":/resimage/horizontal_flip.png"));
    tbar->addAction(horizontalflipAction);
    tbar->addSeparator();

    QAction *eraserAction = new QAction(tr("&橡皮擦"), this);//橡皮动作
    eraserAction->setIcon(QIcon(":/resimage/eraser.png"));//图标
    eraserAction->setShortcut(QKeySequence(tr("Ctrl+E")));//热键
    tbar2->addAction(eraserAction);//添加到工具栏

    QAction *fillAction = new QAction(tr("&填充"), this);//填充动作
    fillAction->setIcon(QIcon(":/resimage/fillcolor.png"));//图标
    fillAction->setShortcut(QKeySequence(tr("Ctrl+F")));//热键
    tbar2->addAction(fillAction);//添加到工具栏
    tbar2->addSeparator();

    QAction *linesAction = new QAction(tr("&画笔"), this);//铅笔动作
    linesAction->setIcon(QIcon(":/resimage/brush.png"));//图标
    linesAction->setShortcut(QKeySequence(tr("Ctrl+P")));//热键
    tbar2->addAction(linesAction);//添加到工具栏
    tbar2->addSeparator();

    QAction *lineAction = new QAction(tr("&直线"), this);//直线动作
    lineAction->setIcon(QIcon(":/resimage/line.png"));//图标
    lineAction->setShortcut(QKeySequence(tr("Ctrl+L")));//热键
    tbar2->addAction(lineAction);//添加到工具栏



    QAction *rectAction = new QAction(tr("&矩形"), this);//矩形动作
    rectAction->setIcon(QIcon(":/resimage/rectangle.png"));//图标
    rectAction->setShortcut(QKeySequence(tr("Ctrl+R")));//热键
    tbar2->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);//椭圆动作
    ellipseAction->setIcon(QIcon(":/resimage/circle.png"));//图标
    ellipseAction->setShortcut(QKeySequence(tr("Ctrl+E")));//热键
    tbar2->addAction(ellipseAction);

    QAction *triangleAction = new QAction(tr("&三角形"), this);//三角形动作
    triangleAction->setIcon(QIcon(":/resimage/triangle.png"));//图标
    triangleAction->setShortcut(QKeySequence(tr("Ctrl+T")));//热键
//    tbar2->addAction(triangleAction);

    QAction *textAction = new QAction(tr("&文字"), this);//文字动作
    textAction->setIcon(QIcon(":/resimage/text.png"));//图标
    textAction->setShortcut(QKeySequence(tr("Ctrl+T")));//热键
    tbar2->addAction(textAction);
    tbar2->addSeparator();

    QAction *lineeditAction = new QAction(tr("&线条样式"), this);//线条动作
    lineeditAction->setIcon(QIcon(":/resimage/lineedit.png"));//图标
    tbar2->addAction(lineeditAction);

    QAction *texteditAction = new QAction(tr("&文字样式"), this);//文字动作
    texteditAction->setIcon(QIcon(":/resimage/textedit.png"));//图标
    tbar2->addAction(texteditAction);

    QAction *colorAction = new QAction(tr("&颜色设置"), this);
    colorAction->setIcon(QIcon(":/resimage/pallet.png"));//图标
    colorAction->setShortcut(QKeySequence(tr("Ctrl+P")));//热键
    tbar2->addAction(colorAction);
    tbar2->addSeparator();
    tbar2->addWidget(label);
    tbar2->addSeparator();
    tbar2->addWidget(textguide);
    tbar2->addWidget(textedit);
    tbar2->addWidget(blank);
    tbar2->addSeparator();
    tbar2->addWidget(fontguide);
    tbar2->addWidget(fontedit);
    tbar2->addWidget(blank2);
    tbar2->addSeparator();

    QAction *deleteAction = new QAction(tr("清空"),this);
    deleteAction->setIcon(QIcon(":/resimage/delete.png"));
    tbar2->addAction(deleteAction);
    tbar2->addSeparator();

    connect(colorAction,&QAction::triggered,this,[=](){
        QColorDialog::setCustomColor(0, QRgb(0x0000FF));
        color = QColorDialog::getColor(QColor(255,0,0));
        QPalette pl = palette();
        pl.setColor(QPalette::WindowText, QColor(color));
        colortext->setPalette(pl);
        _tEdit->setTextColor(color);
        update();

    });
    connect(lineeditAction,&QAction::triggered,this,[=]()
            {
                bool ok;
                penWidth=QInputDialog::getInt(this,tr("线条宽度"),tr("请输入线条宽度："),1,0,30,1,&ok);
                pen.setWidth(penWidth);
                bool flag;
                QStringList items;
                items<<tr("实线")<<tr("虚线")<<tr("点线");
                        QString item=QInputDialog::getItem(this,tr("线条样式"),tr("请输入或选择一个线条样式"),items,0,true,&flag);
                if (item=="实线")
                    pen.setStyle(Qt::SolidLine);
                if(item=="虚线")
                    pen.setStyle(Qt::DashLine);

                if(item=="点线")
                    pen.setStyle(Qt::DotLine);
                update();
            });
    connect(texteditAction,&QAction::triggered,this,[=](){
        bool flag;
        font=QFontDialog::getFont(&flag,QFont("宋体",9));
        textedit->setText(font.family());
        fontedit->setText(QString::number(font.pointSize()));
        fontchange=true;
    });
    connect(cancelAction,&QAction::triggered,this,[=](){
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
                 case 0: _eraser.pop_back();
                         break;
             }
             _shape.pop_back();
             _drag = 0;//设置为非拖拽模式
             update();
         }
    });

    //连接信号与槽函数
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(saveasAction,SIGNAL(triggered()),this,SLOT(SavePic2()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    connect(openasAction, SIGNAL(triggered()), this, SLOT(OpenPic2()));
    connect(textAction, SIGNAL(triggered()), this, SLOT(Texts()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(Cls()));
    connect(rotateAction, SIGNAL(triggered()), this, SLOT(Rotate()));
    connect(verticalflipAction, SIGNAL(triggered()), this, SLOT(Vertical_flip()));
    connect(horizontalflipAction, SIGNAL(triggered()), this, SLOT(Horizontal_flip()));
    connect(eraserAction, SIGNAL(triggered()), this, SLOT(Eraser()));
    connect(escAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(_tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));

    //选择，复制，粘贴，剪切
    connect(selectAction, SIGNAL(triggered()), this, SLOT(Select()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(Copy()));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(Paste()));
    connect(shearAction, SIGNAL(triggered()), this, SLOT(Select()));


}

void MyPaint::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
{
    _Rmenu->exec(cursor().pos());//在光标位置弹出菜单
}


