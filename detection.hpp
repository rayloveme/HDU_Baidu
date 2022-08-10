#pragma once

#include "capture.hpp"
#include "predictor.hpp"
#include "preprocess.hpp"
#include "mat_util.hpp"
#include "stop_watch.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <exception>
#include "frame_save.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include "zsh_construction.h"
#include "lqp_GasStation.hpp"
extern Mat frame;
extern Mat frame_ROI;

extern int sancha_flag; //三叉标志位
extern int sancha_find_cnt;
extern int sancha_lost_cnt;

extern int is_sancha; //只是用来看看图片中是不是有三叉

// extern int zsh_construction_flag;     //施工区标志位
extern int shigongqu_find_cnt;
extern int shigongqu_lost_cnt;

extern int cheku_find_cnt;
extern int cheku_miss_cnt;
extern int cheku_flag;
extern int is_cheku;

extern uint start_key; //按键起止相关
extern uint end_key;
extern int start_end_statues;

using namespace std;
using namespace cv;

struct DetectionResult
{                                               //检测结果结构体
  cv::Mat det_render_frame;                     // 画了框的图
  cv::Mat rgb_frame;                            // 原图
  std::vector<PredictResult> predictor_results; // 预测结果
};

// 对检测模型的封装类，通过一个单独线程完成从摄像头读图，到模型推理

class Detection
{

public:
  Detection(bool logEn = false) : _log_en(logEn) {}
  ~Detection() {}
  // 初始化, 传入相机结点和模型路径
  int init(std::string model_config_path)
  { // std::string file_path,
    _is_file = true;
    // _file_path = file_path;
    // _capture = std::make_shared<Capture>();
    // if (_capture == nullptr) {
    //   std::cout << "Capture create failed." << std::endl;
    //   return -1;
    // }
    // int ret = _capture->open(file_path);
    // if (ret != 0) {
    //   std::cout << "Capture open failed." << std::endl;
    //   return -1;
    // }
    return _init(model_config_path);
  };

  int init(int camera_index, std::string model_config_path)
  {
    // _capture = std::make_shared<Capture>();
    // if (_capture == nullptr) {
    //   std::cout << "Capture create failed." << std::endl;
    //   return -1;
    // }
    // int ret = _capture->open(camera_index);
    // if (ret != 0) {
    //   std::cout << "Capture open failed." << std::endl;
    //   return -1;
    // }
    return _init(model_config_path);
  };

  // 设置opencv显示窗口
  // std::string rgb_window_name = "RGB";
  std::string gray_window_name = "Gray";
  int display_window_init()
  {
    // cv::namedWindow(rgb_window_name, WINDOW_NORMAL);
    // cv::resizeWindow(rgb_window_name, 640, 480);
    // cv::moveWindow(rgb_window_name, 0, 0);

    cv::namedWindow(gray_window_name, WINDOW_NORMAL);
    cv::moveWindow(gray_window_name, 800, 0);
    cv::resizeWindow(gray_window_name, 640, 480);
  }

