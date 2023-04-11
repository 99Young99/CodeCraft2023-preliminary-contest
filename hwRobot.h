#pragma once

#include <iostream>
#include "workbench.h"
#include "parameter.h"
//#include "hwcout.h"

// 引入M_PI等常数
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/// <summary>
/// botId: bot的id [0,3]
/// workbenchType: 所处工作台id
/// </summary>
class hwRobot {
public:
	int botId = 0;

	/// <summary>
	/// 所处工作台id
	/// -1表示不在任何工作台附近
	/// [0,工作台总数-1] 表示在对应工作台附近将, 当前机器人所有购买、出售行为均针对该工作台进行
	/// </summary>
	int workbenchId = -1;

	/// <summary>
	/// 0表示未携带
	/// 1-7表示携带物品id
	/// </summary>
	int gootsId = 0;
	/// <summary>
	/// 时间价值系数
	/// </summary>
	double tvr = 1;
	/// <summary>
	/// 碰撞价值系数
	/// </summary>
	double cvr = 1;

	double angleSpeed = 0;
	double lineSpeed[2] = { 0,0 };
	/// <summary>
	/// 当前朝向
	/// </summary>
	double faceTo = 0;

	double x;
	double y;

	/// <summary>
	/// [-2, 6] 之间的浮点数, 表示前进速度
	/// </summary>
	/// <param name="speed"></param>
	void forward(double speed)const {
		cout << "forward " << botId << ' ' << speed << endl;
	}
	/// <summary>
	/// [-π,π]  设置旋转速度, 单位弧度每秒
	/// </summary>
	/// <param name="angle"></param>
	void rotate(double angle)const {
		cout << "rotate " << botId << ' ' << angle << endl;
	}

	/// <summary>
	/// 注意, 调用该函数后, 并不会自动更新goodsId, 需要等到下一帧更新, 或用户自行修改 bot.goodsId
	/// </summary>
	void buy()const {
		cout << "buy " << botId << endl;
	}
	/// <summary>
	/// 注意, 调用该函数后, 并不会自动更新goodsId, 需要等到下一帧更新, 或用户自行修改 bot.goodsId
	/// </summary>
	void sell()const {
		cout << "sell " << botId << endl;
	}
	/// <summary>
	/// 注意, 调用该函数后, 并不会自动更新goodsId, 需要等到下一帧更新, 或用户自行修改 bot.goodsId
	/// </summary>
	void destroy()const {
		cout << "destroy " << botId << endl;
	}

	double getAngleBetween2Point(double p1_x, double p1_y, double p2_x, double p2_y)const {
		double dx = p2_x - p1_x;
		double dy = p2_y - p1_y;
		double angleBetweenWbAndZero = atan2(dy, dx);
		double ans = angleBetweenWbAndZero - this->faceTo;
		return ans > M_PI ? ans - 2 * M_PI : (ans <= -M_PI ? ans + 2 * M_PI : ans);
	}


	double getAngleBetween2PointAndWorkbench(double p1_x, double p1_y, double p2_x, double p2_y)const {
		double dx = p2_x - p1_x;
		double dy = p2_y - p1_y;
		double angleBetweenWbAndZero = atan2(dy, dx);
		double ans = angleBetweenWbAndZero - this->faceTo;
		return ans > M_PI ? ans - 2 * M_PI : (ans <= -M_PI ? ans + 2 * M_PI : ans);
	}

	/// <summary>
	/// 获取与对应工作台之间夹角, (-pi,pi], 正数逆时针,负数顺时针(与赛题保持一致
	/// </summary>
	/// <returns>返回值属于[-pi,pi]</returns>
	const double getAngleBetweenThisBotAndWorkbench(const hwWorkbench& wb)const {
		double dx = wb.x - this->x;
		double dy = wb.y - this->y;
		double angleBetweenWbAndZero = atan2(dy, dx);
		double ans = angleBetweenWbAndZero - this->faceTo;
		return ans > M_PI ? ans - 2 * M_PI : (ans <= -M_PI ? ans + 2 * M_PI : ans);
	}

