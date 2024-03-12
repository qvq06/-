#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include "QDebug"
#include "QPainter"
#include "QMenu"
#include "QAction"
#include "QComboBox"
#include "QString"
#include "QStack"
#include "QMessageBox"
#include "QLabel"
#include "QtMath"
#include <QDesktopServices>
#include <QUrl>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ditu_width=ui->label->width();
    ditu_height=ui->label->height();

    ditu_x=0;
    ditu_y=0;
    cnt = 0;

    QPixmap temp(":/map.jpg");
    temp.save("temp.png");
    yuantu=temp;
    yuantu=yuantu.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setGeometry(0,0,ditu_width,ditu_height);
    ui->label->setPixmap(yuantu);

    huitu_button_cnt=51;
    setWindowTitle("安徽大学校园导航");
    lcdNumber();
    didianzuobiao();
    guanjiandianzuobiao();
    biandejianli();
    julijuzheng(); //距离矩阵建立
    label_10();
    xinxijianli(); //信息建立
   // jisuanZuiduanLujing();

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::label_10(){                  //查看天气，调用系统默认浏览器打开指定网页

    ui->label_10->setText("<a href='https://caiyunapp.com/weather/'>点我查看天气情况</a>");
    ui->label_10->setOpenExternalLinks(true);
//    QDesktopServices::openUrl(QUrl("https://caiyunapp.com/weather/"));
}

void MainWindow::mousePressEvent(QMouseEvent *e){             //鼠标单击输出点坐标，读取坐标时使用
     last=e->globalPos();
    if(xinxi_flag==0){
         xinxi_flag=1;
         qDebug()<<"xinxi_weizhi["<<xinxi_cnt<<"].x1="<<e->x()<<",xinxi_weizhi["<<xinxi_cnt<<"].y1="<<e->y()<<";";
         huitu_button[xinxi_cnt]=new QPushButton(this);
//         connect(huitu_button[huitu_button_cnt], SIGNAL(clicked()), this, SLOT(huituClicked()));
         huitu_button[xinxi_cnt]->setObjectName(QString::number(xinxi_cnt));
         huitu_button[xinxi_cnt]->setText(QString::number(xinxi_cnt));
         huitu_button[xinxi_cnt]->setGeometry(e->x(),e->y(),20,20);
//         huitu_button[xinxi_cnt]->show();

    }
    else if(xinxi_flag==1){
        xinxi_flag=0;
        qDebug()<<"xinxi_weizhi["<<xinxi_cnt<<"].x2="<<e->x()<<",xinxi_weizhi["<<xinxi_cnt<<"].y2="<<e->y()<<";";
        xinxi_cnt++;
    }

}

/*void MainWindow::mouseMoveEvent(QMouseEvent *e){              //实现鼠标拖动地图

    qDebug()<<"move";
      ditu_x=ui->label->x();
      ditu_y=ui->label->y();
      int dx=e->globalX()-last.x();
      int dy=e->globalY()-last.y();
      last=e->globalPos();
      ditu_x+=dx;
      ditu_y+=dy;
      ui->label->setGeometry(ditu_x,ditu_y,ui->label->width(),ui->label->height());
}*/

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
//  qDebug()<<"release";
    mouse_flag=0;
}

void MainWindow::paintEvent(QPaintEvent *e){   //绘图点
//    qDebug()<<"paint";
//    QPainter painter(ui->label);
//    QPixmap pix(":/map/map.jpg");
//    painter.drawPixmap(0,0,ditu_width,ditu_height,pix);
//    update();
}

