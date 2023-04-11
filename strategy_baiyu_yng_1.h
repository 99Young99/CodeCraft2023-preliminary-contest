#pragma once
#include "strategy.h"
#include <vector>
#include "parameter.h"
#include "hwcout.h"

class strategy_baiyu_yng1 :public Strategy {

	vector<vector<double>> wbDis;
	vector<vector<double>> wbDisSq;
	vector<vector<int>> wbTypeArr = vector<vector<int>>(10, vector<int>());

	// ��0���ʾ������Ƿ���Ϊ������Ŀ��, ��ʶ����̨������˱������������ͻ���Ļ�����˭ѡ�У�������λ��ʾ�����磬0b110����ζ����������˱�����1�Ż����2�Ż���Ļ�����������
	// ��1���ʾ�Ƿ���Ϊ�����Ŀ��
	vector<vector<int>> wbIfTar{ 50,vector<int>(2, 0) };

	// ��0���ʾĿǰ�ƶ����ڼ���Ŀ��, ֵΪ0��ʾ��Ŀ��, ֵΪ1��ʾ��һ��Ŀ��(����, ֵΪ2��ʾ�ڶ���Ŀ��(����
	// ��1��, ��2��Ϊ ����Ŀ���wbid
	vector<vector<int>> botTarId{ 4, vector<int>(3, 0) };

	// ��ǰĳ������Ĳ�Ʒ����Ҫ�ĳ̶ȣ���ϵ�������ѡ��
	char productNeed[8];
	// �����͹���̨�ĸ���
	int n1, n2, n3, n4, n5, n6, n7, n8, n9;
	// ��ʶ��һ��״̬�Ƿ�ɹ�����
	bool successUpdate[4] = { false };

	//double botNowWeight[4] = { 0,0,0,0 };

	//const int valueArr[8] = { 0,3000,3200,3400,7100,7800,8300,29000 };
	const int valueArr[8] = { 0,4000,4000,4000,7000,7000,7000,29000 };

	const int weightOffset[8] = { 0,3000,3000,3000,22000,22000,22000,29000 };

	//const int valueArr[10] = { 0,0,0,0,0,0,0,0,0,0 };



	void initProductNeed(char* productNeed) {

		for (int i = 0; i < 8; ++i)
			productNeed[i] = 0;
		//if (n9 >= 1)     // ���if-else��䣬Ӱ����ǻ����˵Ĺ�������������������Ʒ�Ƿ���Ҫ���粻����Ҫ���򲻹���
		//	/*for (int i = 1; i < 8; ++i)
		//		productNeed[i] = 10;*/
		//else {

		if (n8 >= 1)
			productNeed[7] = 10;
		for (int i = 0; i < n7; ++i)
			for (int j = 4; j <= 6; ++j)
				if (wbArr[wbTypeArr[7][i]].ifNeed(j))
					++productNeed[j];
		for (int i = 0; i < n6; ++i) {
			if (wbArr[wbTypeArr[6][i]].ifNeed(3))
				++productNeed[3];
			if (wbArr[wbTypeArr[6][i]].ifNeed(2))
				++productNeed[2];
		}
		for (int i = 0; i < n5; ++i) {
			if (wbArr[wbTypeArr[5][i]].ifNeed(3))
				++productNeed[3];
			if (wbArr[wbTypeArr[5][i]].ifNeed(1))
				++productNeed[1];
		}
		for (int i = 0; i < n4; ++i) {
			if (wbArr[wbTypeArr[4][i]].ifNeed(2))
				++productNeed[2];
			if (wbArr[wbTypeArr[4][i]].ifNeed(1))
				++productNeed[1];
		}
		//}

	}

	const double calDist(const double x1, const double y1, const double x2, const double y2)const {
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}
	const double calDistSq(const double x1, const double y1, const double x2, const double y2)const {
		return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	}

	void initDisV2AndWbTypeArr() {
		wbDis.resize(wbNum);
		wbDisSq.resize(wbNum);
		for (auto& one : wbDis) {
			one.resize(wbNum, -1);
		}
		for (auto& one : wbDisSq) {
			one.resize(wbNum, -1);
		}
		for (int i = 0; i < wbNum; ++i) {
			wbTypeArr[wbArr[i].wbType].emplace_back(i);

			for (int j = 0; j < i; ++j) {
				int jType = wbArr[j].wbType;
				bool dis_flag = false;
				switch (wbArr[i].wbType)
				{
				case 1:
					if (jType == 4 || jType == 5 || jType == 9) dis_flag = true;
					break;
				case 2:
					if (jType == 4 || jType == 6 || jType == 9) dis_flag = true;
					break;
				case 3:
					if (jType == 5 || jType == 6 || jType == 9) dis_flag = true;
					break;
				case 4:
					if (jType == 1 || jType == 2 || jType == 7 || jType == 9) dis_flag = true;
					break;
				case 5:
					if (jType == 1 || jType == 3 || jType == 7 || jType == 9) dis_flag = true;
					break;
				case 6:
					if (jType == 2 || jType == 3 || jType == 7 || jType == 9) dis_flag = true;
					break;
				case 7:
					if (jType == 4 || jType == 5 || jType == 6 || jType == 8 || jType == 9) dis_flag = true;
					break;
				case 8:
					if (jType == 7) dis_flag = true;
					break;
				case 9:
					if (jType != 8 && jType != 9) dis_flag = true;
					break;
				default:
					break;
				}
				if (dis_flag) {
					double _disSq = calDistSq(wbArr[i].x, wbArr[i].y, wbArr[j].x, wbArr[j].y);
					wbDisSq[i][j] = _disSq;
					wbDisSq[j][i] = _disSq;

					double _dis = sqrt(_disSq);
					wbDis[i][j] = _dis;
					wbDis[j][i] = _dis;
				}
			}
		}
	}