  void start()
  { //开启线程
    _thread = std::make_unique<std::thread>([this]()
                                            {
      // clock_t start, finish;
      cv::Rect rect_select = cv::Rect(170, 180, 400, 300);    //用于截取特定区域 400*300
      cv::Mat frame_0, ROI, _element, _red_mask;

      display_window_init();                    //检测显示窗口初始化

      while (1) {
        // start = clock();  
        
        if(start_key == 0 && end_key == 0 || start_key == 0 && end_key == 1){       //如果检测到停止按键，检测线程阻塞
          start_end_statues = 0;
          wait_key();
        }

        std::shared_ptr<DetectionResult> result = std::make_shared<DetectionResult>();

        // StopWatch stop_watch_capture;
        // stop_watch_capture.tic();
        frame_0 = frame.clone();
        ROI = frame_0(rect_select);                     //截取图片
        // cv::cvtColor(ROI, ROI, cv::COLOR_RGB2HSV);

        // cv::inRange(ROI, Scalar(120, 100, 100), Scalar(140, 255, 255), _red_mask);       //颜色提取
        // _element = getStructuringElement(MORPH_RECT, Size(3, 3));
        // erode(_red_mask, _red_mask, _element, Point(-1, -1), 1, 0);
        // dilate(_red_mask, _red_mask, _element, Point(-1, -1), 1);

        {
          // std::unique_lock<std::mutex> lock(_mutex);
          // result->rgb_frame = frame;       //读主线程的图
          result->rgb_frame = ROI.clone();      //图片送入预测
        }           

        // double capture_times = stop_watch_capture.toc();

        if (result->rgb_frame.empty()) {                                              //图像为空
          std::cout << "Error: Capture Get Empty Error Frame." << std::endl;
          exit(-1);
        }
        result->predictor_results = _predictor->run(result->rgb_frame);     //这里执行图像预测
        result->det_render_frame = result->rgb_frame.clone();
        _predictor->render(result->det_render_frame, result->predictor_results);
        // if (1) {                                                            //输出条件如果是1会怎么样  _log_en
          // _predictor->printer(result->rgb_frame, result->predictor_results);      //结果格式化输出
        // }

        {
          std::unique_lock<std::mutex> lock(_mutex);                  //加锁，阻塞其他线程，将本线程锁起来，以免在运行本线程时其他线程也在运行
          _lastResult = result;
          cond_.notify_all();
        }

        cv::imshow(gray_window_name, _lastResult->det_render_frame);

        get_elements(_lastResult);



        // finish = clock();
        // cout <<endl<<"the time cost is:" << double(finish - start) / CLOCKS_PER_SEC<<endl;

      } });
  }
int number_x=0,number_y=0,number_name=0;
  void get_elements(std::shared_ptr<DetectionResult> result)
  {

    std::vector<PredictResult> results_datas = result->predictor_results; //从子线程获取推理结果
    // cout<<"size = "<<results_datas.size()<<endl;
    if (results_datas.size())
    { //如果返回的结果不为空
      for (int i = 0; i < results_datas.size(); i++)
      {
        cout << "type = " << results_datas[i].type << endl;
        cout << "center_x = " << results_datas[i].x + results_datas[i].width / 2 << endl;
        cout << "center_y = " << results_datas[i].y + results_datas[i].height / 2 << endl;

        int type = results_datas[i].type + 1;                    // yolo从第0类开始，这里要加1
        int x = results_datas[i].x + results_datas[i].width / 2; // x、y指的是检测得到的中心
        int y = results_datas[i].y + results_datas[i].height / 2;
        int width = results_datas[i].width; //检测框的宽度和长度
        int height = results_datas[i].height;
        float score = results_datas[i].score;

        cout << "width == " << width << endl;
        cout << "height == " << height << endl;
        cout << "score == " << score << endl;
        if (type == 1)
        {
          cout << "三叉" << endl;
          is_sancha = 1;
          if (x > 0 && x < 400 && y > 0 && width < 250 && height < 200 && sancha_flag == 0)
          { //有关三叉检测的防误判范围
            // sancha_flag = 1;
            cout << "检测到三叉" << endl;
            sancha_find_cnt++;
            // cv::imwrite("/root/workspace/pic/sancha.jpg", result->det_render_frame);
          }
        }

        if (type == 3)
        { //车库
        }

        // if(type == 5){                                  //坡道计数
        //   // cout<<"检测到坡道"<<endl;
        //   podao(x, y);
        // }

        if (type == 4)
        {
          if (x > 100 && x < 300 && y > 100 && width < 200 && height < 125 && zsh_construction_flag == CONSTRUCTION_NULL)
          { //有关施工区检测的防误判范围
            cout << "检测到施工区" << endl;
            shigongqu_find_cnt++;
          }
        }

        /*******************************************/
        if(type==6)
        {
          //加油站
        }
        if(type==7)
        {
          //数字1
        }
        if(type==8)
        {
          //数字2
        }
        /**********************************************/

      }
    }
    else
    { //如果返回的结果为空
      // cout<<"未检测到目标"<<endl;
      is_sancha = 0;
      // if(sancha_find_cnt > 0 && sancha_find_cnt < 2)   sancha_find_cnt = 0;    //连续看到三叉才算，注意连续看到的意义
      if (sancha_find_cnt >= 1)
      {
        sancha_lost_cnt++;
        cout << "丢失三叉" << endl;
      }
      if (sancha_lost_cnt >= 1)
      {
        if (sancha_flag == 0)
        {
          sancha_flag = 1;
          sancha_find_cnt = 0; //计数器清零
          sancha_lost_cnt = 0;
          cout << "准备进入三叉" << endl;
        }
      }

      // cout<<"shigongqu_find_cnt = "<<shigongqu_find_cnt<<endl;
      // cout<<"shigongqu_lost_cnt = ="<< shigongqu_lost_cnt<<endl;
      // if(shigongqu_find_cnt > 0 && shigongqu_find_cnt <= 1)   shigongqu_find_cnt = 0;    //连续看到施工区才算，注意连续看到的意义
      if (shigongqu_find_cnt >= 1)
      {
        shigongqu_lost_cnt++;
        cout << "丢失施工区" << endl;
      }
      if (shigongqu_lost_cnt >= 1)
      {
        zsh_construction_flag = CONSTRUCTION_BEGIN;
        shigongqu_find_cnt = 0; //计数器清零
        shigongqu_lost_cnt = 0;
        cout << "准备进入施工区" << endl;
      }
    }
  }