	/// <summary>
	/// 获取与对应点之间夹角, (-pi,pi], 正数逆时针,负数顺时针(与赛题保持一致
	/// </summary>
	/// <returns>返回值属于[-pi,pi]</returns>
	const double getAngleBetweenThisBotAndWorkbench(const double _x, const double _y)const {
		double dx = _x - this->x;
		double dy = _y - this->y;
		double angleBetweenWbAndZero = atan2(dy, dx);
		double ans = angleBetweenWbAndZero - this->faceTo;
		return ans > M_PI ? ans - 2 * M_PI : (ans <= -M_PI ? ans + 2 * M_PI : ans);
	}

	void init(int _botId, int _x, int _y) {
		botId = _botId;
		x = _x * 0.5 + 0.25;
		y = _y * 0.5 + 0.25;
	}

	void updateBeforeHandle() {
		cin >> workbenchId >> gootsId >>
			tvr >> cvr >> angleSpeed >>
			lineSpeed[0] >> lineSpeed[1] >>
			faceTo >> x >> y;

		/*hwcout << "now in bot" << hwendl;*/
	}

	void updateAfterHandle() {

	}
	// 移动策略 函数与变量
	// 下列函数施工中, 请勿使用
	enum MOVE_STATE {
		HW_NO_TARGET, HW_ON_THE_WAY, HW_ARRIVE_TARGET
	};
	double tarX = 0;
	double tarY = 0;
	int state = HW_NO_TARGET;

	/// <summary>
	/// 目标工作台的id
	/// </summary>
	int targetId = -1;

	/// <summary>
	/// pid参数, 但暂时没用
	/// </summary>
	double mP = 0, mI = 0, mD = 0;

	/// <summary>
	/// 设置目标点
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	bool setMoveTarget(double _x, double _y, int _tarId) {
		state = HW_ON_THE_WAY;
		tarX = _x;
		tarY = _y;
		targetId = _tarId;
		return true;
	}
	bool setMoveTarget(hwWorkbench& wb) {
		return setMoveTarget(wb.x, wb.y, wb.wbId);
	}
	// 0.2 * PI

	/// <summary>
	/// todo: 这里公式可能出错, 需要验证
	/// 
	/// todo: 当bot与目标点夹角为锐角时, 不减速时间可能更长, 需要分情况讨论
	/// 
	/// 计算是否需要减速才能接近, 因为最大转速是PI/秒, 所以要保证  
	/// 当前速度 > ( 当前距离 / 2 * 弧度) 
	/// 
	/// 两边同时平方后 得到函数中的公式
	/// 这种情况下, bot的朝向 与 bot和工作台的连线垂直
	/// </summary>
	/// <returns>返回值为0 则表示无需减速, 返回值不为零的时候, 表示当前距离能转弯到的最小速度</returns>
	double getMaxSpeed() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// 与朝向相切, 且目标点在其上 的 圆弧 的 弧度;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = 0.3;

