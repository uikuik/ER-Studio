#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <atomic>
using namespace std;
IMAGE LOGO;//LOGO
IMAGE NEW_F;//新建文件
IMAGE OPNE_F;//打开文件
IMAGE CLOSE_ER;//关闭ER
IMAGE FACE_M;//关闭ER

IMAGE UI_element;//图片组件配置页面

HWND hwnd;//窗口句柄
int X=0;//窗口坐标
int Y=0;
int msy=0,msx=0;//鼠标坐标
int dx=1280,dy=700;//桌面大小

bool Li_win_wh()//检查窗口高度变化
{
  RECT rect;
  GetWindowRect(hwnd, &rect);
  if((X!=(rect.right - rect.left))||(Y!=(rect.bottom - rect.top)))
    {
      X = rect.right - rect.left;
      Y = rect.bottom - rect.top;
      return 1;
    }
  return 0;
}
std::wstring s2ws(const std::string& s)
{
  int len;
  int slength = (int)s.length() + 1;
  len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
  wchar_t* buf = new wchar_t[len];
  MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
  std::wstring r(buf);
  delete[] buf;
  return r;
}
void Tabletop_length_wide()//计算桌面的大小
{
  // 获取屏幕的长宽
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);
  // 获取任务栏的高度
  string o="Shell_TrayWnd";
  wchar_t wBuffer[256] = {0};
  std::wstring wStr = s2ws(o);
  wmemcpy_s(wBuffer, 256, wStr.c_str(), wStr.size());
  LPWSTR lpw = wBuffer;
  HWND taskbar = FindWindowW(lpw, NULL);
  RECT taskbarRect;
  GetWindowRect(taskbar, &taskbarRect);
  int taskbarHeight = taskbarRect.bottom - taskbarRect.top;
  // 计算桌面的长宽（不包括任务栏）
  dx = screenWidth;
  dy = screenHeight - taskbarHeight;
  // 输出结果
  printf("桌面宽度（不包括任务栏）：%d\n", dx);
  printf("桌面高度（不包括任务栏）：%d\n", dy);
}

void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //新版png
{
  HDC dstDC = GetImageHDC(dstimg);
  HDC srcDC = GetImageHDC(srcimg);
  int w = srcimg->getwidth();
  int h = srcimg->getheight();
  BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}


void GetMousePosInClient(HWND hWnd,int &x,int &y)//指针相对于这个窗口的工作区的坐标
{
  POINT pt;
  GetCursorPos(&pt);
  RECT rect;
  GetClientRect(hWnd, &rect);
  ClientToScreen(hWnd, (LPPOINT)&rect);
  pt.x -= rect.left;
  pt.y -= rect.top;
  x=pt.x;
  y=pt.y;
}

class ER_MSG//输入消息
{
  public:
    int x=0;
    int y=0;//光标坐标
    vector<int> input;//输入消息缓冲区 1左键 -1左键弹起 2右键 -2右键弹起
} er_msg;

atomic<bool> t_rw;//是否有函数访问msg
void get_msg(HWND hWnd,ER_MSG &m)//获取输入消息
{
  if(t_rw==0)//没有函数访问msg
    {
      t_rw=1;
      if(m.input.size()>500)
        m.input.erase(m.input.begin()); // 删除头部元素

      GetMousePosInClient(hWnd,m.x,m.y);

      if(GetAsyncKeyState(1) & 0x8000)//左键
        m.input.push_back(1);
      if((!m.input.empty())&&!(GetAsyncKeyState(1) & 0x8000)&& m.input.back()==1)
        m.input.push_back(-1);

      if(GetAsyncKeyState(2) & 0x8000)//右键
        m.input.push_back(2);
      if((!m.input.empty())&&!(GetAsyncKeyState(2) & 0x8000)&& m.input.back()==2)
        m.input.push_back(-2);

      if(GetAsyncKeyState(17) & 0x8000)//Ctrl
        m.input.push_back(17);
      if((!m.input.empty())&&!(GetAsyncKeyState(17) & 0x8000)&& m.input.back()==17)
        m.input.push_back(-17);

      t_rw=0;
    }
}

class ER_Button //按钮
{
  public:
    int left = 0, top = 0, right = 0, bottom = 0;//控件坐标
    UINT Format; //文本渲染方式
    const TCHAR* str = nullptr; // 控件内容
    // 定义一个函数指针类型
    using FunctionPtr = std::function<void()>;
    FunctionPtr func; // 函数指针
    string name="?";//名字 用于管理页面
    int line_width=2;//线的宽度
    IMAGE *data=NULL;
    COLORREF linecolor=RGB(27,35,44);//边框线颜色
    COLORREF fillcolor=RGB(27,35,44);//填充颜色
    COLORREF textcolor=RGB(255,255,255);//文本颜色

    COLORREF fillcolor_tx=RGB(46,60,74);//鼠标悬停填充颜色
    COLORREF linecolor_tx=RGB(46,60,74);//鼠标悬停线颜色

