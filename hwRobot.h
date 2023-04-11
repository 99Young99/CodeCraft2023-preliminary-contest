#pragma once

#include <iostream>
#include "workbench.h"
#include "parameter.h"
//#include "hwcout.h"

// ����M_PI�ȳ���
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/// <summary>
/// botId: bot��id [0,3]
/// workbenchType: ��������̨id
/// </summary>
class hwRobot {
public:
	int botId = 0;

	/// <summary>
	/// ��������̨id
	/// -1��ʾ�����κι���̨����
	/// [0,����̨����-1] ��ʾ�ڶ�Ӧ����̨������, ��ǰ���������й��򡢳�����Ϊ����Ըù���̨����
	/// </summary>
	int workbenchId = -1;

	/// <summary>
	/// 0��ʾδЯ��
	/// 1-7��ʾЯ����Ʒid
	/// </summary>
	int gootsId = 0;
	/// <summary>
	/// ʱ���ֵϵ��
	/// </summary>
	double tvr = 1;
	/// <summary>
	/// ��ײ��ֵϵ��
	/// </summary>
	double cvr = 1;

	double angleSpeed = 0;
	double lineSpeed[2] = { 0,0 };
	/// <summary>
	/// ��ǰ����
	/// </summary>
	double faceTo = 0;

	double x;
	double y;

	/// <summary>
	/// [-2, 6] ֮��ĸ�����, ��ʾǰ���ٶ�
	/// </summary>
	/// <param name="speed"></param>
	void forward(double speed)const {
		cout << "forward " << botId << ' ' << speed << endl;
	}
	/// <summary>
	/// [-��,��]  ������ת�ٶ�, ��λ����ÿ��
	/// </summary>
	/// <param name="angle"></param>
	void rotate(double angle)const {
		cout << "rotate " << botId << ' ' << angle << endl;
	}

	/// <summary>
	/// ע��, ���øú�����, �������Զ�����goodsId, ��Ҫ�ȵ���һ֡����, ���û������޸� bot.goodsId
	/// </summary>
	void buy()const {
		cout << "buy " << botId << endl;
	}
	/// <summary>
	/// ע��, ���øú�����, �������Զ�����goodsId, ��Ҫ�ȵ���һ֡����, ���û������޸� bot.goodsId
	/// </summary>
	void sell()const {
		cout << "sell " << botId << endl;
	}
	/// <summary>
	/// ע��, ���øú�����, �������Զ�����goodsId, ��Ҫ�ȵ���һ֡����, ���û������޸� bot.goodsId
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
	/// ��ȡ���Ӧ����̨֮��н�, (-pi,pi], ������ʱ��,����˳ʱ��(�����Ᵽ��һ��
	/// </summary>
	/// <returns>����ֵ����[-pi,pi]</returns>
	const double getAngleBetweenThisBotAndWorkbench(const hwWorkbench& wb)const {
		double dx = wb.x - this->x;
		double dy = wb.y - this->y;
		double angleBetweenWbAndZero = atan2(dy, dx);
		double ans = angleBetweenWbAndZero - this->faceTo;
		return ans > M_PI ? ans - 2 * M_PI : (ans <= -M_PI ? ans + 2 * M_PI : ans);
	}

	/// <summary>
	/// ��ȡ���Ӧ��֮��н�, (-pi,pi], ������ʱ��,����˳ʱ��(�����Ᵽ��һ��
	/// </summary>
	/// <returns>����ֵ����[-pi,pi]</returns>
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
	// �ƶ����� ���������
	// ���к���ʩ����, ����ʹ��
	enum MOVE_STATE {
		HW_NO_TARGET, HW_ON_THE_WAY, HW_ARRIVE_TARGET
	};
	double tarX = 0;
	double tarY = 0;
	int state = HW_NO_TARGET;

	/// <summary>
	/// Ŀ�깤��̨��id
	/// </summary>
	int targetId = -1;

	/// <summary>
	/// pid����, ����ʱû��
	/// </summary>
	double mP = 0, mI = 0, mD = 0;

	/// <summary>
	/// ����Ŀ���
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
	/// todo: ���﹫ʽ���ܳ���, ��Ҫ��֤
	/// 
	/// todo: ��bot��Ŀ���н�Ϊ���ʱ, ������ʱ����ܸ���, ��Ҫ���������
	/// 
	/// �����Ƿ���Ҫ���ٲ��ܽӽ�, ��Ϊ���ת����PI/��, ����Ҫ��֤  
	/// ��ǰ�ٶ� > ( ��ǰ���� / 2 * ����) 
	/// 
	/// ����ͬʱƽ���� �õ������еĹ�ʽ
	/// ���������, bot�ĳ��� �� bot�͹���̨�����ߴ�ֱ
	/// </summary>
	/// <returns>����ֵΪ0 ���ʾ�������, ����ֵ��Ϊ���ʱ��, ��ʾ��ǰ������ת�䵽����С�ٶ�</returns>
	double getMaxSpeed() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// �볯������, ��Ŀ��������� �� Բ�� �� ����;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = 0.3;

