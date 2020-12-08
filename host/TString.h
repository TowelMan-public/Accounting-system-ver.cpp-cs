#pragma once
#include <string>
#include <atlstr.h>

//�����ł̂ݎg���V���{����`�@���݂̕����R�[�h�ɑΉ�����N���X�Ƃ��̔��΂̃N���X
//���݂̕����R�[�h�̔��΂̕����^�̃X�C�b�`
#ifdef _UNICODE
	#define MY_STRING_0028 wstring
	#define MY_OTHER_STRING_0028 string
	#define MY_OTHER_CHAR char
#else
	#define MY_STRING_0028 string
	#define MY_OTHER_STRING_0028 wstring
	#define MY_OTHER_CHAR wchar_t
#endif

namespace MyTools {

	//������ϊ�
	std::string WStringToString(std::wstring istr);
	std::wstring StringToWString(std::string istr);

	std::string UTF8toSJIS(const std::string strUTF8);

	//�t�H�[�}�b�g
	template <typename ... Args>
	std::MY_STRING_0028 format(const std::MY_STRING_0028 fmt, Args ... args)
	{
		CString rtn;
		rtn.Format(fmt.c_str(), args...);
		return rtn.GetBuffer();
	}

	class TString : public std::MY_STRING_0028 {
		//�R���X�g���N�^�̌p���i�e�N���X�̂�������p���j
		using std::MY_STRING_0028::MY_STRING_0028;
	public:
		//�ǉ�����R���X�g���N�^
		TString(void);
		TString(std::MY_OTHER_STRING_0028 str);
		TString(MY_OTHER_CHAR ch);
		TString(MY_OTHER_CHAR* str);
		TString(std::MY_STRING_0028 str);
		//�쐬����L���X�g�ϊ�
		operator std::MY_OTHER_STRING_0028() const;
		operator CString() const;
		//�e�N���X���܂߂����Z�q=�̃I�[�o�[���[�h����
		TString operator=(std::MY_STRING_0028 str);
		TString operator=(std::MY_OTHER_STRING_0028 str);
		TString operator=(CString str);
		TString operator=(MY_OTHER_CHAR ch);
		TString operator=(MY_OTHER_CHAR* str);
		//�e���v���[�g�֐�
		template <typename ... Args>
		std::MY_STRING_0028 format(const std::MY_STRING_0028 fmt, Args ... args);
	};
}