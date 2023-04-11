//#define DEBUG
// 注释掉上面这条后再上传!!!!!!!!************************************注释掉上面这条后再上传!!!!!!!!*****************************
  //                       .,!<i,.                        
  //                     ';><il!<<I`                      
  //                   ^l<>!l!!!l!><!"                    
  //                .,i<i!l!!!!!!!lli<>:.                 
  //              ';><!ll!!!!!!!!!!!!l!><I`               
  //            ^!<>!l!!!!!!!!!!!!!!!!!l!><!"             
  //         .:><ill!!!!!!!!!!!!!!!!!!!!!lli<>;'          
  //       `I<>!l!!!!!!!!!!!!!!!!!!!!!!!!!!!l!><l^        
  //     "!<>!l!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l!i<i,.     
  //  ':><i!l!!!!ll!!!!!!!!!!!!!!!!!!!!!!!ll!!!!lli<>;'   
  //.:~~>iiiiiiii<>l!!!!!!!!!!!!!!!!!!!!!!i<iiiiiiii>~+I. 
  // '...........I+l!!!!!!!!!!!!!!!!!!!!!l~i...........'  
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;+l!!!!!!!!!!!!!!!!!!!!!l~!              
  //             ;_!!!!!!!!!!!!!!!!!!!!!!!+!              
  //             ^!IIIIIIIIIIIIIIIIIIIIIII!"                                                                                                                               
#include <iostream>
#include <vector>
#include "hwRobot.h"
#include "workbench.h"
#include "strategy.h"

#include "strategy_baiyu_yng_1.h"
#include "strategy_baiyu_yng_2.h"
#include "strategy_baiyu_yng_3.h"
#include "strategy_baiyu_yng_4.h"

//#include "hwcout.h"

using namespace std;

Strategy* stra = nullptr;
/////////////////

//////////////////
double botarr_x[4];
double botarr_y[4];
double wbArr_x[50];
double wbArr_y[50];
int wbArr_type[50];

int readMap() {
    char c;
    int botId = 0;
    int wbId = 0;
    for (int y = 99; y >= 0; --y) {
        for (int x = 0; x < 100; ++x) {
            cin >> c;
            if (c == '.')
                continue;
            else if (c == 'A') {
                botarr_x[botId] = x;
                botarr_y[botId] = y;
                ++botId;
            }
            else {
                wbArr_x[wbId] = x;
                wbArr_y[wbId] = y;
                wbArr_type[wbId] = c - '0';
                ++wbId;
            }
        }
    }

    if (wbId == 43)
        stra = new strategy_baiyu_yng1();
    else if (wbId == 25)
        stra = new strategy_baiyu_yng2();
    else if (wbId == 50)
        stra = new strategy_baiyu_yng3();
    else if (wbId == 18)
        stra = new strategy_baiyu_yng4();

    for (int i = 0; i < wbId; ++i)
        stra->wbArr[i].init(i, wbArr_x[i], wbArr_y[i], wbArr_type[i]);
    for (int i = 0; i < 4; ++i)
        stra->botArr[i].init(i, botarr_x[i], botarr_y[i]);

    cin >> c >> c;

    stra->wbNum = wbId;
    return wbId;
}

/// <summary>
/// 从输入读取每帧信息, 更新工作台和bot的成员变量
/// </summary>
/// <param name="K"></param>
void updateBeforeHandle(int K) {
    for (int i = 0; i < K; ++i) {
        stra->wbArr[i].update();
    }
    for (int i = 0; i < 4; ++i) {
        stra->botArr[i].updateBeforeHandle();
    }
}
/// <summary>
/// 在执行完handle之后, 再执行该函数.
/// 让用户在handle中只需要设置目标点, 而不用控制机器人的具体参数
/// </summary>
/// <param name="K"></param>
void updateAfterHandle() {
    for (int i = 0; i < 4; ++i) {
        stra->botArr[i].updateAfterHandle();
    }
}

/// <summary>
/// 读取每帧, 更新工作台与bot信息, 并处理格式问题
/// </summary>
/// <returns></returns>
int readFrame() {
    int tempI;
    int frameId;
    int K;
    cin >> frameId;
    if (frameId == EOF || frameId < 0)
        return frameId;
    cin >> tempI >> K;
    updateBeforeHandle(K);
    char tempC;
    cin >> tempC >> tempC;// OK

    cout << frameId << endl;

    stra->frameId = frameId;
    // 控制bot行为
    stra->afterReadFrame(frameId);
    //handleFrame(frameId);

    updateAfterHandle();
    cout << "OK" << endl;
    return frameId;
}

void init() {
    //hwcout.setCoutBuf();
    //hwendl.setCoutBuf();
}


// todo : 当计算量不足以支撑帧率时, 可选择优化以下部分:
// 1 优化readFrame, 改为快读写法
// 2 优化handleFrame, 将其中遍历的部分改为更高效的查找, 避免重复计算


int main() {
    init();
    //stra = new strategy_yng_2();

    readMap();

    stra->afterReadMap();
    //hwcout << "hwcout 初始化完成" << hwcout;


    cout << "OK" << flush;

    int frameId = 0;
    while (frameId >= 0) {
        frameId = readFrame();

    }
    //of.close(); 
    delete stra;
    return 0;
}
