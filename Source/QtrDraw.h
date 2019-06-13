#pragma once
#include "DxLib.h"
#include<vector>
#include<iostream>
#include<math.h>
#include"Literal.h"
#include"QtrCalc.h"

using namespace std;

const enum OBJECT_NAME {
	PLAYER, ENEMY, MAP, DART, BOMB, CLONE
};

class Qtr_Draw {
private:
	float Qtr_X, Qtr_Y;
	float H_Size_X, H_Size_Y;
	int Type, Graph;

public:
	Qtr_Draw() {}
	//	�N�H�[�^��ʏ�ł̍��W�ƃI�u�W�F�N�g�̎�ނƃO���t�B�b�N���󂯎��
	Qtr_Draw(float qtrx, float qtry,float hsizex,float hsizey, int type, int graph) {
		Qtr_X = qtrx;
		Qtr_Y = qtry;
		H_Size_X = hsizex; 
		H_Size_Y = hsizey;
		Type = type;
		Graph = graph;
	}

	//	�N�H�[�^��ʏ�ł̍��W�ƃI�u�W�F�N�g�̎�ނƃO���t�B�b�N���󂯎��
	Qtr_Draw(float qtrx, float qtry, int type, int graph) {
		Qtr_X = qtrx; 
		Qtr_Y = qtry;
		Type = type;
		Graph = graph;
	}

	//	�I�u�W�F�N�g��`��
	void Draw() {
		int left, top, right, bottom;
		//	����ȕ`�悪�����case�ɕ����ċL�q
		switch (Type) {
		case PLAYER:
		case ENEMY:
		case MAP:
		case BOMB:
		default:
			left = (int)(Qtr_X - H_Size_X);
			top = (int)(Qtr_Y - H_Size_Y);
			right = (int)(Qtr_X + H_Size_X);
			bottom = (int)(Qtr_Y + H_Size_Y);
			DrawExtendGraph(left, top, right, bottom, Graph, TRUE);	//	�`��
			break;
		case DART:
			//	�藠���͉�]���Ă���悤�ɂ݂���
			double angle = DX_PI / 180;
			angle *= rand() % 180;
			DrawRotaGraph2((int)Qtr_X, (int)Qtr_Y, 12, 12, 1.0f, angle, Graph, TRUE, TRUE);
			break;
		}
	}

	float Get_Qtr_X() { return Qtr_X; }
	float Get_Qtr_Y() { return Qtr_Y; }
	int Get_Type() { return Type; }
	int GetGraph() { return Graph; }
};

class Qtr_Draw_Manager {
private:
	//	�`��e�[�u�����t���[���o�^����鐔���Ⴄ�̂�vector�ō��
	vector<Qtr_Draw*> Qtr_Draw_Tbl;
public:
	Qtr_Draw_Manager() {}

	//	�`��e�[�u���ɓo�^���Ă��镨�����ׂĕ`��
	void Draw() {
		for (int i = 0; i < (int)Qtr_Draw_Tbl.size() - 1; i++) {
			Qtr_Draw_Tbl[i]->Draw();
		}
	}

	//	�e�[�u���ɓo�^������
	void Register(float qtrx, float qtry, float hsizex, float hsizey, int type, int gr) {
		//	�e�[�u���̍Ō���ɓo�^
		Qtr_Draw_Tbl.push_back(new Qtr_Draw(qtrx, qtry, hsizex, hsizey, type, gr));
	}

		//	�e�[�u�������ׂċ�ɂ���
	void Clear_Table(){
		Qtr_Draw_Tbl.erase(Qtr_Draw_Tbl.begin(), Qtr_Draw_Tbl.end());
		Qtr_Draw_Tbl.shrink_to_fit();
	}

	// ��{�}���@�ɂ��\�[�g
	void insertionSort() {

		Qtr_Draw *Tmp;
		for (int j = 0; j < (int)Qtr_Draw_Tbl.size() - 1; j++) {	//	�O�����[�v
			for (int i = j; i >= 0; i--) {	//	�������[�v
				// Y���W���������Ƀ\�[�g����
				if (Qtr_Draw_Tbl[i]->Get_Qtr_Y() > Qtr_Draw_Tbl[i + 1]->Get_Qtr_Y()) {
					//	�X���b�v
					Tmp = Qtr_Draw_Tbl[i];
					Qtr_Draw_Tbl[i] = Qtr_Draw_Tbl[i + 1];
					Qtr_Draw_Tbl[i + 1] = Tmp;
				}
				else break;	// �X���b�v�̕K�v���Ȃ��Ȃ������_�œ����̃��[�v�𔲂���
			}
		}
	}
};
