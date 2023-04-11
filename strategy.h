#pragma once
#include <iostream>
#include "hwRobot.h"
#include "workbench.h"
/// <summary>
/// ������������Ϊ
/// 
/// readMap  
/// ��ȡ��ͼ��ʼ������ ִֻ��һ��
///	(�������޸ĸú���)
/// 
/// afterReadMap 
/// ��ȡ��ͼ��ʼ��������, ��ȡ��һ֡����ǰ ִ��, ִֻ��һ��
/// 
/// cout << "OK" << endl; 
/// ��afterReadMap������OK, 
/// ��readMap��ʼ�����OK,�ܼ���Լ5���ʱ��
/// 
/// for(int frameId = 1; frameId < 9001; frameId++){
/// 
///		readFrame 
///		��ȡÿһ֡����, ������ workbench �� bot ������; 
///		ÿִ֡��һ��
///		(�������޸ĸú���)
///		
///		afterReadFrame 
///		��ȡÿһ֡���ݺ�, ��ȡ��һ֡ǰִ��; 
///		ÿִ֡��һ��
///		 
///     cout << "OK" << endl;
///		��readFrame��ʼ�����OK, �ܼ���15msʱ��
/// }
/// 
/// </summary>
/// 
/// 


class Strategy {
public:
	int wbNum = 0;
	hwRobot botArr[4];
	hwWorkbench wbArr[50];
	int frameId = 0;
	int money = 200000;

	virtual void afterReadMap() = 0;

	virtual void afterReadFrame(int) = 0;

};
