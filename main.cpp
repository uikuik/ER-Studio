#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include "UI.h"
#include <thread>
#include "face.h"

void open_f()//打开文件
{
  cout<<"打开文件按钮按下\n";
}
void close_er()//关闭ER
{
  exit(0);
}
int main()
{
  //===========================================初始化
  er_msg.input.push_back(0);
  Tabletop_length_wide();
  hwnd=initgraph(dx,dy,EW_SHOWCONSOLE);
  setbkmode(TRANSPARENT);//启动透明文字
  // thread get_in(get_msg, hwnd, std::ref(msg));
  SetWindowLong(hwnd, GWL_STYLE,GetWindowLong(hwnd, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));// 取消窗口边框
  MoveWindow(hwnd,200,60,889,500,1);
  mainDC = GetImageHDC(NULL);//获取窗口DC
  BeginBatchDraw();

  Li_win_wh();
  FW=336,FH=480;
  fw=336,fh=480;//表盘渲染时的大小 w=x h=y
  fx=(X/2)-168,fy=0;//表盘渲染时的位置
  //----配置文件
  cout<<"\n读取配置文件:\n";
  int draw_fast=0;
  map<string, int*> configVars =
  {
    {"draw_fast", &draw_fast},
    {"examine_face_T", &examine_face_T},
    //  {"save_time", &save_time}
  };
  LoadConfig("data//default.config", &configVars);
  cout<<"draw_fast:"<<draw_fast<<endl;
  cout<<"--结束\n";
  //--------
  fd=vector<face_data>();
  fd_=vector<face_data>();
  fd_AOD=vector<face_data>();

  mainDC = GetImageHDC(NULL);					// 获取主窗口的 DC
  if(0<draw_fast)
    SetStretchBltMode(mainDC, STRETCH_HALFTONE),cout<<"抗锯齿开启\n";	// 设置拉伸贴图模式为抗锯齿
  if(0<examine_face_T)
    cout<<"检查examine_face模块 开启\n";	// 设置拉伸贴图模式为抗锯齿
  //if(0<save_time)
  // cout<<"自动保存模式启动,请注意!!! 间隔时间:"<<save_time<<"秒"<<endl;
  // 如果不设置，默认贴图模式为临近颜色，效果差，性能高


  //===========================================加载图像
  loadimage(&OPNE_F, _T("data//PNG//open_f.png"));
  loadimage(&NEW_F, _T("data//PNG//new_f.png"));
  loadimage(&LOGO, _T("data//PNG//LOGO.png"));
  loadimage(&CLOSE_ER, _T("data//PNG//close_er.png"));
  loadimage(&FACE_M, _T("data//PNG//face_m.png"));
  loadimage(&UI_element, _T("data//PNG//图片组件.png"));
  //===========================================按钮配置
  int _N1_=NEW_Button(735, 28, 760, 50, _T(""), DT_CENTER, close_er);//关闭
  Button_control[_N1_].data=&CLOSE_ER;
  Button_control[_N1_].linecolor_tx=RGB(204,45,29);
  Button_control[_N1_].fillcolor_tx=RGB(204,45,29);

  NEW_Button(95, 0, 175, 25, _T("打开文件"), DT_CENTER, On_btnOk_Click);//X+80
  NEW_Button(175, 0, 255, 25, _T("保存文件"), DT_CENTER, On_save);
  NEW_Button(255, 0, 335, 25, _T("新建控件"), DT_CENTER, open_f);
  NEW_Button(335, 0, 415, 25, _T("预览图"), DT_CENTER, open_f);
  NEW_Button(415, 0, 495, 25, _T("切换显示"), DT_CENTER, open_f);
  NEW_Button(495, 0, 575, 25, _T("编译"), DT_CENTER, open_f);
  NEW_Button(575, 0, 655, 25, _T("设置"), DT_CENTER, open_f);
  NEW_Button(655, 0, 690, 25, _T("..."), DT_CENTER, open_f);
  NEW_Checkbox(100,100,15,15);

  _N1_=NEW_Button(0, 28, 40, 48, _T(""), DT_CENTER, open_f);//X+40打开文件
  Button_control[_N1_].data=&OPNE_F;
  _N1_=NEW_Button(41, 28, 81, 48, _T(""), DT_CENTER, open_f);//X+40新建
  Button_control[_N1_].data=&NEW_F;

  _N1_=NEW_Button(0,52,45, 95, _T(""), DT_CENTER, open_f);//Y+51表盘制作
  Button_control[_N1_].data=&FACE_M;
  Button_control[_N1_].linecolor=RGB(33,43,53);
  Button_control[_N1_].fillcolor=RGB(33,43,53);


  //
  while(1)
    {
      cleardevice();

      // auxiliary_make(fx,fy,fw,fh,FW,FH,"MI band 8pro",fd,fd_N,&face_image);//辅助线
      //transparentimage3(&face_image,0,0,&Frame_M);
      int oldfillcolor = getfillcolor();
      setfillcolor(RGB(11,22,29));//工作区
      solidrectangle(0,51,X,Y-18);
      setfillcolor(oldfillcolor);

      IMAGE face_image=draw_face(face_result,fd);//读取虚拟编辑区
      imgDC = GetImageHDC(&face_image);//获取DC
      info(msg,face_image,er_msg);
      draw_fd_N(msg,&face_image,fd_N);//渲染高亮
      StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式

      draw_UI(1);//渲染UI
      GetMousePosInClient(hwnd,msx,msy);//获取消息
      NO_USE_UI(hwnd);//拖动窗口
      string _o="mx:";
      _o+=to_string(msx),_o+=" my:",_o+=to_string(msy),_o+=" WX:",_o+=to_string(X),_o+=" WY:",_o+=to_string(Y),_o+=" FPS:",_o+=to_string(FPS),line(msx-6,msy-6,msx+6,msy+6),line(msx+6,msy-6,msx-6,msy+6),outtextxy(0,Y-16,(_o.c_str()));


      FlushBatchDraw();
      CalculateFrameRate();//计算渲染帧数的函数
    }
  EndBatchDraw();

  getch();
  closegraph();
  return 0;
}