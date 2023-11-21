#include <vector>
#include <iostream>
#include "jsoncpp.cpp" // JSON库
#include <fstream>
#include <time.h>
#include <string>
#include "Files.h"
#include <Windows.h>

class face_data
{
  public:
  	string draw_data="";//数字内容
    string type="123";
    int x=0;
    int y=0;
    int w=0;//宽
    int h=0;//高
    vector<string> image;
    int showCount=0;
    int align=1;
    int spacing=0;
    int maxValue=0;//指针-最大取值
    int allAngle=0;//指针-推测是最大旋转角度*10  allangle/10/maxvalue=每进1旋转的角度
    int imageRotateX=0;//指针-指针围绕坐标
    int imageRotateY=0;//指针-指针围绕坐标
    int animInterval=0;//动画间隔 单位为ms
    int animRepeat=0;//指定动画重复次数 0为无限
    string jumpName="?";//跳转名字
    string jumpCode="?";//跳转代码
    string showZero="false";//false
    string dataSrc="?";
    int R=0;//随机数 用于显示！！！
    int R_time=0;//计时数据 用于显示！！！
    double Xuan_Angle=-1;//指针的旋转角度 用于显示！！！
    int anim_index=0;//动画下标 用于显示！！！
    bool dataSrc_choose=0;//是否正在选择数据源
    bool lock=0;//是否锁定
    int error_warn=0;//是否报错 0 为无 1为error 2为warn
    vector<string> imageList;
    vector<int> imageIndexList;//百分比
    vector<IMAGE> image_data;//图片数据
    bool attach=0;//是否存在附加图片

    face_data(const face_data& rhs)
    {
      type = rhs.type;
      x = rhs.x;
      y = rhs.y;
      w = rhs.w;
      h = rhs.h;
      image = rhs.image;
      showCount = rhs.showCount;
      align = rhs.align;
      spacing = rhs.spacing;
      maxValue = rhs.maxValue;
      allAngle = rhs.allAngle;
      imageRotateX = rhs.imageRotateX;
      imageRotateY = rhs.imageRotateY;
      animInterval = rhs.animInterval;
      animRepeat = rhs.animRepeat;
      jumpName = rhs.jumpName;
      jumpCode = rhs.jumpCode;
      showZero = rhs.showZero;
      dataSrc = rhs.dataSrc;
      R = rhs.R;
      R_time = rhs.R_time;
      Xuan_Angle = rhs.Xuan_Angle;
      anim_index = rhs.anim_index;
      dataSrc_choose = rhs.dataSrc_choose;
      imageList = rhs.imageList;
      imageIndexList = rhs.imageIndexList;
      error_warn = rhs.error_warn;
      lock = rhs.lock;
      attach = rhs.attach;
    }

    face_data()
    {
      //image_data = vector<IMAGE>(); // 初始化为空vector
    }
};

class auxiliary_data_class
{
  public:
    int x1=0;
    int y1=0;
    int x2=0;
    int y2=0;
    int N;
};

bool fd_ch=0;//0正常表盘 1AOD表盘
int font_b=25;//字体大小
bool R_C=1;//启动跳动预览
bool biger_ALL=0;//全屏是否覆盖任务栏
string face_result="";//表盘地址
bool biger_windows=0;//是否全屏
int examine_face_T=0;//是否启动检查
bool reload=0;//重新加载标志
string face_name="???";
ExMessage msg;

vector<face_data> fd;//显示
vector<face_data> fd_;//正常表盘      | 保存
vector<face_data> fd_AOD;//熄屏幕显示 |
vector<face_data> fd_cz;//临时保存
int time_save=0;//保存计时
string Bei_img2="data//PNG//背景.png";//背景地址
int Bei_n=0;//背景下标
bool save_png_draw=0;//保存预览图标志
int fd_N=0;//要高亮的控件
int C_N=0;//组件管理器的页面 1:快速选择跳转
bool welcome_UI_END=0;//欢迎界面结束
bool cmd_show=0;//是否显示控制台
float big_draw=1.0;
vector<auxiliary_data_class>auxiliary_data;//用于记录辅助显示数据
vector<int>auxiliary_n;//用于记录辅助显示的控件

int FW,FH;//表盘全局大小
int fw,fh;//表盘渲染时的大小 w=x h=y
int fx,fy;//表盘渲染时的位置
HDC mainDC;//窗口DC
HDC imgDC;//渲染DC
int mx=0,my=0;//光标相对于可编辑区的位置
short wheel=0;//鼠标滚动值