		// ��ת������
		// todo: ��֡���ܲ���ʱ,�Ż��ظ�����
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// ǰ����������, �������
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "�н�����ǰ����������, �������" << hwendl;
			}*/
			return 6;
		}



		// Ŀ���ڳ�������ֱ�����
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
			//hwcout << "������С�ٶ�:" << minSpeed << hwendl;
			/*hwcout << "����" << this->x << ':' << this->y << " \nfaceto:" << this->faceTo
				<< "\noffset:" << offset_x << ':' << offset_y << hwendl;*/
		}
		//hwcout << "ping:" << pow(this->x - tarX, 2) + pow(this->y - tarY, 2) << "dis" << dis << " rad" << rad << " minSpd" << minSpeed << hwendl;
		return minSpeed;
	}

	double getMaxSpeed1() {

		double dis = sqrt(pow(this->x - tarX, 2) + pow(this->y - tarY, 2));

		static int outNum = -1;

		// �볯������, ��Ŀ��������ϵ�Բ���Ļ���;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f1_offset;

		// ��ת������
		// todo: ��֡���ܲ���ʱ,�Ż��ظ�����
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// ǰ����������, �������
		if (dis * abs(sin(rad)) < offset) {
			return 6;
		}

		// Ŀ���ڳ�������ֱ�����
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

		// �볯������, ��Ŀ��������� �� Բ�� �� ����;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f2_offset;

		// ��ת������
		// todo: ��֡���ܲ���ʱ,�Ż��ظ�����
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// ǰ����������, �������
		if (dis * abs(sin(rad)) < offset) {
			return 6;
		}

		// Ŀ���ڳ�������ֱ�����
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

		// �볯������, ��Ŀ��������� �� Բ�� �� ����;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f3_offset;

		// ��ת������
		// todo: ��֡���ܲ���ʱ,�Ż��ظ�����
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// ǰ����������, �������
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "�н�����ǰ����������, �������" << hwendl;
			}*/
			return 6;
		}



		// Ŀ���ڳ�������ֱ�����
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

		// �볯������, ��Ŀ��������� �� Բ�� �� ����;
		double rad = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		const double offset = f4_offset;

		// ��ת������
		// todo: ��֡���ܲ���ʱ,�Ż��ظ�����
		if (dis > M_2_PI * 6 + offset) {
			return 6;
		}

		// ǰ����������, �������
		if (dis * abs(sin(rad)) < offset) {
			/*if (outNum != 0) {
				outNum = 0;
				hwcout << "�н�����ǰ����������, �������" << hwendl;
			}*/
			return 6;
		}



		// Ŀ���ڳ�������ֱ�����
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

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
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

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
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

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
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

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
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

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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
	/// Ŀ����ڱ���ʱ���ٰ汾
	/// </summary>
	void move_1() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
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

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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
	/// �����ٰ汾
	/// </summary>
	void move_2() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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
	/// ֻ����ת
	/// </summary>
	void move_3() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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
	/// ֻ����ת
	/// </summary>
	void move_4() {
		double dAngle = getAngleBetweenThisBotAndWorkbench(tarX, tarY);

		// ���ﲻһ������������, minSpeedѡ -2 �� 0Ҳ�ǲ�ͬ����, ������Զ��Լ���, ��ȡ�Ϻý��
		double minSpeed = getMaxSpeed();
		//minSpeed = getMaxSpeed();
		if (minSpeed < 6) {
			this->forward(minSpeed);
		}
		else {
			this->forward(6);
		}

		// �������ڽǶ�, ���⵱ �ǶȲ� < һ֡�ܵ��ڵ���߽Ƕ� ʱ, �������������(�������Ϊ����Ŀ���ʱ��Ҫ
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
	/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
	/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
	/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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
/// ��handleframe�е���, ��������target��, ÿ֡�Զ�����botǰ��target
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




	//// ����Ϊ���ڵĲ��Բ����㷨, �������е���

	/// <summary>
	/// ��ȡ��һ�����ʵ�Ŀ���
	/// </summary>
	/// <param name="wbArr"></param>
	/// <param name="K"></param>
	/// <returns>�������ֵΪ-1���޺��ʵ�</returns>
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