	// botId ȥwbid1 ȡ��Ʒgoodid �͵�wbId2 �� Ȩ��ֵ
	double getWeight(int botId, int wbId1, int wbId2, int goodId) {
		if (wbDis[wbId1][wbId2] == -1) {
			return 0;
		}
		double value = valueArr[goodId];
		int mstate = wbArr[wbId2].materialsState;
		for (int i = 0; i < 8; ++i) {
			if (mstate & 1) {		// �ж�wbArr[wbid2]�費��Ҫ�������
				value += valueArr[i];	// Ϊ�˲�ȫ����ӵ�Ȩ�أ����wbArr[wbid2]��ǰӵ�еĲ���Խ�࣬����Ȩ�ؾ�Խ���������ȱ���ϣ�������Ӧ�Ļ��ƴ���
			}
			mstate = mstate >> 1;
		}


		double _disBot2Wb1 = calDist(botArr[botId].x, botArr[botId].y, wbArr[wbId1].x, wbArr[wbId1].y);
		//return pow(value, 1.0) / _disBot2Wb1;
		return pow(value, 1.0) / (pow(_disBot2Wb1 + wbDis[wbId1][wbId2], 1.0));
		
	}

	// botId����Ʒgoodid�͵�wbId2��Ȩ��ֵ
	double getWeight2(int botId, int id2, int goodId) {

		double value = valueArr[goodId];
		int mstate = wbArr[id2].materialsState;	// ���ڹ���̨8��9����ӵ�е�ԭ���Ͽ���˵һֱ��0�������Կ���
		for (int i = 0; i < 8; ++i) {
			if (mstate & 1) {		// �ж�wbArr[id2]�費��Ҫ�������
				value += valueArr[i];	// Ϊ�˲�ȫ����ӵ�Ȩ�أ����wbArr[id2]��ǰӵ�еĲ���Խ�࣬����Ȩ�ؾ�Խ���������ȱ���ϣ�������Ӧ�Ļ��ƴ���
			}
			mstate = mstate >> 1;
		}

		double _disBot2Wb2 = calDist(botArr[botId].x, botArr[botId].y, wbArr[id2].x, wbArr[id2].y);
		return value / _disBot2Wb2;
		//return 1 / (_disBot2Wb2);
	}

	// ���ݻ�������̶ȵļ۸���
	double getWeightOffset(int botId, int wbId1, int wbId2, int goodId) {
		int typeId2 = wbArr[wbId2].wbType;

		int pn = productNeed[typeId2];
		
		double n = 0;
		switch (typeId2)
		{
		case 1:
			n = n4 + n5;
			break;
		case 2:
			n = n4 + n6;
			break;
		case 3:
			n = n5 + n6;
			break;
		case 4:
			n = n7 + n9;
			break;
		case 5:
			n = n7 + n9;
			break;
		case 6:
			n = n7 + n9;
			break;
		case 7:
			n = n8 + n9;
			break;
		default:
			break;
		}

		double offset = pn / n * weightOffset[goodId];

		return offset;

	}
	