string dataSRC[41] = {"0811","0911","0A11","1011","1111","1211","1811","1911","1A11","0812","1012","1812","1912","1A12","2012","0821","0822","1022","0823","0851","1051","0824","0826","0828","0841","2031","3031","5031","2831","1013","3041","2041","1841","1021","1023","3012","3812","1832","2032","7831","4831"};
string SRC_str[41] = {"小时","小时(个)","小时(十)","分钟","分钟(个)","分钟(十)","秒","秒(个)","秒(十)","日期-年","日期-月","日期-日","日期-日(个)","日期-日(十)","星期","步数","心率","心率值区间","卡路里","距离-米","距离-千米","活动","压力","睡眠","电量","天气-温度","天气-类型","天气-空气","天气-湿度","上午下午","锁定","蓝牙","勿扰","步数-百分比","卡路里-百分比","农历 月","农历 日","当日最高气温","当日最低气温","紫外线","风力"};

string _jumpNameSTR[] = {"天气状况","步数","血氧","卡路里","站立","运动强度","睡眠","气压","日出时间","日落时间","湿度","紫外线指数","空气质量","闹钟","倒计时","秒表","音乐控制","拍照","微信支付","卡包","心率","电量","小爱同学"};
string _jumpName[] = {"weatherCondition","step","oxygen","calorie","stand","intensity","sleep","pressure","sunRise","sunSet","humidity","UVI","AQI","alarm","countdown","stopwatch","musicController","photograph","wechatPayment","cardBag","heartRate","batteryLevel","AIVS"};
string _jumpCode[] = {"030B1F02","03023F01","0307EF00","03033F01","03043F01","03053F01","0300CF00","03083F00","050B1F02","050B1F02","030B1F02","030B1F02","030B1F02","010C8F02","010C6F02","010C0F02","010C5F01","010CCF01","010D5F02","010ABF02","03065F00","03090C00","010CDF02"};

string dataSRC_out(string data,string find)//数据源对照表 find:查找类型  str返回中文  src返回数据源id  ,find_i 在表上调整指针位置对应的序号
{
  if(find=="str")
    {
      for(int i=0; i<42; i++)
        {
          if(dataSRC[i]==data)
            return SRC_str[i];
        }
    }
  if(find=="scr")
    {
      for(int i=0; i<42; i++)
        {
          if(SRC_str[i]==data)
            return dataSRC[i];
        }
    }
  return "?";
}

string dataSRC_out_2(string data,int n)//返回指定数据源 +多少位
{
  for(int i=0; i<42; i++)
    {
      if(dataSRC[i]==data)
        {
          if(i+n>41)
            i=0;
          if(i+n<0)
            i=41;
          return dataSRC[i+n];
        }
    }
  return "";
}

void parseFile(const string& filepath, vector<face_data>& faceDataList)//读取
{

  Json::Value root;
  Json::Reader reader;

  ifstream infile(filepath);
  if (!infile)
    {
      cerr << "Error opening file: " << filepath << endl;
      return;
    }

  if (!reader.parse(infile, root))
    {
      cerr << "Error parsing json" << endl;
      return;
    }
  face_name=root["name"].asString();

  auto elements = root["elementsNormal"];
  for(auto element : elements)
    {

      face_data data;

      data.type = element["type"].asString();
      if (element.isMember("x"))
        {
          data.x = element["x"].asInt();
        }
      if (element.isMember("y"))
        {
          data.y = element["y"].asInt();
        }
      if (element.isMember("showCount"))
        {
          data.showCount = element["showCount"].asInt();
        }
      if (element.isMember("spacing"))
        {
          data.spacing = element["spacing"].asInt();
        }
      if (element.isMember("dataSrc"))
        {
          data.dataSrc = element["dataSrc"].asString();
        }
      if (element.isMember("align"))
        {
          data.align = element["align"].asInt();
        }
      if (element.isMember("showZero"))
        {
          data.showZero = element["showZero"].asString();
        }
      if (element.isMember("maxValue"))
        {
          data.maxValue = element["maxValue"].asInt();
        }
      if (element.isMember("allAngle"))
        {
          data.allAngle = element["allAngle"].asInt();
        }
      if (element.isMember("imageRotateX"))
        {
          data.imageRotateX = element["imageRotateX"].asInt();
        }
      if (element.isMember("imageRotateY"))
        {
          data.imageRotateY = element["imageRotateY"].asInt();
        }
      if (element.isMember("animInterval"))
        {
          data.animInterval = element["animInterval"].asInt();
        }
      if (element.isMember("animRepeat"))
        {
          data.animRepeat = element["animRepeat"].asInt();
        }
      if (element.isMember("jumpName"))
        {
          data.jumpName = element["jumpName"].asString();
        }
      if (element.isMember("jumpCode"))
        {
          data.jumpCode = element["jumpCode"].asString();
        }

      //...解析其他字段

      // 解析image字段
      if (element.isMember("image"))
        {
          data.image.push_back(element["image"].asString());
          if(data.type=="widge_dignum")
            {
              cout<<"附加图片\n";
              data.attach=1;
            }
        }

      // 解析imageList字段
      if (element.isMember("imageList"))
        {
          auto imageList = element["imageList"];
          for(auto img : imageList)
            {
              data.imageList.push_back(img.asString());
            }
        }
      if (element.isMember("imageIndexList"))
        {
          auto imageIndexList = element["imageIndexList"];
          for(auto img : imageIndexList)
            {
              data.imageIndexList.push_back(img.asInt());
            }
        }

      faceDataList.push_back(data);
    }

}