		// 旋转死区外
		// todo: 当帧率跑不满时,优化重复计算
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// 前方矩形区域, 无需减速
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "行进方向前方矩形区域, 无需减速" << hwendl;
			}*/
			return 6;
		}



		// 目标在朝向两侧分别讨论
		double offset_x, offset_y;
		if (rad > 0) {
			offset_x = offset * cos(this->faceTo + M_PI_2);
			offset_y = offset * sin(this->faceTo + M_PI_2);
		}
		else {
			offset_x = offset * cos(this->faceTo - M_PI_2);
			offset_y = offset * sin(this->faceTo - M_PI_2);
		}

		dis = sqrt(pow(this->x + offset_x - tarX, 2) + pow(this->y + offset_y - tarY, 2));

		double rad2 = getAngleBetween2PointAndWorkbench(this->x + offset_x, this->y + offset_y, tarX, tarY);


		//double minSpeed = dis / 2 / abs(sin(rad2)) * M_PI;
		double minSpeed = dis / 2 / abs(sin(rad2));
		if (outNum != 1) {
			outNum = 2;
			//hwcout << "计算最小速度:" << minSpeed << hwendl;
			/*hwcout << "坐标" << this->x << ':' << this->y << " \nfaceto:" << this->faceTo
				<< "\noffset:" << offset_x << ':' << offset_y << hwendl;*/
		}
		//hwcout << "ping:" << pow(this->x - tarX, 2) + pow(this->y - tarY, 2) << "dis" << dis << " rad" << rad << " minSpd" << minSpeed << hwendl;
		return minSpeed;
	}

	double getMaxSpeed1() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// 与朝向相切, 且目标点在其上的圆弧的弧度;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f1_offset;

		// 旋转死区外
		// todo: 当帧率跑不满时,优化重复计算
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// 前方矩形区域, 无需减速
		if (dis * abs(sin(rad)) < offset) {
			return 6;
		}

		// 目标在朝向两侧分别讨论
		double offset_x, offset_y;
		if (rad > 0) {
			offset_x = offset * cos(this->faceTo + M_PI_2);
			offset_y = offset * sin(this->faceTo + M_PI_2);
		}
		else {
			offset_x = offset * cos(this->faceTo - M_PI_2);
			offset_y = offset * sin(this->faceTo - M_PI_2);
		}

		dis = sqrt(pow(this->x + offset_x - tarX, 2) + pow(this->y + offset_y - tarY, 2));

		double rad2 = getAngleBetween2PointAndWorkbench(this->x + offset_x, this->y + offset_y, tarX, tarY);

		double minSpeed = dis / 2 / abs(sin(rad2)) * f1_getMaxSpeed;
		if (outNum != 1) {
			outNum = 2;
		}
		return minSpeed;
	}

	double getMaxSpeed2() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// 与朝向相切, 且目标点在其上 的 圆弧 的 弧度;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f2_offset;

		// 旋转死区外
		// todo: 当帧率跑不满时,优化重复计算
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// 前方矩形区域, 无需减速
		if (dis * abs(sin(rad)) < offset) {
			return 6;
		}

		// 目标在朝向两侧分别讨论
		double offset_x, offset_y;
		if (rad > 0) {
			offset_x = offset * cos(this->faceTo + M_PI_2);
			offset_y = offset * sin(this->faceTo + M_PI_2);
		}
		else {
			offset_x = offset * cos(this->faceTo - M_PI_2);
			offset_y = offset * sin(this->faceTo - M_PI_2);
		}

		dis = sqrt(pow(this->x + offset_x - tarX, 2) + pow(this->y + offset_y - tarY, 2));

		double rad2 = getAngleBetween2PointAndWorkbench(this->x + offset_x, this->y + offset_y, tarX, tarY);

		double minSpeed = dis / 2 / abs(sin(rad2)) * f2_getMaxSpeed;
		if (outNum != 1) {
			outNum = 2;
		}
		return minSpeed;
	}

	double getMaxSpeed3() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// 与朝向相切, 且目标点在其上 的 圆弧 的 弧度;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f3_offset;

		// 旋转死区外
		// todo: 当帧率跑不满时,优化重复计算
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// 前方矩形区域, 无需减速
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "行进方向前方矩形区域, 无需减速" << hwendl;
			}*/
			return 6;
		}



		// 目标在朝向两侧分别讨论
		double offset_x, offset_y;
		if (rad > 0) {
			offset_x = offset * cos(this->faceTo + M_PI_2);
			offset_y = offset * sin(this->faceTo + M_PI_2);
		}
		else {
			offset_x = offset * cos(this->faceTo - M_PI_2);
			offset_y = offset * sin(this->faceTo - M_PI_2);
		}

		dis = sqrt(pow(this->x + offset_x - tarX, 2) + pow(this->y + offset_y - tarY, 2));

		double rad2 = getAngleBetween2PointAndWorkbench(this->x + offset_x, this->y + offset_y, tarX, tarY);


		double minSpeed = dis / 2 / abs(sin(rad2)) * f3_getMaxSpeed;
		if (outNum != 1) {
			outNum = 2;
		}
		return minSpeed;
	}

	double getMaxSpeed4() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// 与朝向相切, 且目标点在其上 的 圆弧 的 弧度;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f4_offset;

		// 旋转死区外
		// todo: 当帧率跑不满时,优化重复计算
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// 前方矩形区域, 无需减速
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "行进方向前方矩形区域, 无需减速" << hwendl;
			}*/
			return 6;
		}



		// 目标在朝向两侧分别讨论
		double offset_x, offset_y;
		if (rad > 0) {
			offset_x = offset * cos(this->faceTo + M_PI_2);
			offset_y = offset * sin(this->faceTo + M_PI_2);
		}
		else {
			offset_x = offset * cos(this->faceTo - M_PI_2);
			offset_y = offset * sin(this->faceTo - M_PI_2);
		}

		dis = sqrt(pow(this->x + offset_x - tarX, 2) + pow(this->y + offset_y - tarY, 2));

		double rad2 = getAngleBetween2PointAndWorkbench(this->x + offset_x, this->y + offset_y, tarX, tarY);


		double minSpeed = dis / 2 / abs(sin(rad2)) * f4_getMaxSpeed;
		if (outNum != 1) {
			outNum = 2;
		}
		return minSpeed;
	}

	void move_yng1() {

		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed;
		if (abs(dAngle) > M_PI_2) {
			minSpeed = -2;
		}
		else {
			minSpeed = getMaxSpeed1();
		}
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.0;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}

		this->rotate(omega);

	}

	void move_yng2() {

		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed;
		if (abs(dAngle) > M_PI_2) {
			minSpeed = -2;
		}
		else {
			minSpeed = getMaxSpeed2();
		}
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.0;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}

		this->rotate(omega);

	}

	/// <summary>