void MainWindow::wheelEvent(QWheelEvent *e){          //实现鼠标滚轮缩放
    if(e->delta()>0){
        if(suofang_flag==0){
            ditu_width*=1.1;
            ditu_height*=1.1;
            beishu=beishu*1.1;
            yuantu.load("temp.png");
            yuantu=yuantu.scaled(ditu_width, ditu_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->label->setPixmap(yuantu);
            ui->label->setGeometry(ui->label->x(),ui->label->y(),ditu_width,ditu_height);
            for(int i=0;i<XinxiNumber;i++){
               xinxi[i]->setGeometry(xinxi_weizhi[i].x1*beishu,xinxi_weizhi[i].y1*beishu,(xinxi_weizhi[i].x2-xinxi_weizhi[i].x1)*beishu,(xinxi_weizhi[i].y2-xinxi_weizhi[i].y1)*beishu);
            }
         }
        else if(suofang_flag==1){
            QMessageBox *suofang_jinggao=new QMessageBox(this);
            suofang_jinggao->setText("正在进行路径规划，无法缩放，请稍候");
            suofang_jinggao->setWindowTitle("警告信息");
            suofang_jinggao->show();
        }

    }
    else if (e->delta()<0){

        if(suofang_flag==0){
            ditu_width*=0.9;
            ditu_height*=0.9;
            beishu=beishu*0.9;
            yuantu.load("temp.png");
            yuantu=yuantu.scaled(ditu_width, ditu_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->label->setPixmap(yuantu);
            ui->label->setGeometry(ui->label->x(),ui->label->y(),ditu_width,ditu_height);
            for(int i=0;i<XinxiNumber;i++){
               xinxi[i]->setGeometry(xinxi_weizhi[i].x1*beishu,xinxi_weizhi[i].y1*beishu,(xinxi_weizhi[i].x2-xinxi_weizhi[i].x1)*beishu,(xinxi_weizhi[i].y2-xinxi_weizhi[i].y1)*beishu);
            }
         }
        else if(suofang_flag==1){
            QMessageBox *suofang_jinggao=new QMessageBox(this);
            suofang_jinggao->setText("正在进行路径规划，无法缩放，请稍候");
            suofang_jinggao->setWindowTitle("警告信息");
            suofang_jinggao->show();
        }
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)         //选择起点
{
    if(arg1=="西门") s=0;
    else if(arg1=="桔园餐厅）")s=1;
    else if(arg1==" 蕙园")s=2;
    else if(arg1=="理工楼C座")s=3;
    else if(arg1=="南体育场")s=4;
    else if(arg1=="槐园")s=5;
    else if(arg1=="笃行南楼")s=6;
    else if(arg1=="梅园）")s=7;

}

void MainWindow::didianzuobiao(){                    //地点坐标
    didian[0].x=389,didian[0].y=552;
    didian[1].x=328,didian[1].y=491;
    didian[2].x=184,didian[2].y=365;
    didian[3].x=301,didian[3].y=347;
    didian[4].x=572,didian[4].y=396;
    didian[5].x=520,didian[5].y=464;
    didian[6].x=429,didian[6].y=448;
    didian[7].x=450,didian[7].y=519;

}

void MainWindow::guanjiandianzuobiao(){                              //关键点坐标
    guanjiandian[0].x=389,guanjiandian[0].y=552,guanjiandian[ 0 ].duiyingjianzhu= 0 ;
    guanjiandian[1].x=328,guanjiandian[1].y=491,guanjiandian[ 1 ].duiyingjianzhu= 1 ;
    guanjiandian[2].x=184,guanjiandian[2].y=365,guanjiandian[ 2 ].duiyingjianzhu= 2 ;
    guanjiandian[3].x=301,guanjiandian[3].y=353,guanjiandian[ 3 ].duiyingjianzhu= 3 ;
    guanjiandian[4].x=572,guanjiandian[4].y=396,guanjiandian[ 4 ].duiyingjianzhu= 4 ;
    guanjiandian[5].x=520,guanjiandian[5].y=464,guanjiandian[ 5 ].duiyingjianzhu= 5 ;
    guanjiandian[6].x=429,guanjiandian[6].y=448,guanjiandian[ 6 ].duiyingjianzhu= 6 ;
    guanjiandian[7].x=450,guanjiandian[7].y=497,guanjiandian[ 7 ].duiyingjianzhu= 7 ;

    guanjiandian[8].x=407,guanjiandian[8].y=527 ;
    guanjiandian[9].x=387,guanjiandian[9].y=506 ;
    guanjiandian[10].x=333,guanjiandian[10].y=502;

    guanjiandian[11].x=293,guanjiandian[11].y=443;
    guanjiandian[12].x=247,guanjiandian[12].y=433;
    guanjiandian[13].x=243,guanjiandian[13].y=386;
    guanjiandian[14].x=181,guanjiandian[14].y=385;
    guanjiandian[15].x=156,guanjiandian[15].y=367;
    guanjiandian[16].x=187,guanjiandian[16].y=366;

    guanjiandian[17].x=276,guanjiandian[17].y=364;
    guanjiandian[18].x=288,guanjiandian[18].y=373;

    guanjiandian[19].x=382,guanjiandian[19].y=369;
    guanjiandian[20].x=384,guanjiandian[20].y=383;
    guanjiandian[21].x=478,guanjiandian[21].y=383;
    guanjiandian[22].x=513,guanjiandian[22].y=389;
    guanjiandian[23].x=557,guanjiandian[23].y=376;

    guanjiandian[24].x=557,guanjiandian[24].y=424;
    guanjiandian[25].x=537,guanjiandian[25].y=469;
    guanjiandian[26].x=531,guanjiandian[26].y=455;
    guanjiandian[27].x=509,guanjiandian[27].y=471;

    guanjiandian[ 28 ].x= 504 ,guanjiandian[ 28 ].y= 480;
    guanjiandian[ 29 ].x= 464 ,guanjiandian[ 29 ].y= 478 ;
    guanjiandian[ 30 ].x= 433 ,guanjiandian[ 30 ].y= 492 ;
    guanjiandian[ 31 ].x= 429 ,guanjiandian[ 31 ].y= 438 ;

    guanjiandian[ 32 ].x= 432 ,guanjiandian[ 32 ].y= 518 ;
    guanjiandian[ 33 ].x= 449 ,guanjiandian[ 33 ].y= 518 ;

    guanjiandian[ 34 ].x= 448 ,guanjiandian[ 34 ].y= 521 ;
    guanjiandian[ 35 ].x= 427 ,guanjiandian[ 35 ].y= 521 ;
    guanjiandian[ 36 ].x= 399 ,guanjiandian[ 36 ].y= 544 ;

    guanjiandian[ 37 ].x= 324 ,guanjiandian[ 37 ].y= 474 ;
    guanjiandian[ 38 ].x= 340 ,guanjiandian[ 38 ].y= 454 ;
    guanjiandian[ 39 ].x= 313 ,guanjiandian[ 39 ].y= 433 ;
    guanjiandian[ 40 ].x= 317 ,guanjiandian[ 40 ].y= 414 ;
    guanjiandian[ 41 ].x= 297 ,guanjiandian[ 41 ].y= 387 ;
    guanjiandian[ 42 ].x= 281 ,guanjiandian[ 42 ].y= 367 ;


    guanjiandian[ 43 ].x= 308 ,guanjiandian[ 43 ].y= 371 ;
    guanjiandian[ 44 ].x= 382 ,guanjiandian[ 44 ].y= 369 ;
    guanjiandian[ 45 ].x= 385 ,guanjiandian[ 45 ].y= 428 ;
    guanjiandian[ 46 ].x= 427 ,guanjiandian[ 46 ].y= 430 ;
    guanjiandian[ 47 ].x= 428 ,guanjiandian[ 47 ].y= 452 ;
}

void MainWindow::huituClicked(){             //测试用
    huitu_button_cnt--;
    delete huitu_button[huitu_button_cnt];
}

void MainWindow::on_pushButton_clicked()     //确定按钮
{
    if(suofang_flag==0){
        if(s==e){
           QMessageBox *message=new QMessageBox(this);
           message->setText("起点和终点不能相同！");
           message->setWindowTitle("警告信息");
           message->show();
        }
        else {
            jisuanZuiduanLujing(s);
            lujingjisuan(e,s);
            huaxian();
            lcdNumber();
        }
    }
    else if(suofang_flag==1){
        QMessageBox *message=new QMessageBox(this);
        message->setText("请等待本次路径规划完毕再进行下次路径规划！");
        message->setWindowTitle("警告信息");
        message->show();
    }
//  huitu();
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)         //选择终点
{
    if(arg1=="西门") e=0;
    else if(arg1=="桔园餐厅")e=1;
    else if(arg1==" 蕙园")e=2;
    else if(arg1=="理工楼C座")e=3;
    else if(arg1=="南体育场")e=4;
    else if(arg1=="槐园")e=5;
    else if(arg1=="笃行南楼")e=6;
    else if(arg1=="梅园")e=7;

}

void MainWindow::edgeClicked(){
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    int i=clickedButton->text().toInt();
    if(edge_huitu_flag==0){
        e_s=i;
        edge_huitu_flag=1;
    }
    else if(edge_huitu_flag==1){
        e_e=i;
        edge_huitu_flag=0;
        qDebug()<<"edge["<<edge_cnt<<"].start="<<e_s<<",edge["<<edge_cnt<<"].end="<<e_e<<";";
        edge_cnt++;
        //      edge[0].start=0,edge[0].end=1;
    }
}

void MainWindow::biandejianli(){                      //定义边（无方向）
    edge[ 0 ].start=0 ,edge[ 0 ].end=8;
    edge[ 1 ].start=8 ,edge[ 1 ].end=9;
    edge[ 2 ].start=9 ,edge[ 2 ].end=10;
    edge[ 3 ].start=10 ,edge[ 3 ].end=1;

    edge[ 4 ].start=1 ,edge[ 4 ].end=11;
    edge[ 5 ].start=11 ,edge[ 5 ].end=12;
    edge[ 6 ].start=12 ,edge[ 6 ].end=13;
    edge[ 7 ].start=13 ,edge[ 7 ].end=14;
    edge[ 8 ].start=14 ,edge[ 8 ].end=15;
    edge[ 9 ].start=15 ,edge[ 9 ].end=16;
    edge[ 10 ].start=16 ,edge[ 10 ].end=2;

    edge[ 11 ].start=2 ,edge[ 11 ].end=17;
    edge[ 12 ].start=17 ,edge[ 12 ].end=18;
    edge[ 13 ].start=18 ,edge[ 13 ].end=3;

    edge[ 14 ].start=3 ,edge[ 14 ].end=19;
    edge[ 15 ].start=19 ,edge[ 15 ].end=20;
    edge[ 16 ].start=20 ,edge[ 16 ].end=21;
    edge[ 17 ].start=21 ,edge[ 17 ].end=22;
    edge[ 18 ].start=22 ,edge[ 18 ].end=23;
    edge[ 19 ].start=23 ,edge[ 19 ].end=4;

    edge[ 20 ].start=4 ,edge[ 20 ].end=24;
    edge[ 21 ].start=24 ,edge[ 21 ].end=25;
    edge[ 22 ].start=25 ,edge[ 22 ].end=26;
    edge[ 23 ].start=26 ,edge[ 23 ].end=27;
    edge[ 24 ].start=27 ,edge[ 24 ].end=5;

    edge[ 25 ].start=5 ,edge[ 25 ].end=28;
    edge[ 26 ].start=28 ,edge[ 26 ].end=29;
    edge[ 27 ].start=29 ,edge[ 27 ].end=30;
    edge[ 28 ].start=30 ,edge[ 28 ].end=31;
    edge[ 29 ].start=31 ,edge[ 29 ].end=6;

    edge[ 30 ].start=6 ,edge[ 30 ].end=32;
    edge[ 31 ].start=32 ,edge[ 31 ].end=33;
    edge[ 32 ].start=33 ,edge[ 32 ].end=7;

    edge[ 33 ].start=7 ,edge[ 33 ].end=34;
    edge[ 34 ].start=34 ,edge[ 34 ].end=35;
    edge[ 35 ].start=35 ,edge[ 35 ].end=36;
    edge[ 36 ].start=36 ,edge[ 36 ].end=0;

    edge[ 37 ].start=1 ,edge[ 37 ].end=37;
    edge[ 38 ].start=37 ,edge[ 38 ].end=38;
    edge[ 39 ].start=38 ,edge[ 39 ].end=39;
    edge[ 40 ].start=39 ,edge[ 40 ].end=40;
    edge[ 41 ].start=40 ,edge[ 41 ].end=41;
    edge[ 42 ].start=41 ,edge[ 42 ].end=42;
    edge[ 43 ].start=42 ,edge[ 43 ].end=3;

    edge[ 44 ].start=3 ,edge[ 44 ].end=43;
    edge[ 45 ].start=43 ,edge[ 45 ].end=44;
    edge[ 46 ].start=44 ,edge[ 46 ].end=45;
    edge[ 47 ].start=45 ,edge[ 47 ].end=46;
    edge[ 48 ].start=46 ,edge[ 48 ].end=47;
    edge[ 49 ].start=47 ,edge[ 49 ].end=6;


}

void MainWindow::julijuzheng(){                //将边的距离输入到数组
    for(int i=0;i<GuanjiandianNumber;i++){
        for(int j=0;j<GuanjiandianNumber;j++){
            juli[i][j]=0;
        }
    }
    for(int i=0;i<EdgeNumber;i++){
        juli[edge[i].start][edge[i].end]=juli[edge[i].end][edge[i].start]= sqrt(
                (guanjiandian[edge[i].start].x-guanjiandian[edge[i].end].x)*
                (guanjiandian[edge[i].start].x-guanjiandian[edge[i].end].x)+
                (guanjiandian[edge[i].start].y-guanjiandian[edge[i].end].y)*
                (guanjiandian[edge[i].start].y-guanjiandian[edge[i].end].y));
    }
}

void MainWindow::jisuanZuiduanLujing(int s)  //最短路径迪杰斯特拉算法
{
    bool fangwen_flag[GuanjiandianNumber];
    for(int i=0;i<GuanjiandianNumber;i++)
     {
        if(juli[s][i]>0&&i!=s)
        {
            gengxin[i]=juli[s][i];
            lujing[i]=s;
         }
        else
        {
            gengxin[i]=INT_MAX;
            lujing[i]=-1;
        }
        fangwen_flag[i]=false;
        lujing[s]=s;
        gengxin[s]=0;
    }
    fangwen_flag[s]=true;
    for(int i=1;i<GuanjiandianNumber;i++)
     {
        int min=INT_MAX;
        int temp = 0;
        for(int j=0;j<GuanjiandianNumber;j++)
         {
            if(fangwen_flag[j]==false&&gengxin[j]<min)
            {
                min=gengxin[j];
                temp=j;
            }
        }
        fangwen_flag[temp]=true;
        for(int k=0;k<GuanjiandianNumber;k++)
         {
            if(fangwen_flag[k]==false&&juli[temp][k]>0&&min+juli[temp][k]<gengxin[k])
            {
                gengxin[k]=min+juli[temp][k];
                lujing[k]=temp;
            }
        }
    }
}

void MainWindow::lujingjisuan(int e,int s)   //规划存储路径,建立栈
{
    for(int i=0;i<GuanjiandianNumber;i++)
        zuiduan_lujing[i]=-1;

    QTextStream cout(stdout,  QIODevice::WriteOnly);
    QStack<int> stack;
    while(e!=s)
    {
        stack.push(e);
        e=lujing[e];
    }
    stack.push(e);
    while(!stack.empty())
    {
        cout<<stack.top()<<" ";
        zuiduan_lujing[cnt++]=stack.top();
        stack.pop();                       //出栈，输出路线经过的关键点
    }

}

void MainWindow::huaxian(){             //画出路径
        int s=this->s;
        int e=this->e;
        //画线
        suofang_flag=1;
        QPixmap pixmap_yuantu(":/map.jpg");
        pixmap_yuantu = pixmap_yuantu.scaled(ditu_width,ditu_height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        QPainter llpainter(&pixmap_yuantu);
        llpainter.setRenderHint(QPainter::Antialiasing);
        llpainter.setPen(QPen(Qt::black,5,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
       /* if(s==0)
             llpainter.drawLine(515*beishu, (191-40)*beishu, guanjiandian[s].x*beishu, (guanjiandian[s].y-40)*beishu);*/
        //  if(s!=23&&s!=24&&s!=25&&s!=26)
        llpainter.drawLine(didian[s].x*beishu, (didian[s].y)*beishu, guanjiandian[s].x*beishu, (guanjiandian[s].y)*beishu);

      for (int i=1;zuiduan_lujing[i]!=-1;i++)
            {
                llpainter.drawLine(guanjiandian[zuiduan_lujing[i-1]].x*beishu, (guanjiandian[zuiduan_lujing[i-1]].y)*beishu, guanjiandian[zuiduan_lujing[i]].x*beishu, (guanjiandian[zuiduan_lujing[i]].y)*beishu);
                //延时
                sleep(25);
                QPixmap pixmap = pixmap_yuantu.scaled(ditu_width,ditu_height);
                QPainter painter(this);

                yuantu=pixmap;
                ui->label->setPixmap(yuantu);
                yuantu.save("temp.png");
            }
            llpainter.setPen(QPen(Qt::red,5,Qt::NoPen,Qt::RoundCap,Qt::MiterJoin)); //设置虚线
            if(e==0)
                llpainter.drawLine(515*beishu, (191-40)*beishu, guanjiandian[e].x*beishu, (guanjiandian[e].y-40)*beishu);
            else if(e!=23&&e!=24&&e!=25&&e!=26)
            llpainter.drawLine(didian[e].x*beishu, (didian[e].y-40)*beishu, guanjiandian[e].x*beishu, (guanjiandian[e].y-40)*beishu);
            llpainter.end();

            QPixmap pixmap = pixmap_yuantu.scaled(ditu_width,ditu_height);
            QPainter painter(this);
            yuantu=pixmap;
            ui->label->setPixmap(yuantu);
            yuantu.save("temp.png");
            suofang_flag=0;
}

void MainWindow::on_pushButton_2_clicked(){      //重置按钮
    if(suofang_flag==0)
    {
        QPixmap temp(":/map.jpg");
        temp.save("temp.png");
        yuantu=temp;
        yuantu=yuantu.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->label->setPixmap(yuantu);
        distance = 0;
        time = 0;
        ui->lcdNumber->display(0);
        ui->lcdNumber_2->display(0);
    }
    else if(suofang_flag==1){
        QMessageBox *qingchu_jinggao=new QMessageBox(this);
        qingchu_jinggao->setText("请等待路径规划完毕再清除!");
        qingchu_jinggao->setWindowTitle("警告信息");
        qingchu_jinggao->show();
    }

}


void MainWindow::xinxijianli(){     //读取所需点坐标，设计用
    //信息标签初始化祖先
    for (int i=0;i<XinxiNumber;i++){
        xinxi[i]=new QLabel(ui->label);
    }

  //  xinxizuobiao();
//    信息位置与大小初始化
    for(int i=0;i<XinxiNumber;i++){
       xinxi[i]->setGeometry(xinxi_weizhi[i].x1,xinxi_weizhi[i].y1,xinxi_weizhi[i].x2-xinxi_weizhi[i].x1,xinxi_weizhi[i].y2-xinxi_weizhi[i].y1);

       xinxi[i]->show();
    }
}

/*void MainWindow::xinxizuobiao (){ //存储信息位置坐标x1，y1,x2,y2

    for(int i=0;i<XinxiNumber;i++)
        xinxi_weizhi[i].y1-=11,xinxi_weizhi[i].y2-=11;
}
*/

void MainWindow::sleep(unsigned int msec){            //延时
    QTime reachTime =QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,1);
    }
}

void MainWindow::lcdNumber(){
    int i = 0;
       for(i=1;i<cnt;i++){
          distance = distance + juli[zuiduan_lujing[i-1]][zuiduan_lujing[i]];
       }
    distance = distance*2.07;
 //   qDebug("%d",cnt);

    ui->lcdNumber->display(distance);
 //   ui->lcdNumber_2->display(time);
    cnt = 0;
}
void MainWindow::on_comboBox_3_activated(const QString &arg1) //选择出行方式
{
    if(arg1=="骑行")
        v = v_bicycle;
    else if(arg1=="步行")
        v = v_walk;
     time = distance/v;
     if(time == 0)
      time = 1;
    ui->lcdNumber_2->display(time);
}