void parseFile_AOD(const string& filepath, vector<face_data>& faceDataList)//读取！！！AOD
{

  Json::Value root;
  Json::Reader reader;

  ifstream infile(filepath);
  if (!infile)
    {
      cerr << "Error opening file: " << filepath << endl;
      return;
    }

  if (!reader.parse(infile, root))
    {
      cerr << "Error parsing json" << endl;
      return;
    }

  auto elements = root["elementsAod"];
  for(auto element : elements)
    {

      face_data data;

      data.type = element["type"].asString();
      if (element.isMember("x"))
        {
          data.x = element["x"].asInt();
        }
      if (element.isMember("y"))
        {
          data.y = element["y"].asInt();
        }
      if (element.isMember("showCount"))
        {
          data.showCount = element["showCount"].asInt();
        }
      if (element.isMember("spacing"))
        {
          data.spacing = element["spacing"].asInt();
        }
      if (element.isMember("dataSrc"))
        {
          data.dataSrc = element["dataSrc"].asString();
        }
      if (element.isMember("align"))
        {
          data.align = element["align"].asInt();
        }
      if (element.isMember("showZero"))
        {
          data.showZero = element["showZero"].asString();
        }
      if (element.isMember("maxValue"))
        {
          data.maxValue = element["maxValue"].asInt();
        }
      if (element.isMember("allAngle"))
        {
          data.allAngle = element["allAngle"].asInt();
        }
      if (element.isMember("imageRotateX"))
        {
          data.imageRotateX = element["imageRotateX"].asInt();
        }
      if (element.isMember("imageRotateY"))
        {
          data.imageRotateY = element["imageRotateY"].asInt();
        }
      if (element.isMember("animInterval"))
        {
          data.animInterval = element["animInterval"].asInt();
        }
      if (element.isMember("animRepeat"))
        {
          data.animRepeat = element["animRepeat"].asInt();
        }

      //...解析其他字段

      // 解析image字段
      if (element.isMember("image"))
        {
          data.image.push_back(element["image"].asString());
          if(data.type=="widge_dignum")
            {
              cout<<"附加图片\n";
              data.attach=1;
            }
        }

      // 解析imageList字段
      if (element.isMember("imageList"))
        {
          auto imageList = element["imageList"];
          for(auto img : imageList)
            {
              data.imageList.push_back(img.asString());
            }
        }
      if (element.isMember("imageIndexList"))
        {
          auto imageIndexList = element["imageIndexList"];
          for(auto img : imageIndexList)
            {
              data.imageIndexList.push_back(img.asInt());
            }
        }

      faceDataList.push_back(data);
    }

}
void serializeToJson(const vector<face_data>& faceDataList,const vector<face_data>& faceDataList_aod, const string& filepath)//保存
{
  Json::Value root;
  Json::Value elementsNormal(Json::arrayValue);
  Json::Value elementsAod(Json::arrayValue);

  root["name"] = face_name;
  root["id"] = "267210032";
  root["previewImg"] = "preview";

  for (const auto& data : faceDataList)
    {
      Json::Value element;

      element["type"] = data.type;

      if (data.type == "element")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          if(data.jumpCode!="?"&&data.jumpName!="?")
            {
              element["jumpName"] = data.jumpName;
              element["jumpCode"] = data.jumpCode;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="element_anim")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["animInterval"] = data.animInterval;
          element["animRepeat"] = data.animRepeat;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
        }
      if(data.type =="widge_imagelist")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.imageIndexList.empty())
            {
              Json::Value imageIndexList(Json::arrayValue);
              for (const auto& img : data.imageIndexList)
                {
                  imageIndexList.append(img);
                }
              element["imageIndexList"] = imageIndexList;
            }
        }
      if(data.type =="widge_dignum")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["showCount"] = data.showCount;
          element["spacing"] = data.spacing;
          element["dataSrc"] = data.dataSrc;
          element["align"] = data.align;
          if(data.showZero=="true")
            element["showZero"] = true;
          if(data.showZero=="false")
            element["showZero"] = false;
          // Serialize imageList field
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="widge_pointer")//指针
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          element["maxValue"] = data.maxValue;
          element["allAngle"] = data.allAngle;
          element["imageRotateX"] = data.imageRotateX;
          element["imageRotateY"] = data.imageRotateY;
          if (!data.image.empty())
            {
              element["image"] = data.image[0];
            }
        }

      elementsNormal.append(element);
    }

  root["elementsNormal"] = elementsNormal;

  for (const auto& data : faceDataList_aod)
    {
      Json::Value element;

      element["type"] = data.type;

      if (data.type == "element")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="element_anim")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["animInterval"] = data.animInterval;
          element["animRepeat"] = data.animRepeat;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
        }
      if(data.type =="widge_imagelist")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.imageIndexList.empty())
            {
              Json::Value imageIndexList(Json::arrayValue);
              for (const auto& img : data.imageIndexList)
                {
                  imageIndexList.append(img);
                }
              element["imageIndexList"] = imageIndexList;
            }
        }
      if(data.type =="widge_dignum")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["showCount"] = data.showCount;
          element["spacing"] = data.spacing;
          element["dataSrc"] = data.dataSrc;
          element["align"] = data.align;
          if(data.showZero=="true")
            element["showZero"] = true;
          if(data.showZero=="false")
            element["showZero"] = false;
          // Serialize imageList field
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="widge_pointer")//指针
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          element["maxValue"] = data.maxValue;
          element["allAngle"] = data.allAngle;
          element["imageRotateX"] = data.imageRotateX;
          element["imageRotateY"] = data.imageRotateY;
          if (!data.image.empty())
            {
              element["image"] = data.image[0];
            }
        }

      elementsAod.append(element);
    }

  root["elementsAod"] = elementsAod;


  Json::StyledWriter writer;
  std::ofstream outfile(filepath);
  outfile << writer.write(root);
}

