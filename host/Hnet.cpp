#include "Hnet.h"
#pragma comment(lib,"ws2_32.lib")

namespace MyTools {

	std::string wide_to_utf8_cppapi(const std::wstring& wstr) {
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	Hnet::Hnet(bool IsInit, int timeout, int portnum) {
		portNumber = portnum;
		timeOut = timeout;
		ret_code = 0;
		FD_accept = -1;//�ڑ��󂯗p��FD
		FD_GorS = -1;//���ۂɎ�M�E���M������FD
		//flag = 0;//�����g��Ȃ�
		HaveInit = false;
		if (IsInit)
			this->Init();
	}

	bool Hnet::Init() {
		if (HaveInit)return true;
		HaveInit = true;
		const char timeo[] = "1000";//�^�C���A�E�g�̒l�i�~���b�j
		//�\�P�b�g�A�h���X�\���̂�������
		memset(&ssin, 0, sizeof(ssin));
		memset(&sin_clim, 0, sizeof(sin_clim));
		//�T�C�Y�擾
		socklen = sizeof(sin_clim);
		//�\�P�b�g�X�g���[���̂��߂̏�����
		int result = WSAStartup(MAKEWORD(2, 0), &wsaData);
		//TCP�\�P�b�g�̍쐬
		FD_accept = socket(AF_INET, SOCK_STREAM, 0);
		if (FD_accept == -1)return false;
		//�\�P�b�g�̐ݒ������
		ssin.sin_family = AF_INET;
		ssin.sin_addr.s_addr = INADDR_ANY;
		ssin.sin_port = htons(portNumber);
		//FD_accept�\�P�b�g�̐ݒ������i���f�j
		ret_code = bind(FD_accept, (const sockaddr*)&ssin, sizeof(ssin));
		if (ret_code == -1) {
			closesocket(FD_accept);
			return false;
		}
		//�^�C���A�E�g�̐ݒ�
		//setsockopt(FD_accept, SOL_SOCKET, SO_SNDTIMEO, timeo, sizeof(timeo));
		//�\�P�b�g�ɒʐM�҂���ݒ肷�� 5�̓o�b�N���O
		ret_code = listen(FD_accept, 5);
		if (ret_code == -1) {
			closesocket(FD_accept);
			return false;
		}

		return true;
	}

	bool Hnet::Accept(TString& pass) {
		u_long val;//�u���b�L���O�̂��߂̕ϐ�
		//�u���b�L���O���O��
		///val = 1;
		//ioctlsocket(FD_accept, FIONBIO, &val);
		//�ڑ��̎�t
		//FD_GorS = accept(FD_accept, (sockaddr*)&sin_clim, &socklen);
		FD_GorS = accept(FD_accept, (sockaddr*)&sin_clim, &socklen);
		//�u���b�L���O�ɂ���
		//val = 0;
		//ioctlsocket(FD_accept, FIONBIO, &val);
		//�ʐM�v�����Ȃ���ΏI��
		if (FD_GorS == -1)
			return false;
		//IP�A�h���X�̎擾
#pragma warning(suppress : 4996) 
		pass = inet_ntoa(sin_clim.sin_addr);
		return true;
	}

	bool Hnet::Send(TString str) {
		//TODO �G���[����
		//�f�[�^�T�C�Y�̑��M
		this->m_Send(std::to_string(str.length()));
		this->m_Get();
		//�f�[�^�̑��M
		this->m_Send(str);
		this->m_Get();
		return true;
	}

	TString Hnet::Get() {
		//TODO �G���[����
		TString num, date;
		int size;
		//�f�[�^�T�C�Y�̎擾
		num = this->m_Get(4);
		this->m_Send(NetMeg::ANS_SUCCESS);
		size = atoi(((std::string)num).c_str());
		//�f�[�^�̎擾
		date = this->m_Get(size);
		this->m_Send(NetMeg::ANS_SUCCESS);
		return date;
	}

	TString Hnet::m_Get(int size) {
		char* baf = new char[size];//��M�p�o�b�t�@�[	
		int s;
		//��M
		s = recv(FD_GorS, baf, size, NULL);
		int error = WSAGetLastError();
		std::string str = baf;
		if (s == -1 || str.substr(0, s) == "")return NetMeg::ANS_ERROR;
		delete[] baf;
		return MyTools::UTF8toSJIS(str.substr(0, s));
	}

	bool Hnet::m_Send(TString date) {
		const int size = date.length();//���񑗂�f�[�^�̃T�C�Y
		std::string strCheck = ((std::string)(MyTools::TString)date).c_str();
		//���M
		if (send(FD_GorS, ((std::string)(MyTools::TString)date).c_str(), size, 0) >= size)
			return true;//����
		else return false;//���s
	}

	bool Hnet::CutEnd() {
		closesocket(FD_GorS);
		FD_GorS = -1;
		return true;
	}
}