  void wait_key()
  {
    cout << "detection waiting......" << endl;
    while (1)
    {
      if (start_key == 1 && end_key == 0)
      {
        // ANGLE = 128;
        // SPEED = 0;
        sleep(1);
        start_end_statues = 1;
        break;
      }
      else
        continue;
    }
  }

  int stop() { return 0; }
  int deinit() { return 0; }

  // 外部获取推理结果的接口函数
  std::shared_ptr<DetectionResult> getLastFrame()
  {
    std::shared_ptr<DetectionResult> ret = nullptr;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      // 临界区内，等待预测线程的结果，等待条件变量
      while (_lastResult == nullptr)
      {
        cond_.wait(lock);
      }
      // 拿到当前最新结果帧，返回
      ret = _lastResult;
      _lastResult = nullptr;
    }
    return ret;
  }

  std::string getLabel(int type) { return _predictor->getLabel(type); }

public:
  // 单例模式，避免被复制，用户可自行选择是否使用单例
  // 用户可以用此API调用模型推理，传入的是模型目录，具体目录的配置方式，详见ReadMe文档
  static std::shared_ptr<Detection> DetectionInstance(std::string file_path, std::string model_path)
  {
    static std::shared_ptr<Detection> detectioner = nullptr;
    if (detectioner == nullptr)
    {
      detectioner = std::make_shared<Detection>();

      int ret = detectioner->init(model_path);
      if (ret != 0)
      {
        std::cout << "Detection init error :" << model_path << std::endl;
        exit(-1);
      }
      // Detection类实例化过程中, 启动预测器，完成模型加载并开始异步推理
      // 用户通过调用getLastFrame获取最新一帧的推理结果
      // 如果用户可以设计一个轻巧的模型，使得一帧推理的时间小于相机取图周期，那么异步的方式可以有效的避免神经网络处理带来的延迟
      // 进而方便提升小车速度
      detectioner->start();
    }
    return detectioner;
  }

  // 同上一个API, 默认模型路径是"../../res/model/mobilenet-ssd/"
  // 不建议使用这个API
  static std::shared_ptr<Detection> DetectionInstance()
  {
    static std::shared_ptr<Detection> detectioner = nullptr;
    if (detectioner == nullptr)
    {
      detectioner = std::make_shared<Detection>();
      std::string model_path = "../../res/model/mobilenet-ssd/";
      int ret = detectioner->init(0, model_path);
      if (ret != 0)
      {
        std::cout << "Detection init error :" << model_path << std::endl;
        exit(-1);
      }
      detectioner->start();
    }
    return detectioner;
  }

public:
  int _init(std::string model_config_path)
  {
    _predictor = std::make_shared<Predictor>(model_config_path);
    if (_predictor == nullptr)
    {
      std::cout << "Predictor Create failed." << std::endl;
      return -1;
    }
    int ret = _predictor->init();
    if (ret != 0)
    {
      std::cout << "Predictor init failed." << std::endl;
      exit(-1);
    }

    std::shared_ptr<DetectionResult> result =
        std::make_shared<DetectionResult>();

    result->rgb_frame = frame; //读图

    if (result->rgb_frame.empty())
    {
      std::cout << "Error: Capture Get Empty Error Frame." << std::endl;
      exit(-1);
    }

    try
    {
      _predictor->run(result->rgb_frame);
    }
    catch (exception &e)
    {
      std::cout << "Predictor optimize failed with " << e.what() << std::endl;
      exit(-1);
    }

    return 0;
  }

private:
  bool _is_file = false;
  std::string _file_path;
  bool _log_en;
  std::shared_ptr<DetectionResult> _lastResult;

  std::mutex _mutex;
  std::condition_variable cond_;

  std::unique_ptr<std::thread> _thread;

  std::shared_ptr<Capture> _capture;
  std::shared_ptr<Predictor> _predictor;
};