bool moveElement(std::vector<face_data>& vec, int index, int n)//移动控件图层
{
  int size = vec.size();
  if((index+n)<-1)
    return 1;
  if((index+n)>size)
    return 1;

  if (n > 0)
    {
      if (index - n < 0)
        {
          // 向前移动会越界,不做任何操作
          return 1;
        }
      // 向前移动
      face_data temp = vec[index];
      for (int i = index; i > index - n; i--)
        {
          vec[i] = vec[i-1];
        }
      vec[index - n] = temp;

    }
  else if (n < 0)
    {
      if (index - n >= size)
        {
          // 向后移动会越界,不做任何操作
          return 1;
        }
      // 向后移动
      face_data temp = vec[index];
      for (int i = index; i < index - n; i++)
        {
          vec[i] = vec[i+1];
        }
      vec[index - n] = temp;
    }
  return 0;
}

string replaceSlash(const string& str)//替换  \\->//
{
  string result = str;

  for(size_t i = 0; i < result.length(); i++)
    {
      if (result[i] == '\\')
        {
          result.replace(i, 1, "//");
          //i++; // 加2是因为增加了两个字符
        }
    }
  string s2="//wfDef.json";
  while (result.find(s2) < result.length())
    {
      result.erase(result.find(s2), s2.size());
    }
  return result;
}

int getRand(int min, int max)//生成随机数
{
  return ( rand() % (max - min + 1) ) + min ;
}

void image_wh(const string& filepath,vector<face_data>& faceDataList,bool ch)//计算控件长宽
{
  for(int i=0; i<faceDataList.size(); i++)
    {
      if(faceDataList[i].type=="element")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].image[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_imagelist"||faceDataList[i].type=="element_anim")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].imageList[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].imageList[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_dignum")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].imageList[faceDataList[i].anim_index]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].imageList[faceDataList[i].anim_index]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = (img.getwidth()*faceDataList[i].showCount)+(faceDataList[i].showCount*faceDataList[i].spacing);
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_pointer")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].image[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
    }
}