    COLORREF linecolor_cl=RGB(243,102,56);//鼠标按下线颜色
    COLORREF fillcolor_cl=RGB(243,102,56);//鼠标按下填充颜色

    //初始化
    void Ini_Button(int Left, int Top, int Right, int Bottom, const TCHAR* title, UINT I_Format = DT_CENTER, FunctionPtr _FunctionPtr = nullptr)
    {
      left = Left;
      top = Top;
      right = Right;
      bottom = Bottom;
      str = _tcsdup(title); // 复制一份标题
      Format = I_Format;
      func = _FunctionPtr;
    }
    void draw(bool TX,bool cl)//渲染 输入是否有鼠标悬停 按下
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinestyle(PS_SOLID,line_width);
      if(TX==0)
        {
          setlinecolor(linecolor);		 // 设置画线颜色
          settextcolor(textcolor);//设置字体颜色
          setfillcolor(fillcolor); // 设置填充颜色
          //setbkcolor(fillcolor);
        }
      else
        {
          setlinecolor(linecolor_tx);		 // 设置画线颜色
          settextcolor(textcolor);//设置字体颜色
          setfillcolor(fillcolor_tx); // 设置填充颜色
          //setbkcolor(fillcolor_tx);
        }
      if(cl)
        {
          setlinecolor(linecolor_cl);		 // 设置画线颜色
          setfillcolor(fillcolor_cl); // 设置填充颜色
          func();
        }
      fillrectangle(left, top, right, bottom);
      outtextxy(left + (right - left - textwidth(str) + 1) / 2, top + (bottom - top - textheight(str) + 1) / 2, str);
      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
      setlinestyle(PS_SOLID,1);
      if(data!=NULL)
        {
          transparentimage3(NULL,left + (right - left - data->getwidth() + 1) / 2, top + (bottom - top - data->getheight() + 1) / 2,data);
        }
    }
    bool MOUS(int x, int y)//鼠标检测
    {
      return (left <= x && x <= right && top <= y && y <= bottom);
    }
    void OnMessage(int msx,int msy,bool cl)//按钮消息
    {
      int t=0;
      if(MOUS(msx,msy))
        t=1;
      draw(t,(cl&&MOUS(msx,msy)));
    }
};

class ER_Checkbox
{
  public:
    int x = 0, y = 0, w = 0, h = 0;//控件坐标
    bool Check=0;//是否按下
    COLORREF linecolor=RGB(59,69,80);//边框线颜色
    COLORREF fillcolor=RGB(27,35,44);//填充颜色
    int line_width=2;//线的宽度
    string name="?";//名字 用于管理页面
    COLORREF fillcolor_tx=RGB(59,69,80);//鼠标悬停填充颜色
    COLORREF linecolor_tx=RGB(59,69,80);//鼠标悬停线颜色

    COLORREF linecolor_cl=RGB(243,102,56);//鼠标按下线颜色
    COLORREF fillcolor_cl=RGB(243,102,56);//鼠标按下填充颜色
    void Ini_Checkbox(int X, int Y, int W, int H,bool CL=0)
    {
      x = X;
      y = Y;
      w = W;
      h = H;
      Check=CL;
    }
    void draw(bool TX,bool cl)//渲染 输入是否有鼠标悬停 按下
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinestyle(PS_SOLID,line_width);
      if(TX==0)
        {
          setlinecolor(linecolor);		 // 设置画线颜色
          setfillcolor(fillcolor); // 设置填充颜色
          //setbkcolor(fillcolor);
        }
      else
        {
          setlinecolor(linecolor_tx);		 // 设置画线颜色
          setfillcolor(fillcolor_tx); // 设置填充颜色
          //setbkcolor(fillcolor_tx);
        }
      fillrectangle(x,y,x+w,y+h);
      if(cl)
        {
          Check=!Check;
          while(GetAsyncKeyState(1) & 0x8000)
            {
            }
        }
      if(Check)
        {
          setlinecolor(linecolor_cl);		 // 设置画线颜色
          setfillcolor(fillcolor_cl); // 设置填充颜色
          line(x,y,x+(w/2),y+h);
          line(x+(w/2),y+h,x+w,y-(h*0.6));
        }
      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
      setlinestyle(PS_SOLID,1);
    }
    bool MOUS(int mx, int my)//鼠标检测
    {
      return (x <= mx && mx <= x+w && y <= my && my <= y+h);
    }
    void OnMessage(int msx,int msy,bool cl)//按钮消息
    {
      int t=0;
      if(MOUS(msx,msy))
        t=1;
      draw(t,(cl&&MOUS(msx,msy)));
    }
};

vector<ER_Button> Button_control;//按钮控件管理
using FunctionPtr2 = std::function<void()>;//函数指针
int NEW_Button(int Left, int Top, int Right, int Bottom, const TCHAR* title, UINT I_Format = DT_CENTER, FunctionPtr2 _FunctionPtr = nullptr,string name="?")
{
  ER_Button* p = new ER_Button;
  p->Ini_Button(Left, Top, Right, Bottom, title, I_Format, _FunctionPtr);
  p->name=name;
  Button_control.push_back(*p);
  return (Button_control.size())-1;
}

