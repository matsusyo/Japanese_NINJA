#pragma once
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

//	�X�R�A�N���X
//	�Q�[���N���A�̃��U���g�f�[�^���Ǘ�����

const int SCORE_STRLENGTH = 256;
const float TIME_LIMIT = 180.0f;		//	��������

class Time_Manager {

	bool Flg;						//	�J�����g�v���C���[�̂��邵
	char Score_Str[SCORE_STRLENGTH];	//	�X�R�A����s�̕�����ɂ܂Ƃ߂�
	char Player_Name[100];			//	�v���C���[��
	time_t Date;					//	�N���A�������̓��t���L�^����
	float Clear_Time;				//	�o�ߎ��Ԃ�������
	float Time_Limit = TIME_LIMIT;	//	�c�萧�����Ԃ���


public:

	// �R���X�g���N�^
	Time_Manager() {		// �����L���O�Ŏg�p
		Score_Str[0] = NULL;	// �X�R�A�p�̕�����
		Player_Name[0] = NULL;	// �v���C���[��
		Date = 0;				// �N���A���̓��t
		Clear_Time = 0;			//	�N���A���ԏ�����
		Time_Limit = TIME_LIMIT;//	�������ԃZ�b�g
	}

	~Time_Manager() {

	}

	// �X�R�A�̃N���A
	void Clear() {
		Score_Str[0] = NULL;	// �X�R�A�p�̕�����
		Player_Name[0] = NULL;	// �v���C���[��
		Date = 0;				// �N���A���̓��t
		Clear_Time = 0;			//	�N���A���ԏ�����
		Time_Limit = TIME_LIMIT;//	�������ԃZ�b�g
	}

	// �X�R�A����s�̕�����ɂ܂Ƃ߂ĕԂ�
	void Get_Total_Score_Str(char *arg){
		struct tm Tms;	// ���Ԃ̍\���̂ւ̃|�C���^
		localtime_s(&Tms, &Date);	// �L�^�^�C����������f�[�^���\���̂֎��o��
		char TimeStmpBuf[100];		// �����p�̕����f�[�^�̊m��
		sprintf_s(TimeStmpBuf, 100, "%02d/%02d/%02d %02d:%02d:%02d", Tms.tm_year+1900, Tms.tm_mon+1, Tms.tm_mday, Tms.tm_hour, Tms.tm_min, Tms.tm_sec);
		sprintf_s(Score_Str, 256, "%10s  %09f  %s", Player_Name, Clear_Time, TimeStmpBuf);
		strcpy_s(arg, 256, Score_Str);	// �Z�b�g
	}

	// �A�N�Z�X���\�b�h
	void Set_Flg(bool arg) { Flg = arg; }	// �J�����g�v���C���[�̂��邵���Z�b�g
	bool Is_Flg() { return Flg; }	// �J�����g�v���C���[�̂��邵���Q�b�g

	void SetClear_Time() { Clear_Time = (TIME_LIMIT - Time_Limit); }
	void Reset_Clear_Time() { Clear_Time = NULL; }

	float Get_Clear_Time() { return Clear_Time; }
	
	void Set_Time_Limit() { Time_Limit = TIME_LIMIT - Clear_Time; }
	float Get_Time_Limit() { return Time_Limit; }
	void Add_Time_Limit(float arg) { Time_Limit += arg; }

	void Set_Date() { Date = time(NULL); }		// ���̓��t���Z�b�g

	void Set_Player_Name(char* arg) { strcpy_s(Player_Name, 100, arg); }	// �v���C���[���̃Z�b�g
};