void reloadImageData(vector<face_data>& faceDataList, const string& filepath)
{

  for (face_data& fdata : faceDataList)
    {

      fdata.image_data.clear();

      if (fdata.type == "element")
        {
          // 加载单张图片
          IMAGE img;
          string data;
          if(fd_ch==0)
            data=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
          data=replaceSlash(data);

          loadimage(&img, data.c_str());
          fdata.image_data.push_back(img);

        }
      else if (fdata.type == "widge_imagelist")
        {

          // 加载多张图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }

        }
      else if (fdata.type == "widge_dignum")
        {
          // 加载多张图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }
          if(fdata.attach)//附加图片加载
            {
              IMAGE img2;
              string data2;
              if(fd_ch==0)
                data2=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
              else
                data2=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
              data2=replaceSlash(data2);

              loadimage(&img2, data2.c_str());
              fdata.image_data.push_back(img2);
            }
        }
      else if (fdata.type == "widge_pointer")
        {

          // 加载单张图片
          IMAGE img;
          string imgPath;
          if(fd_ch==0)
            imgPath=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
          else
            imgPath=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
          imgPath=replaceSlash(imgPath);
          loadimage(&img, imgPath.c_str());
          fdata.image_data.push_back(img);

        }
      else if (fdata.type == "element_anim")
        {

          // 加载多张动画图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }

        }

    }

}



IMAGE draw_face(const string& filepath, vector<face_data>& faceDataList)
{

  // 先重新加载所有图片


  IMAGE result(336, 480);

  for (auto& data : faceDataList)
    {
      if (!data.image_data.empty())
        {
          if(R_C==0)
            data.R=0;
          // 访问data.image_data

          if (data.type == "element")
            {
              transparentimage3(&result, data.x, data.y, &data.image_data[0]);
            }
          else if (data.type == "widge_imagelist")
            {
              if(data.R_time > 500)
                {
                  if(data.R<data.image_data.size()-1)
                    data.R++;
                  else
                    data.R=0;
                  data.R_time = 0;
                }
              else
                {
                  // data.R_time还没大于5,直接增加
                  data.R_time++;
                }
              if(R_C==0)
                data.R=0;
              transparentimage3(&result, data.x, data.y, &data.image_data[data.R]);
            }
          else if (data.type == "widge_dignum")
            {
              if(data.type=="widge_dignum")//数字
                {
                  if(data.R_time > 100)
                    {
                      if(!data.attach)
                        {
                          if(data.R<data.image_data.size()-1)
                            data.R++;
                          else
                            data.R=0;
                          data.R_time = 0;
                        }
                      if(data.attach)
                        {
                          if(data.R<data.image_data.size()-2)
                            data.R++;
                          else
                            data.R=0;
                          data.R_time = 0;
                        }
                    }
                  else
                    {
                      // data.R_time还没大于5,直接增加
                      data.R_time++;
                    }
                  if(R_C==0)
                    data.R=0;
                  bool DEBOU=0;//小数点Rain.Cl专属部分
                  int showCount=data.showCount;
                  if(data.showCount==20)
                    {
                      DEBOU=1;
                      showCount=3;//睡眠小数点Rain.Cl专属部分
                    }
                  for(int j=0; j<showCount; j++)
                    {
                      int mun=(data.imageList.size());
                      int R=data.R;
                      if(getRand(0,40)==5)
                        {
                          if(data.attach)
                            R=getRand(0,mun-2);
                          if(!data.attach)
                            R=getRand(0,mun-1);
                        }
                      if(j==0)
                        {
                          if(data.draw_data!="")
                            data.R=data.draw_data[0]- '0';
                          if(data.align==1)
                            {
                              transparentimage3(&result,data.x, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==0)
                            {
                              transparentimage3(&result,data.x-data.w, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==2)
                            {
                              transparentimage3(&result,data.x-(data.w/2), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                            {
                              transparentimage3(&result,data.x-(data.w/20), data.y,&data.image_data[data.R]);
                            }
                        }
                      else
                        {
                          if(data.draw_data!="")
                            data.R=data.draw_data[j]- '0';
                          if(data.align==1)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+data.spacing*j, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==0)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+(data.spacing*j-data.w), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==2)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+(data.spacing*j-(data.w/2)), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                            {
                              transparentimage3(&result,data.x-data.image_data[data.R].getwidth()*j+data.spacing*j-(data.w/20), data.y,&data.image_data[data.R]);
                            }
                        }
                      if((j+1)==showCount)//附加图片
                        {
                          if(data.attach)
                            {
                              if(data.align==1)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+data.spacing*j + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==0&&DEBOU==0)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+(data.spacing*j-data.w) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==2)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+(data.spacing*j-(data.w/2)) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                                transparentimage3(&result,data.x - data.image_data[data.R].getwidth()*j+data.spacing*j-(data.w/20) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());

                            }
                        }
                    }
                }
            }
          else if (data.type == "widge_pointer")
            {
              transparentimage3(&result, data.x, data.y, &data.image_data[0]);
              int oldlinecolor = getlinecolor();
              SetWorkingImage(&result);//渲染光标
              setlinecolor(RGB(243,102,56));
              line(data.x+data.imageRotateX-6, data.y+data.imageRotateY-6,data.x+data.imageRotateX+6, data.y+data.imageRotateY+6);
              line(data.x+data.imageRotateX+6, data.y+data.imageRotateY-6,data.x+data.imageRotateX-6, data.y+data.imageRotateY+6);
              SetWorkingImage(NULL);//渲染光标
              setlinecolor(oldlinecolor);
            }
          else if (data.type == "element_anim")
            {
              if(data.R_time > 50)
                {
                  if(data.R<data.image_data.size()-1)
                    data.R++;
                  else
                    data.R=0;
                  data.R_time = 0;
                }
              else
                {
                  // data.R_time还没大于5,直接增加
                  data.R_time++;
                }
              if(R_C==0)
                data.R=0;
              transparentimage3(&result, data.x, data.y, &data.image_data[data.anim_index]);
            }
        }
    }

  return result;

}

