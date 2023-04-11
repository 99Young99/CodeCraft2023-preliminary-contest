//#define DEBUG
// ע�͵��������������ϴ�!!!!!!!!************************************ע�͵��������������ϴ�!!!!!!!!*****************************
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
/// �������ȡÿ֡��Ϣ, ���¹���̨��bot�ĳ�Ա����
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
/// ��ִ����handle֮��, ��ִ�иú���.
/// ���û���handle��ֻ��Ҫ����Ŀ���, �����ÿ��ƻ����˵ľ������
/// </summary>
/// <param name="K"></param>
void updateAfterHandle() {
    for (int i = 0; i < 4; ++i) {
        stra->botArr[i].updateAfterHandle();
    }
}

/// <summary>
/// ��ȡÿ֡, ���¹���̨��bot��Ϣ, �������ʽ����
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
    // ����bot��Ϊ
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


// todo : ��������������֧��֡��ʱ, ��ѡ���Ż����²���:
// 1 �Ż�readFrame, ��Ϊ���д��
// 2 �Ż�handleFrame, �����б����Ĳ��ָ�Ϊ����Ч�Ĳ���, �����ظ�����


int main() {
    init();
    //stra = new strategy_yng_2();

    readMap();

    stra->afterReadMap();
    //hwcout << "hwcout ��ʼ�����" << hwcout;


    cout << "OK" << flush;

    int frameId = 0;
    while (frameId >= 0) {
        frameId = readFrame();

    }
    //of.close(); 
    delete stra;
    return 0;
}