vector<ER_Checkbox> Checkbox_control;//复选框控件管理
int NEW_Checkbox(int x, int y, int w, int h,string name="?")
{
  ER_Checkbox* p = new ER_Checkbox;
  p->Ini_Checkbox(x,y,w,h,0);
  p->name=name;
  Checkbox_control.push_back(*p);
  return (Checkbox_control.size())-1;
}

void Button_msg(ER_MSG &m)//按钮消息循环
{
  for(int i=0; i<Button_control.size(); i++)
    {
      if(t_rw==0)
        {
          t_rw=1;
          if(!m.input.empty())
            {
              if(m.input.back()==1)
                Button_control[i].OnMessage(m.x,m.y,1);

              Button_control[i].OnMessage(m.x,m.y,0);
            }
          else
            {
              Button_control[i].OnMessage(m.x,m.y,0);
            }
          t_rw=0;
        }
    }
  for(int i=0; i<Checkbox_control.size(); i++)
    {
      if(t_rw==0)
        {
          t_rw=1;
          if(!m.input.empty())
            {
              if(m.input.back()==1)
                Checkbox_control[i].OnMessage(m.x,m.y,1);
              Checkbox_control[i].OnMessage(m.x,m.y,0);
            }
          else
            {
              Checkbox_control[i].OnMessage(m.x,m.y,0);
            }
          t_rw=0;
        }
    }

}


void draw_UI(bool MSG)//渲染界面
{
  if(Li_win_wh())//检查窗口高度变化
    {
      Button_control[0].left=X-30;
      Button_control[0].right=X;
    }
  int oldlinecolor = getlinecolor();
  int oldbkcolor = getbkcolor();
  int oldfillcolor = getfillcolor();

  setlinecolor(RGB(52,58,65));
  setlinestyle(PS_SOLID,1);
  line(0,26,X,26);
  line(0,50,X,50);
  line(0,Y-17,X,Y-17);

  setfillcolor(RGB(27,35,44));//上下边框
  solidrectangle(0,0,X,25);
  solidrectangle(0,Y-16,X,Y);

  setfillcolor(RGB(33,43,53));//按钮栏
  solidrectangle(0,27,X,49);
  solidrectangle(0,51, 45, Y-18);//侧边 左
  setfillcolor(RGB(29,37,46));
  solidrectangle(X,51,X-250, Y-18);//侧边 右
  putimage(X-250,51,&UI_element);


  setlinestyle(PS_SOLID,1);
  setlinecolor(oldlinecolor);
  setbkcolor(oldbkcolor);
  setfillcolor(oldfillcolor);
  putimage(0,0,&LOGO);
  if(MSG)//是否启动鼠标消息
    {
      get_msg(hwnd,er_msg);
    }
  Button_msg(er_msg);//按钮消息
}

void NO_USE_UI(HWND hwnd)//焦点是否在ER 并通过渲染拖动窗口
{
  if ((GetForegroundWindow() == hwnd))
    {
      if (GetAsyncKeyState(1) && 0x8000) // 左键按下
        {
          if(( 690 <= msx && msx <= X && 0 <= msy && msy <= 25)||(0 <= msx && msx <= 89 && 0 <= msy && msy <= 25))
            {
              POINT cursorPos;
              int mouseX=0;
              int mouseY=0;
              while((GetAsyncKeyState(VK_LBUTTON) && 0x8000))
                {
                  GetCursorPos(&cursorPos);
                  mouseX=cursorPos.x;
                  mouseY=cursorPos.y;
                  if(mouseY<25)
                    MoveWindow(hwnd,0,0,dx,dy,0);
                  else if(mouseX<10)
                    MoveWindow(hwnd,0,0,dx/2,dy,0);
                  else if(mouseX>dx-10)
                    MoveWindow(hwnd,dx-(dx/2),0,dx/2,dy,0);
                  else
                    MoveWindow(hwnd,mouseX-msx,mouseY-msy,889,500,0);

                  draw_UI(0);//渲染界面
                  FlushBatchDraw();
                }
            }
        }
    }
}
int FPS=0;
//计算渲染帧数的函数
void CalculateFrameRate()
{
  // 使用静态变量保存上一帧时间和帧数
  static int frameCount = 0;
  static double lastTime = 0;
  // 获取当前时间
  double currentTime = GetTickCount() / 1000.0;
  // 如果是第一帧,记录起始时间
  if (lastTime == 0)
    {
      lastTime = currentTime;
    }
  // 增加帧数
  frameCount++;
  // 如果时间间隔大于1秒
  if (currentTime - lastTime >= 1.0)
    {
      // 计算帧率
      double frameRate = frameCount / (currentTime - lastTime);
      // 显示帧率
      FPS=(int)frameRate;
      // 重置帧数和时间
      frameCount = 0;
      lastTime = currentTime;
    }
}