void cl_msg()
{
  flushmessage();
  msg.x=300;
  msg.y=100;
  msg.message=0;
}

string choose_image_data;
void choose_image(int N,vector<face_data>& data)//修改控件 加载图片
{
  choose_image_data=chooseFiles();
  if(choose_image_data=="")
    {
      return ;
    }
  string s2=".png";
  while (choose_image_data.find(s2) < choose_image_data.length())
    {
      choose_image_data.erase(choose_image_data.find(s2), s2.size());
    }
  vector<string> result = splitPath(choose_image_data);

  cout<<endl<<choose_image_data<<endl;

  choose_image_data="";
  cout<<endl<<"拆分:"<<endl;
  for(int i=0; i<result.size(); i++)
    {
      cout<<result[i]<<endl;
    }

  if(data[N].type=="element")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      data[N].image[0]=result[0];
      string result_data;
      if(fd_ch==0)
        result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";
      else
        result_data=face_result+"\\"+"images_aod"+"\\"+data[N].image[0]+".png";
      //string result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";//重新计算大小
      result_data=replaceSlash(result_data);
      IMAGE img;
      loadimage(&img,result_data.c_str());
      data[N].w = img.getwidth();
      data[N].h = img.getheight();
    }
  if(data[N].type=="widge_pointer")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      data[N].image[0]=result[0];

      string result_data;
      if(fd_ch==0)
        result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";
      else
        result_data=face_result+"\\"+"images_aod"+"\\"+data[N].image[0]+".png";

      // string result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";//重新计算大小
      result_data=replaceSlash(result_data);
      IMAGE img;
      loadimage(&img,result_data.c_str());
      data[N].w = img.getwidth();
      data[N].h = img.getheight();
    }
  if(data[N].type=="widge_imagelist"||data[N].type=="widge_dignum"||data[N].type=="element_anim")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      if(data[N].imageList.size()==result.size())
        {
          cout<<"\n数量一样";
          for(int i=0; i<data[N].imageList.size(); i++)
            {

              data[N].imageList[i]=result[i];
            }
          if(data[N].type=="widge_imagelist"||data[N].type=="element_anim")
            {
              string result_data;
              if(fd_ch==0)
                result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
              else
                result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
              // string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
              result_data=replaceSlash(result_data);
              IMAGE img;
              loadimage(&img,result_data.c_str());
              data[N].w = img.getwidth();
              data[N].h = img.getheight();
            }
          if(data[N].type=="widge_dignum")//指针
            {
              string result_data;
              if(fd_ch==0)
                result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
              else
                result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
//             / string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
              result_data=replaceSlash(result_data);
              IMAGE img;
              loadimage(&img,result_data.c_str());
              data[N].w = (img.getwidth()*data[N].showCount)+(data[N].showCount*data[N].spacing);
              data[N].h = img.getheight();
            }
        }
      if(data[N].imageList.size()!=result.size())
        {
          cout<<"\n数量不一样";
          data[N].imageList.clear();

          for(int i=0; i<result.size(); i++)
            {
              data[N].imageList.push_back(result[i]);
            }
        }
      if(data[N].type=="widge_imagelist"||data[N].type=="element_anim")
        {
          string result_data;
          if(fd_ch==0)
            result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
          else
            result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
          //string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
          result_data=replaceSlash(result_data);
          IMAGE img;
          loadimage(&img,result_data.c_str());
          data[N].w = img.getwidth();
          data[N].h = img.getheight();
        }
      if(data[N].type=="widge_dignum")
        {
          string result_data;
          if(fd_ch==0)
            result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
          else
            result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
          //string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
          result_data=replaceSlash(result_data);
          IMAGE img;
          loadimage(&img,result_data.c_str());
          data[N].w = (img.getwidth()*data[N].showCount)+(data[N].showCount*data[N].spacing);
          data[N].h = img.getheight();

        }

    }
  flushmessage();
  reloadImageData(data, face_result);
  cl_msg();
}