	// �ڵ�ǰ��ͼ�У�����getWeight()�����Ȩ�أ�����botID���ʺ����������Ĺ���̨wbArr[id1]�����ʺ����������Ĺ���̨wbArr[id2]
	int loopId1ToId2(int tp1, int tp2, int botId, int goodId, int& wbId1, int& wbId2, double& maxWeight) {
		
		if (productNeed[tp1] <= 0)		// �鿴��ǰtp1���͵Ļ����Ƿ���Ҫ���漰�������
			return 0;

		// ����8�Ź���̨�Ƿ�����Ҫ��
		for (int id2 : wbTypeArr[tp2]) {
			// wbArr[id2]�Ƿ���Ҫ��tp1��Ʒ, �Ƿ���Ϊ������Ŀ�꣨��Ϊ��������Ŀ��Ļ����Ͳ�ѡ����
			if (wbArr[id2].ifNeed(tp1) && !(wbIfTar[id2][0] & (1 << tp1))) {

				double weightOffset = getWeightOffset(botId, 0, id2, goodId);
				for (int id1 : wbTypeArr[tp1]) {
					
					double x = botArr[botId].x, y = botArr[botId].y;

					//// �����ȥ7�û�, �����ж������Ƿ���7����, ���ô����ж�
					//if (tp1 == 7) 
					//	if (fabs(x - wbArr[id1].x) > 0.4001 || fabs(y - wbArr[id1].y) > 0.4001)
					//		continue;



					double dist = calDist(x, y, wbArr[id1].x, wbArr[id1].y);
					bool checkProduct = wbArr[id1].ifHaveProduct;
					if (frameId < 50 && wbArr[id1].rpt > 0 && dist > (wbArr[id1].rpt * 0.02 * 6))
					    checkProduct = true;

					//if (tp1 < 4) {
					//	if (checkProduct) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
					//		double nowWeight = getWeight(botId, id1, id2, goodId) + getWeightOffset(botId, id1, id2, goodId);
					//		if (nowWeight > maxWeight) {
					//			maxWeight = nowWeight;
					//			wbId1 = id1;
					//			wbId2 = id2;
					//		}
					//	}
					//}
					//else {
					//	if (checkProduct && !wbIfTar[id1][1]) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
					//		double nowWeight = getWeight(botId, id1, id2, goodId) + getWeightOffset(botId, id1, id2, goodId);
					//		if (nowWeight > maxWeight) {
					//			maxWeight = nowWeight;
					//			wbId1 = id1;
					//			wbId2 = id2;
					//		}
					//	}
					//}
					
					if (checkProduct && !wbIfTar[id1][1]) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
						double nowWeight = getWeight(botId, id1, id2, goodId) + weightOffset;
						if (nowWeight > maxWeight) {
							maxWeight = nowWeight;
							wbId1 = id1;
							wbId2 = id2;
						}
					}		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
					
				}
			}
		}
		return 0;
	};


	// ���Ҳ���Ŀ��ʱ, ������һ��123����
	int loopId1ToId2_noLock(int tp1, int tp2, int botId, int goodId, int& wbId1, int& wbId2, double& maxWeight) {

		if (productNeed[tp1] <= 0)		// �鿴��ǰtp1���͵Ļ����Ƿ���Ҫ���漰�������
			return 0;

		// ����8�Ź���̨�Ƿ�����Ҫ��
		for (int id2 : wbTypeArr[tp2]) {
			// wbArr[id2]�Ƿ���Ҫ��tp1��Ʒ, �Ƿ���Ϊ������Ŀ�꣨��Ϊ��������Ŀ��Ļ����Ͳ�ѡ����
			if (wbArr[id2].ifNeed(tp1) && !(wbIfTar[id2][0] & (1 << tp1))) {

				double weightOffset = getWeightOffset(botId, 0, id2, goodId);
				for (int id1 : wbTypeArr[tp1]) {

					double x = botArr[botId].x, y = botArr[botId].y;

					//// �����ȥ7�û�, �����ж������Ƿ���7����, ���ô����ж�
					//if (tp1 == 7) 
					//	if (fabs(x - wbArr[id1].x) > 0.4001 || fabs(y - wbArr[id1].y) > 0.4001)
					//		continue;



					double dist = calDist(x, y, wbArr[id1].x, wbArr[id1].y);
					bool checkProduct = wbArr[id1].ifHaveProduct;
					if (frameId < 50 && wbArr[id1].rpt > 0 && dist > (wbArr[id1].rpt * 0.02 * 6))
						checkProduct = true;

					if (tp1 < 4) {
						if (checkProduct) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
							double nowWeight = getWeight(botId, id1, id2, goodId) + getWeightOffset(botId, id1, id2, goodId);
							if (nowWeight > maxWeight) {
								maxWeight = nowWeight;
								wbId1 = id1;
								wbId2 = id2;
							}
						}
					}
					else {
						if (checkProduct && !wbIfTar[id1][1]) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
							double nowWeight = getWeight(botId, id1, id2, goodId) + getWeightOffset(botId, id1, id2, goodId);
							if (nowWeight > maxWeight) {
								maxWeight = nowWeight;
								wbId1 = id1;
								wbId2 = id2;
							}
						}
					}

					//if (checkProduct && !wbIfTar[id1][1]) {		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��
					//	double nowWeight = getWeight(botId, id1, id2, goodId) + weightOffset;
					//	if (nowWeight > maxWeight) {
					//		maxWeight = nowWeight;
					//		wbId1 = id1;
					//		wbId2 = id2;
					//	}
					//}		// �Ƿ񼴽�ӵ��tp2��Ʒ, �Ƿ���Ϊ�����Ŀ��

				}
			}
		}
		return 0;
	};


	// �ڵ�ǰ��ͼ�У�����getWeight()�����Ȩ�أ�����botID���ʺ�����sell�Ĺ���̨wbArr[wb2Id]
	void loopWbId2(int botId, int goodId, int wb2Type, int& wb2Id, double& maxWeight) {

		// ������productNeeed����Ϊ��֤goodId����Ҫ
		for (int id2 : wbTypeArr[wb2Type]) {
			if (wbArr[id2].ifNeed(goodId) && !(wbIfTar[id2][0] & (1 << goodId))) {
				double nowWeight = getWeight2(botId, id2, goodId);
				if (nowWeight > maxWeight) {
					maxWeight = nowWeight;
					wb2Id = id2;
				}
			}
		}
	}

	// ��ͼ�ҵ���ѵ�buy����̨ ���������ﲻ���ǹ���̨���ޱ���������������������
	int findBestWbBuy(int botId, int goodId, double& maxWeight) {

		int tarId = -1;
		int id2 = botTarId[botId][2];
		for (int id1 : wbTypeArr[goodId]) {
			if (wbArr[id1].ifHaveProduct) {		// �Ƿ�ӵ��goodId��Ʒ
				double nowWeight = getWeight(botId, id1, id2, goodId);
				if (nowWeight > maxWeight) {
					maxWeight = nowWeight;
					tarId = id1;
				}
			}
		}
		return tarId;

	};

	int findNext(int botId) {
		int wbId1 = -1;
		int wbId2 = -1;

		double maxWeight = 0;

// todo
		loopId1ToId2(7, 8, botId, 7, wbId1, wbId2, maxWeight);

		loopId1ToId2(6, 7, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 7, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 7, botId, 4, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 6, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 6, botId, 2, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 5, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 5, botId, 1, wbId1, wbId2, maxWeight);

		loopId1ToId2(2, 4, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 4, botId, 1, wbId1, wbId2, maxWeight);


		loopId1ToId2(7, 9, botId, 7, wbId1, wbId2, maxWeight);
		/*loopId1ToId2(6, 9, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 9, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 9, botId, 4, wbId1, wbId2, maxWeight);
		loopId1ToId2(3, 9, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 9, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 9, botId, 1, wbId1, wbId2, maxWeight);*/


		if (wbId1 != -1 && wbId2 != -1) {
			botTarId[botId] = { 1,wbId1,wbId2 };
			//botNowWeight[botId] = maxWeight;
			wbIfTar[wbId1][1] = 1;


			if (wbArr[wbId2].wbType != 8 && wbArr[wbId2].wbType != 9)
				wbIfTar[wbId2][0] = (wbIfTar[wbId2][0] | (1 << wbArr[wbId1].wbType));
			return 0;
		}
		else {
			//botNowWeight[botId] = 0;
			botTarId[botId] = { 0, 0, 0 };
			return 1;
		}

	}

	int findNext_noLock(int botId) {
		int wbId1 = -1;
		int wbId2 = -1;

		double maxWeight = 0;


		loopId1ToId2_noLock(3, 6, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2_noLock(2, 6, botId, 2, wbId1, wbId2, maxWeight);

		loopId1ToId2_noLock(3, 5, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2_noLock(1, 5, botId, 1, wbId1, wbId2, maxWeight);

		loopId1ToId2_noLock(2, 4, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2_noLock(1, 4, botId, 1, wbId1, wbId2, maxWeight);



		if (wbId1 != -1 && wbId2 != -1) {
			botTarId[botId] = { 1,wbId1,wbId2 };
			//botNowWeight[botId] = maxWeight;
			wbIfTar[wbId1][1] = 1;


			if (wbArr[wbId2].wbType != 8 && wbArr[wbId2].wbType != 9)
				wbIfTar[wbId2][0] = (wbIfTar[wbId2][0] | (1 << wbArr[wbId1].wbType));
			return 0;
		}
		else {
			//botNowWeight[botId] = 0;
			botTarId[botId] = { 0, 0, 0 };
			return 1;
		}

	}

	int findNext1_dontSetTar(int botId, int& wbId1, int& wbId2, double& maxWeight) {
		wbId1 = -1;
		wbId2 = -1;

		maxWeight = 0;
		// todo
		loopId1ToId2(7, 8, botId, 7, wbId1, wbId2, maxWeight);

		loopId1ToId2(6, 7, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 7, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 7, botId, 4, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 6, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 6, botId, 2, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 5, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 5, botId, 1, wbId1, wbId2, maxWeight);

		loopId1ToId2(2, 4, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 4, botId, 1, wbId1, wbId2, maxWeight);


		loopId1ToId2(7, 9, botId, 7, wbId1, wbId2, maxWeight);
		/*loopId1ToId2(6, 9, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 9, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 9, botId, 4, wbId1, wbId2, maxWeight);
		loopId1ToId2(3, 9, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 9, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 9, botId, 1, wbId1, wbId2, maxWeight);*/


		if (wbId1 != -1 && wbId2 != -1) {
			return 0;
		}
		else {
			return 1;
		}

	}

	void findNext2(int botId, int& wbId1, int& wbId2, double& maxWeight) {

		if (botArr[botId].gootsId)		// ���������ĿǰЯ���Ż���Ͳ�����
			return;

		loopId1ToId2(7, 8, botId, 7, wbId1, wbId2, maxWeight);

		loopId1ToId2(6, 7, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 7, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 7, botId, 4, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 6, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 6, botId, 2, wbId1, wbId2, maxWeight);

		loopId1ToId2(3, 5, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 5, botId, 1, wbId1, wbId2, maxWeight);

		loopId1ToId2(2, 4, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 4, botId, 1, wbId1, wbId2, maxWeight);


		loopId1ToId2(7, 9, botId, 7, wbId1, wbId2, maxWeight);
		loopId1ToId2(6, 9, botId, 6, wbId1, wbId2, maxWeight);
		loopId1ToId2(5, 9, botId, 5, wbId1, wbId2, maxWeight);
		loopId1ToId2(4, 9, botId, 4, wbId1, wbId2, maxWeight);
		loopId1ToId2(3, 9, botId, 3, wbId1, wbId2, maxWeight);
		loopId1ToId2(2, 9, botId, 2, wbId1, wbId2, maxWeight);
		loopId1ToId2(1, 9, botId, 1, wbId1, wbId2, maxWeight);
	}

	void findNext3(int botId, int goodId, int& wb2Id, double& maxWeight) {

		if (goodId == 1) {
			loopWbId2(botId, 1, 4, wb2Id, maxWeight);
			loopWbId2(botId, 1, 5, wb2Id, maxWeight);
			//loopWbId2(botId, 1, 9, wb2Id, maxWeight);
		}
		else if (goodId == 2) {
			loopWbId2(botId, 2, 4, wb2Id, maxWeight);
			loopWbId2(botId, 2, 6, wb2Id, maxWeight);
			//loopWbId2(botId, 2, 9, wb2Id, maxWeight);
		}
		else if (goodId == 3) {
			loopWbId2(botId, 3, 5, wb2Id, maxWeight);
			loopWbId2(botId, 3, 6, wb2Id, maxWeight);
			//loopWbId2(botId, 3, 9, wb2Id, maxWeight);
		}
		else if (goodId == 4) {
			loopWbId2(botId, 4, 7, wb2Id, maxWeight);
			//loopWbId2(botId, 4, 9, wb2Id, maxWeight);
		}
		else if (goodId == 5) {
			loopWbId2(botId, 5, 7, wb2Id, maxWeight);
			//loopWbId2(botId, 5, 9, wb2Id, maxWeight);
		}
		else if (goodId == 6) {
			loopWbId2(botId, 6, 7, wb2Id, maxWeight);
			//loopWbId2(botId, 6, 9, wb2Id, maxWeight);
		}
		else if (goodId == 7) {
			loopWbId2(botId, 7, 8, wb2Id, maxWeight);
			loopWbId2(botId, 7, 9, wb2Id, maxWeight);
		}
	}

	void avoidCrash() {

		int crashNum[4] = { 0 };
		int crashId[4][4] = { {-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1} };
		double vi_par, vi_ver, vj_par, vj_ver, xi, yi, xj, yj, boti_vx, boti_vy, botj_vx, botj_vy, angleTowPoint;
		double vij_par, vij_ver, botk_vx, botk_vy, botm_vx, botm_vy, cosAngle, sinAngle, dist, t_catch;
		double ri, rj, face_to_i, face_to_j;
		bool rotate_dir[4][4];		// ֵΪtrue����ʾ���߶�Ӧ����ʱ����ת��ֵΪfalse����ʾ���߶�Ӧ��˳ʱ����ת
		bool rotate_flag[4];		// ֵΪtrue����ʾӦ����ת��ֵΪfalse����ʾ��Ӧ����ת����ֹ���˵�ʱ����ת

		// ��ײ���ģ�飨Ŀǰֻ���۽���ײ��
		for (int i = 0; i < 4; ++i) {
			boti_vx = botArr[i].lineSpeed[0];
			boti_vy = botArr[i].lineSpeed[1];
			face_to_i = botArr[i].faceTo;

			double deltaAngle_i = fabs(atan2(boti_vy, boti_vx) - face_to_i);
			if (deltaAngle_i > M_PI) {
				deltaAngle_i = 2 * M_PI - deltaAngle_i;
			}

			if (deltaAngle_i > M_PI_2) {	// ˵����ʱ�������������˶�
				rotate_flag[i] = false;
			}
			else {
				rotate_flag[i] = true;
			}
		}

		for (int i = 0; i < 4; ++i) {

			xi = botArr[i].x;
			yi = botArr[i].y;
			boti_vx = botArr[i].lineSpeed[0];
			boti_vy = botArr[i].lineSpeed[1];
			face_to_i = botArr[i].faceTo;
			if (botArr[i].gootsId)
				ri = 0.53;
			else
				ri = 0.45;

			for (int j = i + 1; j < 4; ++j) {

				xj = botArr[j].x;
				yj = botArr[j].y;
				dist = calDist(xi, yi, xj, yj);
				botj_vx = botArr[j].lineSpeed[0];
				botj_vy = botArr[j].lineSpeed[1];
				face_to_j = botArr[j].faceTo;
				if (botArr[j].gootsId)
					rj = 0.53;
				else
					rj = 0.45;

				if (botj_vx * boti_vx + botj_vy * boti_vy > 0)		// �����ĵ����ͬ���˶����Ȳ���
					continue;

				double delta_vx = botj_vx - boti_vx, delta_vy = botj_vy - boti_vy;
				double deltax = xj - xi, deltay = yj - yi;

				if (delta_vx * deltax + delta_vy * deltay > 0)	// �����ĵ����������λ������������ٶ������Ƿ��򣨳ɶ۽ǣ���������˵���������������ڿ���
					continue;

				angleTowPoint = atan2(yj - yi, xj - xi);
				cosAngle = cos(angleTowPoint);
				sinAngle = sin(angleTowPoint);

				vi_par = boti_vx * cosAngle + boti_vy * sinAngle;	// i���������ߵ�ˮƽ�ٶ�
				vi_ver = boti_vx * sinAngle - boti_vy * cosAngle;	// i���������ߵĴ�ֱ�ٶ�
				vj_par = botj_vx * cosAngle + botj_vy * sinAngle;	// j���������ߵ�ˮƽ�ٶ�
				vj_ver = botj_vx * sinAngle - botj_vy * cosAngle;	// j���������ߵĴ�ֱ�ٶ�

				vij_par = vj_par - vi_par;	// ��i��j��j��iֱ�ӿ������ٶ�
				vij_ver = vj_ver - vi_ver;	// ��i��j��j��i��ֱԶ����ٶ�

				t_catch = dist / vij_par;

				if (rotate_flag[i] && rotate_flag[j]) {	// ���߶�û���������˶�
					if (fabs(t_catch) > f1_crash_t)	// ��ײʱ�仹ʣ�Ƚ϶�Ļ�����������
						continue;
				}
				else {	// ������һ�����������˶�
					if (fabs(t_catch) > 1 * f1_crash_t)	// ��ײʱ�仹ʣ�Ƚ϶�Ļ�����������
						continue;
				}

				if (fabs(vij_ver * t_catch) < (ri + rj)) {	// ˵����ʱ�����ǵ��ٶȱ��ֲ���Ļ�������t_catch��ʱ�䣬���Ǿͻ���ײ

					++crashNum[i];
					++crashNum[j];
					crashId[i][j] = 1;
					crashId[j][i] = 1;

					if (deltax * delta_vy - deltay * delta_vx < 0) {	// �����Ĳ����ֵΪ���������pi�Ľ��ٶ���ת
						rotate_dir[i][j] = true;
						rotate_dir[j][i] = true;
					}
					else {		// ֵΪ���������-pi�Ľ��ٶ���ת
						rotate_dir[i][j] = false;
						rotate_dir[j][i] = false;
					}
				}
			}
		}

		// ��ײ����ģ��
		for (int k = 0; k < 4; ++k) {

			if (crashNum[k] == 0)	// û�м���Ҫ��ײ��Ŀ�꣬��������
				continue;

			botk_vx = botArr[k].lineSpeed[0];
			botk_vy = botArr[k].lineSpeed[1];

			if (crashNum[k] >= 2) {	// ������õ�����ײĿ�������ڵ���2����ֻ�����ٴ��������������
				botArr[k].forward(0.5 * sqrt(botk_vx * botk_vx + botk_vy * botk_vy));

				// ͬʱָʾ�����Ļ����ˣ����ؽ�k��Ϊ��ײĿ�꣬��������
				for (int m = 0; m < 4; ++m) {
					if (crashId[k][m] != -1) {
						--crashNum[m];
						--crashNum[k];
						crashId[k][m] = -1;
						crashId[m][k] = -1;
					}
				}
				continue;
			}

			// ��ʼ��ײ����
			for (int m = 0; m < 4; ++m) {
				if (crashId[k][m] != -1) {
					botm_vx = botArr[m].lineSpeed[0];
					botm_vy = botArr[m].lineSpeed[1];
					double vk = sqrt(botk_vx * botk_vx + botk_vy * botk_vy);
					double vm = sqrt(botm_vx * botm_vx + botm_vy * botm_vy);
					if (vk > 6)
						vk = 6;
					if (vm > 6)
						vm = 6;

					if (rotate_dir[k][m]) {
						if (rotate_flag[m]) {
							botArr[m].rotate(M_PI);
							botArr[m].forward(vm);
						}
						else {	// ���������ں��ˣ��û����˲����б���
							botArr[m].rotate(botArr[m].angleSpeed);
							botArr[m].forward(-vm);
						}
						if (rotate_flag[k]) {
							botArr[k].rotate(M_PI);
							botArr[m].forward(vk);
						}
						else {	// ���������ں��ˣ��û����˲����б���
							botArr[k].rotate(botArr[k].angleSpeed);
							botArr[m].forward(-vk);
						}
					}
					else {
						if (rotate_flag[m]) {
							botArr[m].rotate(-M_PI);
							botArr[m].forward(vm);
						}
						else {	// ���������ں��ˣ��û����˲����б���
							botArr[m].rotate(botArr[m].angleSpeed);
							botArr[m].forward(-vm);
						}
						if (rotate_flag[k]) {
							botArr[k].rotate(-M_PI);
							botArr[m].forward(vk);
						}
						else {	// ���������ں��ˣ��û����˲����б���
							botArr[k].rotate(botArr[k].angleSpeed);
							botArr[m].forward(-vk);
						}

					}
					crashId[k][m] = -1;
					crashId[m][k] = -1;
					--crashNum[m];
					--crashNum[k];

					//hwcout << "frame " << frameId << hwendl;
					//hwcout << "botidk " << k << hwendl;
					//hwcout << "botidm " << m << hwendl;
					//if (rotate_flag[k])
					//	hwcout << "vk " << vk << hwendl;
					//else
					//	hwcout << "vk " << -vk << hwendl;
					//if (rotate_flag[m])
					//	hwcout << "vm " << vm << hwendl;
					//else
					//	hwcout << "vm " << -vm << hwendl;
					//if (rotate_dir[k][m])
					//	hwcout << "omega: M_PI " << hwendl;
					//else
					//	hwcout << "omega: -M_PI " << hwendl;
				}
			}

		}
	}

	void competeTar(int botId);
	void competeTar_1(int botId);

	/// <summary>
	/// �������·�
	/// </summary>
	virtual void afterReadMap();

	/// <summary>
	/// �������·�
	/// </summary>
	virtual void afterReadFrame(int frameId);
};

void strategy_baiyu_yng1::afterReadMap() {
	initDisV2AndWbTypeArr();
	n1 = wbTypeArr[1].size(), n2 = wbTypeArr[2].size(), n3 = wbTypeArr[3].size();
	n4 = wbTypeArr[4].size(), n5 = wbTypeArr[5].size(), n6 = wbTypeArr[6].size();
	n7 = wbTypeArr[7].size(), n8 = wbTypeArr[8].size(), n9 = wbTypeArr[9].size();
}

void strategy_baiyu_yng1::afterReadFrame(int frameId) {

// todo
	initProductNeed(productNeed);
	
	for (int i = 0; i < 4; ++i) {
		// ������в�Ʒ�������϶����Ի��߽���ĳ������̨ΪĿ�꣬���������ĵ�1����Ʒ����
		if (botArr[i].gootsId > 0 && botArr[i].gootsId < 7) // ������һ����û��update�ɹ���ֻҪ�����˲�Ʒ���ͻ����ĵ�1����Ʒ����
			--productNeed[botArr[i].gootsId];
		// ��������в�Ʒ����֮ǰ�ܳɹ�����״̬����Ŀǰ��ͼȥ��ĳ����Ʒ(����)������Ҳ���ĵ�1����Ʒ����
		if (botArr[i].gootsId == 0 && successUpdate[i]) // ���֮ǰ���ܳɹ�����״̬��˵������������Ʒ���Ҳ�����һ��Ŀ�꣬��botTarget[i]��Ȼ������������Ʒ�Ĺ���̨id
			--productNeed[wbArr[botTarId[i][1]].wbType];
		// ��̬����Ŀ��
		//competeTar(i);
		//competeTar_1(i);
	}

	/*for (int i = 0; i < 8; ++i) {
		hwcout << "product " << i << " need " << int(productNeed[i]) << hwendl;
	}*/

	for (int i = 0; i < 4; ++i) {
		if (i == 0 && frameId < 200) {
			botArr[i].forward(6);

			continue;
		}
			
		

		hwRobot& bot = botArr[i];
		int state = bot.moveUntillArriveTarget_YNG1();
		if (botTarId[i][0] != 0) {		// �����ʱ������i��Ŀ��
			if (state == hwRobot::MOVE_STATE::HW_ARRIVE_TARGET) {	// �������Ŀ��
				if (botTarId[i][0] == 1) {		// ���Ŀǰ�������Ҫ�����Ŀ��
					if (frameId > 9000 - f1_lastN[i])
						continue;
					bot.buy();

					// ��ʱ������������������ֹͣ��������ȡ����Ʒ��Ӧ�ý����������ֶ���������̨��״̬��rpt��materialsState��ifHaveProduct��
					if (wbArr[botTarId[i][1]].getNeed() == 0 && wbArr[botTarId[i][1]].rpt == 0) {
						wbArr[botTarId[i][1]].materialsState = 0;
						if (wbArr[botTarId[i][1]].wbType == 6) {
							++productNeed[3];
							++productNeed[2];
							wbArr[botTarId[i][1]].rpt = 500;
						}
						else if (wbArr[botTarId[i][1]].wbType == 5) {
							++productNeed[3];
							++productNeed[1];
							wbArr[botTarId[i][1]].rpt = 500;
						}
						else if (wbArr[botTarId[i][1]].wbType == 4) {
							++productNeed[2];
							++productNeed[1];
							wbArr[botTarId[i][1]].rpt = 500;
						}
						else if (wbArr[botTarId[i][1]].wbType == 7) {
							++productNeed[6];
							++productNeed[5];
							++productNeed[4];
							wbArr[botTarId[i][1]].rpt = 1000;
						}
					}

					wbArr[botTarId[i][1]].ifHaveProduct = 0;
					wbIfTar[botTarId[i][1]][1] = 0;
					botTarId[i][0] = 2;
					bot.setMoveTarget(wbArr[botTarId[i][2]]);
					bot.moveUntillArriveTarget_YNG1();

					//// ������в�Ʒ�������϶�����ĳ������̨ΪĿ�꣬���������ĵ�1����Ʒ����
					//if (bot.gootsId > 0 && bot.gootsId < 7) // ������һ����û��update�ɹ���ֻҪ�����˲�Ʒ���ͻ����ĵ�1����Ʒ����
					//	--productNeed[botArr[i].gootsId];
				}
				else if (botTarId[i][0] == 2) {	// ���Ŀǰ�������Ҫ�۳���Ŀ��
					bot.sell();
					wbArr[bot.workbenchId].materialsState ^= (1 << bot.gootsId);

					if (wbArr[botTarId[i][2]].getNeed() == 0 && wbArr[botTarId[i][2]].rpt == -1) {  // �����ʱ����̨����Ҫ�κ�ԭ�ϣ��ҿ�������
						wbArr[botTarId[i][2]].materialsState = 0;     // ������������
						if (wbArr[botTarId[i][2]].wbType == 6) {
							++productNeed[3];
							++productNeed[2];
							wbArr[botTarId[i][2]].rpt = 500;
						}
						else if (wbArr[botTarId[i][2]].wbType == 5) {
							++productNeed[3];
							++productNeed[1];
							wbArr[botTarId[i][2]].rpt = 500;
						}
						else if (wbArr[botTarId[i][2]].wbType == 4) {
							++productNeed[2];
							++productNeed[1];
							wbArr[botTarId[i][2]].rpt = 500;
						}
						else if (wbArr[botTarId[i][2]].wbType == 7) {
							++productNeed[6];
							++productNeed[5];
							++productNeed[4];
							wbArr[botTarId[i][2]].rpt = 1000;
						}
					}

					// �ͷŹ���̨��goodID�����
					if (wbArr[bot.workbenchId].wbType != 8 && wbArr[bot.workbenchId].wbType != 9)
						wbIfTar[botTarId[i][2]][0] ^= (1 << bot.gootsId);
					botTarId[i][0] = 0;
					// �ֶ����Ļ�����ӵ�л����״̬
					botArr[i].gootsId = 0;

					if (findNext(i) != 1) {		// ������һ�β��ң��ɹ������
						successUpdate[i] = false;
						--productNeed[wbArr[botTarId[i][1]].wbType];
						bot.setMoveTarget(wbArr[botTarId[i][1]]);
						bot.moveUntillArriveTarget_YNG1();
					}
					else {					// ��������Ҳ���Ŀ�꣬����ʱͣ����
						if (findNext_noLock(i) != 1) {		// ���ж� 123�ĵ�������
							successUpdate[i] = false;
							--productNeed[wbArr[botTarId[i][1]].wbType];
							bot.setMoveTarget(wbArr[botTarId[i][1]]);
							bot.moveUntillArriveTarget_YNG1();
						}
						else {					// ��������Ҳ���Ŀ�꣬����ʱͣ����
							successUpdate[i] = false;

							//bot.forward(0);
						}
					}
				}
			}
		}
		else {							// �����ʱ������iû��Ŀ��
			if (findNext(i) != 1) {		// ������һ�β��ң��ɹ������
				successUpdate[i] = false;
				--productNeed[wbArr[botTarId[i][1]].wbType];
				bot.setMoveTarget(wbArr[botTarId[i][1]]);
				bot.moveUntillArriveTarget_YNG1();
			}
			else {					// ��������Ҳ���Ŀ�꣬����ʱͣ����
				successUpdate[i] = false;
				
				if (findNext_noLock(i) != 1) {		// ���ж� 123�ĵ�������
					successUpdate[i] = false;
					--productNeed[wbArr[botTarId[i][1]].wbType];
					bot.setMoveTarget(wbArr[botTarId[i][1]]);
					bot.moveUntillArriveTarget_YNG1();
				}
				else {					// ��������Ҳ���Ŀ�꣬����ʱͣ����
					successUpdate[i] = false;

					//bot.forward(0);
				}


				//bot.forward(0);
			}
		}
	}

	avoidCrash();

}

void strategy_baiyu_yng1::competeTar_1(int botId) {

	hwRobot& bot = botArr[botId];
	double penalty1 = 1.0;		// �����˵�Ŀ��ĳͷ�����
	double penalty2 = f1_penalty2;		// �ı��Լ���Ŀ��ĳͷ�����
	double penalty3 = f1_penalty3;		// �ı��Լ���Ŀ��ĳͷ�����
	int botIdid1 = botTarId[botId][1];	// botIdbuyĿ��id
	int botIdid2 = botTarId[botId][2];	// botIdsellĿ��id
	int goodId = wbArr[botIdid1].wbType;	// buy�Ļ�������


	if (botTarId[botId][0] == 1) {		// �����ʱ������botId��ǰ�������buyĿ��
		// ���ﲻ��Ҫ���productNeed����ΪbotId��һ�θ���ʱ���Ѿ�֪����goodId����Ҫ��
		/*double nowWeight = getWeight(botId, botIdid1, botIdid2, goodId) + getWeightOffset(botId,botIdid1,botIdid2,goodId);
		int wbId1, wbId2;
		double maxWeight;
		findNext1_dontSetTar(botId, wbId1, wbId2, maxWeight);
		if (maxWeight > nowWeight) {
			botTarId[botId][1] = wbId1;
			botTarId[botId][2] = wbId2;
			wbIfTar[botIdid1][1] = 0;
			wbIfTar[botIdid2][0] &= ~(1 << goodId);
			wbIfTar[wbId1][1] = 1;
			if(wbArr[wbId2].wbType < 8)
				wbIfTar[wbId2][0] |= (1 << wbArr[botTarId[botId][1]].wbType);	
			bot.setMoveTarget(wbArr[botTarId[botId][1]]);
			bot.moveUntillArriveTarget_YNG1();
		}*/


	}

	else if (botTarId[botId][0] == 2) {	// �����ʱ������botId��ǰ�������sellĿ��
		//double nowWeight = getWeight2(botId, botIdid2, goodId);
		//int wbId2 = -1;
		//double maxWeight = 0;
		//findNext3(botId, goodId, wbId2, maxWeight);
		//if (maxWeight > nowWeight) {
		//	botTarId[botId][2] = wbId2;
		//	/*hwcout << "nowWeight: " << nowWeight << hwendl;
		//	hwcout << "maxWeight: " << maxWeight << hwendl;*/
		//	wbIfTar[botIdid2][0] &= ~(1 << goodId);	
		//	if (wbArr[wbId2].wbType < 8)
		//		wbIfTar[wbId2][0] |= (1 << goodId);
		//	bot.setMoveTarget(wbArr[botTarId[botId][2]]);
		//	bot.moveUntillArriveTarget_YNG1();
		//}
	}
}

void strategy_baiyu_yng1::competeTar(int botId) {

	hwRobot& bot = botArr[botId];
	double penalty1 = 1.0;		// �����˵�Ŀ��ĳͷ�����
	double penalty2 = f1_penalty2;		// �ı��Լ���Ŀ��ĳͷ�����
	double penalty3 = f1_penalty3;		// �ı��Լ���Ŀ��ĳͷ�����
	int botIdid1 = botTarId[botId][1];	// botIdbuyĿ��id
	int botIdid2 = botTarId[botId][2];	// botIdsellĿ��id
	int goodId = wbArr[botIdid1].wbType;	// buy�Ļ�������


	if (botTarId[botId][0] == 1) {		// �����ʱ������botId��ǰ�������buyĿ��
		// ���ﲻ��Ҫ���productNeed����ΪbotId��һ�θ���ʱ���Ѿ�֪����goodId����Ҫ��

		bool robFlag = false;	// ��ʶ�����˵�Ŀ���Ƿ����ɹ���ֻ����Ļ����˵�buy��
		double botIdLastWeight = getWeight(botId, botIdid1, botIdid2, goodId);  // δ����Ŀ��ʱ��Ȩֵ
		double botIdCurWeight = botIdLastWeight;
		int newTarId = findBestWbBuy(botId, goodId, botIdCurWeight);

		if (robFlag == false) {		// ��������˵�Ŀ�겻�ɹ�������ͼ��û�л�����������buy����̨���Ҹ��º���С׬

			double x = botArr[botId].x, y = botArr[botId].y;
			double minDist = 1000.0;
			int tarId = -1;
			for (int i = 0; i < wbTypeArr[goodId].size(); ++i) {
				int id = wbTypeArr[goodId][i];
				if (wbArr[id].ifHaveProduct && !wbIfTar[id][1]) {    // �л����δ���������Լ���Ŀ���Ѿ����Լ������ˣ������л��ᱻѡ��
					double dist = calDist(x, y, wbArr[id].x, wbArr[id].y);
					if (dist < minDist) {
						tarId = id;
						minDist = dist;
					}
				}
			}
			if (tarId != -1) {

				double botIdCurWeight = getWeight(botId, tarId, botIdid2, goodId);
				if (botIdCurWeight <= penalty2 * botIdLastWeight)
					return;

				// �ͷ�ԭ���Ĺ���̨�������
				wbIfTar[botIdid1][1] = 0;
				// ������Ŀ�깤��̨�������
				wbIfTar[tarId][1] = 1;

				// ���»����˵�Ŀ�깤��̨
				botTarId[botId] = { 1,tarId,botIdid2 };

				// ���»������ƶ���Ŀ��
				botArr[botId].setMoveTarget(wbArr[botTarId[botId][1]]);
				botArr[botId].moveUntillArriveTarget_YNG1();

				//hwcout << "frame " << frameId << hwendl;
				//hwcout << "botId " << botId << hwendl;
				//hwcout << "oldTarId " << botIdid1 << hwendl;
				//hwcout << "botIdLastWeight " << botIdLastWeight << hwendl;
				//hwcout << "newTarId " << tarId << hwendl;
				//hwcout << "botIdCurWeight " << botIdCurWeight << hwendl;
				//hwcout << "goodId " << goodId << hwendl;
			}
		}
	}

	else if (botTarId[botId][0] == 2) {	// �����ʱ������botId��ǰ�������sellĿ��

		double botIdLastWeight = getWeight2(botId, botIdid2, goodId);  // δ����Ŀ��ʱ��Ȩֵ
		double botIdCurWeight = botIdLastWeight;
		int tarId = -1;
		findNext3(botId, goodId, tarId, botIdCurWeight);	// �϶������Լ����Լ���Ŀ�꣬��Ϊ�Լ��Ѿ����Լ���Ŀ������ס��
		if (tarId != -1) {

			if (botIdCurWeight <= penalty3 * botIdLastWeight)
			//if (botIdCurWeight <= 1.00 * botIdLastWeight)
				return;

			// �ͷ�ԭ���Ĺ���̨�������
			wbIfTar[botIdid2][0] ^= (1 << goodId);
			// ������Ŀ�깤��̨�������
			if (wbArr[tarId].wbType != 8 && wbArr[tarId].wbType != 9)
				wbIfTar[tarId][0] = (wbIfTar[tarId][0] | (1 << goodId));
			botTarId[botId][2] = tarId;
			// ���»������ƶ���Ŀ��
			botArr[botId].setMoveTarget(wbArr[tarId]);
			botArr[botId].moveUntillArriveTarget_YNG1();
		}
	}
}
