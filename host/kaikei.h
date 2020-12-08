#pragma once
#include <map>
#include <iostream>
#include <string>
#include <regex>
#include "Hnet.h"
#include "TString.h"
#include "DB.h"

#define KAIKEI_1_UPDARE_NONE
#define KAIKEI_1_DELETE_NONE

class Kaikei {
private://�����o�ϐ�
	MyTools::Hnet* hnet;//�z�X�g���̒ʐM
	std::map<std::string, std::string> LoginMap;
	std::string ipAddress;

private://�����o�֐�
	std::string GetRequest(void);//���N�G�X�g�̎擾��
	void DoRequest(std::string meg);//���N�G�X�g�̎��s
	bool IsLogin(void);//���O�C������Ă邩
	/*���b�Z�[�W�����i���O�C���j*/
	void Login(std::string meg);
	/*���b�Z�[�W�����i���������j*/
	void Select(std::string meg);
	/*���b�Z�[�W�����i�o�^�����j*/
	void Insert(std::string meg);
	void InsertCustomer(std::string meg);
	void InsertExpensesItem(std::string meg);
	void InsertExpenses(std::string meg);
	void InsertUnpaid(std::string meg);
	void InsertSales(std::string meg);
	/*���b�Z�[�W�����i�X�V�����j*/
#ifndef KAIKEI_1_UPDARE_NONE
	void Update(std::string meg);
	void UpdateCustomer(std::string meg);
	void UpdateExpensesItem(std::string meg);
	void UpdateExpenses(std::string meg);
	void UpdateUnpaid(std::string meg);
	void UpdateSales(std::string meg);
#endif
	/*���b�Z�[�W�����i�폜�����j*/
#ifndef KAIKEI_1_DELETE_NONE
	void Delete(std::string meg);
	void DeleteCustomer(std::string meg);
	void DeleteExpensesItem(std::string meg);
	void DeleteExpenses(std::string meg);
	void DeleteUnpaid(std::string meg);
	void DeleteSales(std::string meg);
#endif
public:
	Kaikei();//������
	~Kaikei();//�I������
	void Run(void);//�G���g���[�|�C���g
};