void On_btnOk_Click()   //打开
{
  welcome_UI_END=1;
  fd_N=0;
  fd.clear();
  fd_.clear();
  fd_AOD.clear();
  fd_cz.clear();
  auxiliary_n.clear();
  face_result=chooseFiles();

  string s2="|";
  while (face_result.find(s2) < face_result.length())
    {
      face_result.erase(face_result.find(s2), s2.size());
    }

  parseFile(face_result,fd_);
  image_wh(face_result,fd_,0);

  parseFile_AOD(face_result,fd_AOD);
  image_wh(face_result,fd_AOD,1);

  fd=fd_;

  cout<<"========================\n";
  time_save=0;
  reloadImageData(fd, face_result);
  fd_cz=fd;
  if(fd.size()>0)
    {
      fd_N=1;
    }
}

void draw_fd_N(ExMessage msg,IMAGE *face_image,int i)   //高亮渲染
{
  if(save_png_draw)
    {
      cout<<"保存预览图\n";
      save_png_draw=0;
      string s2="wfDef.json";
      string re=face_result+"preview.png";
      while (re.find(s2) < re.length())
        {
          re.erase(re.find(s2), s2.size());
        }
      cout<<"预览图路径:"<<re<<endl;
      IMAGE img;
      saveimage(_T(re.c_str()),face_image);
      loadimage(&img, _T(re.c_str()),230,328,true);
      saveimage(_T(re.c_str()),&img);
    }
  int oldlinecolor_ = getlinecolor();
  setlinecolor(RGB(243,102,56));
  line(0,26,X,26);
  setlinecolor(oldlinecolor_);
  if(i==0)
    {

      return ;
    }
  i=i-1;
  // cout<<"控件坐标:"<<"X"<<fd[i].x<<"Y"<<fd[i].y<<endl;
  SetWorkingImage(face_image);//渲染光标
  setlinecolor(RGB(243,102,56));

  if(fd[i].align==1)//左对齐
    rectangle(fd[i].x,fd[i].y,(fd[i].w+fd[i].x)-1,(fd[i].h+fd[i].y)-1);

  if(fd[i].align==0)//右对齐
    rectangle(fd[i].x-fd[i].w,fd[i].y,(fd[i].x)-1,(fd[i].h+fd[i].y)-1);

  if(fd[i].align==2)//中对齐
    rectangle(fd[i].x-(fd[i].w/2),fd[i].y,(fd[i].x+(fd[i].w/2))-1,(fd[i].h+fd[i].y)-1);

  setlinecolor(WHITE);
  SetWorkingImage(NULL);
}