/// </summary>
	void move_yng3() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed;
		if (abs(dAngle) > M_PI_2) {
			minSpeed = -2;
		}
		else {
			minSpeed = getMaxSpeed3();
		}

		//double minSpeed = getMaxSpeed4();

		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.0;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			//omega = dAngle / (0.02);
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			//omega = dAngle / (0.02);
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}

		this->rotate(omega);
	}

	void move_yng4() {

		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed;
		if (abs(dAngle) > M_PI_2) {
			minSpeed = -2;
		}
		else {
			minSpeed = getMaxSpeed4();
		}

		//double minSpeed = getMaxSpeed4();

		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.0;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			//omega = dAngle / (0.02);
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			//omega = dAngle / (0.02);
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}

		this->rotate(omega);

	}

	/// <summary>
	/// 目标点在背后时减速版本
	/// </summary>
	void move_1() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed;
		if (abs(dAngle) > M_PI_2) {
			minSpeed = -2;
		}
		else {
			minSpeed = getMaxSpeed();
		}
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.5;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}
		this->rotate(omega);

		/*if (minSpeed != 0) {
			this->forward(minSpeed);
		}*/
	}

	/// <summary>
	/// 不减速版本
	/// </summary>
	void move_2() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.5;
		double omega;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega < -M_PI ? -M_PI : omega;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
			omega = dAngle / (0.02);
			omega = omega > M_PI ? M_PI : omega;
		}
		this->rotate(omega);

		/*if (minSpeed != 0) {
			this->forward(minSpeed);
		}*/
	}

	/// <summary>
	/// 只向左转
	/// </summary>
	void move_3() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.5;
		if (dAngle < 0) {
			dAngle = M_PI;
		}
		else {
			dAngle = dAngle * ratio > M_PI ? M_PI : dAngle * ratio;
		}
		this->rotate(dAngle);

		/*if (minSpeed != 0) {
			this->forward(minSpeed);
		}*/
	}

	/// <summary>
	/// 只向右转
	/// </summary>
	void move_4() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// 这里不一定是正面提升, minSpeed选 -2 或 0也是不同方案, 具体可以多试几次, 获取较好结果
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// 过量调节角度, 避免当 角度差 < 一帧能调节的最高角度 时, 产生的误差问题(具体表现为靠近目标点时需要
		double ratio = 1.5;
		if (dAngle < 0) {
			dAngle = dAngle * ratio < -M_PI ? -M_PI : dAngle * ratio;
		}
		else {
			dAngle = -M_PI;
		}
		this->rotate(dAngle);

		/*if (minSpeed != 0) {
			this->forward(minSpeed);
		}*/
	}

	/// <summary>
	/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
	/// </summary>
	/// <returns></returns>
	int moveUntillArriveTarget() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_1();

		return HW_ON_THE_WAY;
	}

	/// <summary>
	/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
	/// </summary>
	/// <returns></returns>
	int moveUntillArriveTarget_onlyTurnLeft() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_3();

		return HW_ON_THE_WAY;
	}

	/// <summary>
	/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
	/// </summary>
	/// <returns></returns>
	int moveUntillArriveTarget_onlyTurnRight() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_4();

		return HW_ON_THE_WAY;
	}

	/// <summary>
