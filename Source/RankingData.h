#pragma once
#include "TimeManager.h"
#include"Map.h"

const int RANKING_NUM = 10;	// �����L���O�̐�
						
// �����L���O�N���X
class RankingData {

	// �����L���O�f�[�^�}�X�^
	Time_Manager Ranking_List[RANKING_NUM];	// 10�Ԗڂ͒ǉ��X�R�A�p�Ɏg�p�B�\����0�`9��10�ʂ܂�
	int Select_Stage;

public:

	// �R���X�g���N�^
	RankingData() {
		// �N�����Ƀ��[�h
		Data_Load();
	}

	// �R���X�g���N�^ ���������g��
	RankingData(int stage) {
		Select_Stage = stage;
		// �N�����Ƀ��[�h
		Data_Load();
	}

	// �f�X�g���N�^
	~RankingData() {

		// �I�����ɃZ�[�u
		Data_Save();
	}

	// �f�[�^���[�h
	errno_t Data_Load() {
		FILE *Fp;	// �t�@�C���|�C���^
		errno_t Err;	// �G���[�R�[�h
		switch (Select_Stage)
		{//	�X�e�[�W�ʂ̐�p�����L���O�t�@�C�����J��
		default:
		case AAA:
			Err = fopen_s(&Fp, "Data/Ranking1.txt", "rb");
			break;
		case BBB:
			Err = fopen_s(&Fp, "Data/Ranking2.txt", "rb");
			break;
		case CCC:
			Err = fopen_s(&Fp, "Data/Ranking3.txt", "rb");
			break;
		case DDD:
			Err = fopen_s(&Fp, "Data/Ranking4.txt", "rb");
			break;
		case EEE:
			Err = fopen_s(&Fp, "Data/Ranking5.txt", "rb");
			break;
		case FFF:
			Err = fopen_s(&Fp, "Data/Ranking6.txt", "rb");
			break;
		}
		if (Err != 0) return Err;	// �I�[�v���G���[�Ȃ�N���[�Y���Ȃ��ŏI��
		fread(Ranking_List, sizeof(Time_Manager), RANKING_NUM, Fp);	// �u���b�N���[�h
		fclose(Fp);	// �t�@�C�����N���[�Y
		return Err;	// ���[�h����
	}

	// �f�[�^�Z�[�u
	errno_t Data_Save() {
		// �Z�[�u����
		FILE *Fp;	// �t�@�C���|�C���^
		errno_t Err;	// �G���[�R�[�h
		switch (Select_Stage)
		{//	�X�e�[�W�ʂ̐�p�����L���O�t�@�C�����J��
		default:
		case AAA:
			Err = fopen_s(&Fp, "Data/Ranking1.txt", "wb");
			break;
		case BBB:
			Err = fopen_s(&Fp, "Data/Ranking2.txt", "wb");
			break;
		case CCC:
			Err = fopen_s(&Fp, "Data/Ranking3.txt", "wb");
			break;
		case DDD:
			Err = fopen_s(&Fp, "Data/Ranking4.txt", "wb");
			break;
		case EEE:
			Err = fopen_s(&Fp, "Data/Ranking5.txt", "wb");
			break;
		case FFF:
			Err = fopen_s(&Fp, "Data/Ranking6.txt", "wb");
			break;
		}
		if (Err != 0) return Err;	// �I�[�v���G���[�Ȃ�N���[�Y���Ȃ��ŏI��
		fwrite(Ranking_List, sizeof(Time_Manager), RANKING_NUM,Fp);	// �u���b�N���C�g
		fclose(Fp);	// �t�@�C�����N���[�Y
		return Err;	// �Z�[�u����
	}

	// �����L���O�Ƀ^�C����ǉ�
	void Add_Time(Time_Manager *argscore) {
		Ranking_List[RANKING_NUM - 1] = *argscore;	// �����L���O�e�[�u���̈�ԍŌ�ɒǉ�����
		Ranking_Sort();		// �\�[�g��������
	}

	// �^�C���Ń\�[�g
	void Ranking_Sort() {
			//	�����L���O�̃N���A�^�C����0�b�ȉ���������NULL
		for (int i = 0; i < RANKING_NUM - 1; i++)
			if (Ranking_List[i].Get_Clear_Time() <= 0)
				Ranking_List[i].Reset_Clear_Time();
		
		// �o�u���\�[�g ���������Ƀ\�[�g
		bool Flg = true;
		while (Flg) {	// �X���b�v��������胋�[�v
			Flg = false;	// �t���O���Ƃ��Ă���
			Time_Manager Tmp;
			for (int i = 0; i < RANKING_NUM - 1; i++) {

				//	�X���b�v����^�C����NULL�łȂ�������X���b�v
				if (Ranking_List[i].Get_Clear_Time() > Ranking_List[i + 1].Get_Clear_Time() && (Ranking_List[i + 1].Get_Clear_Time() != NULL)) {
					Tmp = Ranking_List[i];
					Ranking_List[i] = Ranking_List[i + 1];
					Ranking_List[i + 1] = Tmp;
					Flg = true;		// �X���b�v������
				}

				//	�^�C����NULL
				else if (Ranking_List[i].Get_Clear_Time() == NULL && Ranking_List[i + 1].Get_Clear_Time() > 0) {
					Tmp = Ranking_List[i];
					Ranking_List[i] = Ranking_List[i + 1];
					Ranking_List[i + 1] = Tmp;
					Flg = true;		// �X���b�v������
				}
			}
		}
	}

	// �J�����g�t���O�𗎂Ƃ�
	void Set_Flg_Off_All() { for (int i = 0; i < RANKING_NUM; i++) Ranking_List[i].Set_Flg(false); }

	// �w�胉���N�̃t���O�𓾂�
	bool Is_Flg(int arg) { return Ranking_List[arg].Is_Flg(); }

	// �A�N�Z�X���\�b�h
	void Get_String(int arg, char* argstr) {		// �w�菇�ʂ̃X�R�A��������Q�b�g
		char Buf[256];
		Ranking_List[arg].Get_Total_Score_Str(Buf);
		strcpy_s(argstr, 256, Buf);
	}
	float Get_Clear_Time(int arg) { return Ranking_List[arg].Get_Clear_Time();  }	// �w�菇�ʂ̃^�C���̃Q�b�g
};