void info(ExMessage &msg,IMAGE face_image,ER_MSG &m)
{
  // 计算鼠标位置相对于图片在屏幕上的左上角的偏移
  int dx = msg.x - fx;
  int dy = msg.y - fy;
  // 将偏移量映射到原图片大小
  float sx = (float)dx / fw * 336;
  float sy = (float)dy / fh * 480;
  mx = (int)sx;
  my = (int)sy;

  msg.x=msx;
  msg.y=msy;
  if (m.input.back()==1&&!fd.empty())   //拖动控件+选中控件
    {
      int time;
      while(GetAsyncKeyState(1) && 0x8000&&fd[fd_N-1].lock==0)
        {
          time++;
          Sleep(10);
          if(time>5)
            {
              int mxo=mx,myo=my;
              if(fd_N==0)
                {
                  return ;
                }
              while(GetAsyncKeyState(VK_LBUTTON) < 0)
                {
                  int _x=msg.x,_y=msg.y;
                  GetMousePosInClient( hwnd,_x,_y);
                  msg.x=_x,msg.y=_y;
                  // 计算鼠标位置相对于图片在屏幕上的左上角的偏移
                  int dx = msg.x - fx;
                  int dy = msg.y - fy;
                  // 将偏移量映射到原图片大小
                  float sx = (float)dx / fw * 336;
                  float sy = (float)dy / fh * 480;
                  mx = (int)sx;
                  my = (int)sy;
                  if(mxo!=mx||myo!=my)
                    {
                      fd[fd_N-1].x=fd[fd_N-1].x+(mx-mxo);
                      fd[fd_N-1].y=fd[fd_N-1].y+(my-myo);
                      mxo=mx,myo=my;
                      IMAGE face_image2=draw_face(face_result,fd);//读取虚拟编辑区
                      draw_fd_N(msg,&face_image2,fd_N);
                      imgDC = GetImageHDC(&face_image2);//获取DC
                      StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
                      draw_UI(1);//渲染UI);
                      FlushBatchDraw();
                    }
                }
            }
        }
      //cout<<"x"<<msg.x<<" y"<<msg.y<<" fx"<<fx<<" fy"<<fy<<" fw"<<fw<<" fh"<<fh<<" "<<(msg.y >= fy)<<endl;
      if(msg.x >= fx && msg.x <= fx+fw && msg.y >= fy && msg.y <= fy+fh)
        {
          //cout<<"点击在表盘上!!\n";
          for(int i=fd.size()-1; i>-1; i--)
            {
              if (i < 0) i = 0; // 防止变成负数

              fd[i].dataSrc_choose=0;
              if(fd[i].x <= mx && mx <= (fd[i].w+fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==1)   //左对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
              if(fd[i].x-fd[i].w <= mx && mx <= (fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==0)   //右对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
              if(fd[i].x-(fd[i].w/2) <= mx && mx <= ((fd[i].w/2)+fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==2)   //中对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
            }
        }

    }
  if (m.input.back()==2)   //移动编辑区
    {
      int relX = msg.x - fx;
      int relY = msg.y - fy;
      while(GetAsyncKeyState(VK_RBUTTON) < 0)
        {
          int _x=msg.x,_y=msg.y;
          GetMousePosInClient( hwnd,_x,_y);
          msg.x=_x,msg.y=_y;
          int offsetX = msg.x - relX;
          int offsetY = msg.y - relY;
          fx = offsetX;
          fy = offsetY;
          int oldfillcolor = getfillcolor();
          setfillcolor(RGB(11,22,29));//工作区
          solidrectangle(0,51,X,Y-18);
          setfillcolor(oldfillcolor);
          StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          draw_UI(1);
          FlushBatchDraw();
        }
      flushmessage();
    }
  if(m.input.back()==17&&GetAsyncKeyState(17) && 0x8000)   //缩放
    {
      flushmessage();
      while(GetAsyncKeyState(17) && 0x8000)
        {
          msg = getmessage(EM_MOUSE|EM_KEY);			// 获取消息输入
          if(msg.wheel > 0)
            {
              msg.wheel=0;
              big_draw= big_draw+0.05;
              fw=FW* big_draw, fh=FH* big_draw;
            }
          else if(msg.wheel < 0)
            {
              msg.wheel=0;
              big_draw= big_draw-0.05;
              fw=FW* big_draw,fh=FH* big_draw;
            }
          int oldfillcolor = getfillcolor();
          setfillcolor(RGB(11,22,29));//工作区
          solidrectangle(0,51,X,Y-18);
          setfillcolor(oldfillcolor);
          StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          draw_UI(1);
          FlushBatchDraw();
        }
      return ;
    }
}

void AOD_do()
{
  try
    {
      if(fd_ch==0)
        {
          fd_N=0;
          fd_.clear();
          for(int i=0; i<fd.size(); i++)
            fd_.push_back(fd.at(i));
          fd.clear();
          for(int i=0; i<fd_AOD.size(); i++)
            fd.push_back(fd_AOD.at(i));
          fd_N=0;
        }

      if(fd_ch==1)
        {
          fd_N=0;
          fd_AOD.clear();
          for(int i=0; i<fd.size(); i++)
            fd_AOD.push_back(fd[i]);
          fd.clear();
          for(int i=0; i<fd_.size(); i++)
            fd.push_back(fd_[i]);
          fd_N=0;
        }
      fd_ch=!fd_ch;
      reloadImageData(fd, face_result);
    }
  catch (const std::exception& e)
    {
      // 打印错误信息
      std::cerr << e.what() << '\n';
    }
  catch (...)
    {
      // 打印未知错误
      std::cerr << "Unknown exception!\n";
    }

}

void On_save()   //保存
{
  bool ch=0;
  if(fd_ch==1)
    {
      AOD_do();
      cout<<"切换 正常表盘\n";
      ch=1;
    }
  //fd_=fd;
  serializeToJson(fd,fd_AOD,face_result);
  MessageBox(NULL,"已保存表盘    但请注意注意！使用第三方表盘有一定风险！","保存",MB_OK|MB_ICONINFORMATION);
  if(ch)
    AOD_do(),cout<<"切换 AOD表盘\n";
}