/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
/// </summary>
/// <returns></returns>
	int moveUntillArriveTarget_withoutSlowDown() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_2();

		return HW_ON_THE_WAY;
	}

	/// <summary>
/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
/// </summary>
/// <returns></returns>
	int moveUntillArriveTarget_YNG1() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_yng1();

		return HW_ON_THE_WAY;
	}
	/// <summary>
/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
/// </summary>
/// <returns></returns>
	int moveUntillArriveTarget_YNG2() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_yng2();
		//move_yng2_test();

		return HW_ON_THE_WAY;
	}

	/// <summary>
/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
/// </summary>
/// <returns></returns>
	int moveUntillArriveTarget_YNG3() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_yng3();

		return HW_ON_THE_WAY;
	}

	/// <summary>
/// 在handleframe中调用, 当你设置target后, 每帧自动控制bot前往target
/// </summary>
/// <returns></returns>
	int moveUntillArriveTarget_YNG4() {
		if (state == HW_NO_TARGET) {
			return HW_NO_TARGET;
		}
		if (abs(this->x - tarX) <= 0.4 && abs(this->y - tarY) <= 0.4) {
			/*if (sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2)) < 0.4) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}*/
			if (this->workbenchId != -1) {
				this->state = HW_ARRIVE_TARGET;
				return HW_ARRIVE_TARGET;
			}

		}

		// move
		move_yng4();

		return HW_ON_THE_WAY;
	}




	//// 以下为白钰的测试策略算法, 可以自行调整

	/// <summary>
	/// 获取下一个合适的目标点
	/// </summary>
	/// <param name="wbArr"></param>
	/// <param name="K"></param>
	/// <returns>如果返回值为-1则无合适点</returns>
	int findNextTarId(hwWorkbench* wbArr, int K) {
		if (this->gootsId == 0) {
			double minDis = 10000;
			int minId = -1;
			double temp = 0;
			for (int i = 0; i < K; ++i) {
				hwWorkbench& wb = wbArr[i];
				if (wb.ifHaveProduct == 0 || wb.isTarget)
					continue;
				temp = pow(wb.x - this->x, 2) + pow(wb.y - this->y, 2);
				if (temp < minDis) {
					minDis = temp;
					minId = i;
				}
			}
			return minId;
		}
		else {
			double minDis = 10000;
			int minId = -1;
			double temp = 0;
			for (int i = 0; i < K; ++i) {
				hwWorkbench& wb = wbArr[i];
				if (!wb.ifNeed(this->gootsId) || wb.isTarget)
					continue;

				temp = pow(wb.x - this->x, 2) + pow(wb.y - this->y, 2);
				if (temp < minDis) {
					minDis = temp;
					minId = i;
				}
			}
			return minId;
		}
		return -1;
	}

};