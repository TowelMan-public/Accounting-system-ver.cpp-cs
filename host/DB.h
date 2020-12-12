#pragma once
#include "TString.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string>
#include <system_error>
#include <vector>
#include <codecvt>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn/Statement.h>
#include <cppconn/ResultSet.h>
///////////�f�[�^�x�[�X�ɂ��Ă̒�`/////////////
const std::string DB_USER_NAME = "root";
const std::string DB_USER_PASSWORD = "root";

namespace DB {
	class DB {
	protected:
		std::string sqlStr;
		std::string columnType;
	protected:
		void RunSql(void);
		std::string GetResultStr(void);
	public:
		DB(void);
		void Conect(std::string name = DB_USER_NAME, std::string password = DB_USER_PASSWORD);//���[�g�A�J�E���g�̃��O�C���i��������Ȃ��Ǝ��s����j
		void CutEnd(void);//���O�A�E�g�E�{�v���O�������I������Ƃ��ɕK���Ăяo������
		bool GetPassword(std::string userName, std::string password);
	};

	//����
	class Select : public DB {
	public:
		std::string ExpensesItem(int itemID, int allStert, int allFinish, bool isEffectiveness);
		std::string Expenses(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string Customer(int itemID, int allStert, int allFinish, bool isEffectiveness);
		std::string Sales(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string Paid(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string Unpaid(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string ConsumptionTax(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string IncomeTax(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
		std::string Income(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem);
	};

	//�o�^
	class Insert : public DB {
	public:
		void Customer(std::string name);
		void ExpensesItem(std::string name, double rate);
		void Expenses(std::string date, int item, int money);
		void Unpaid(int id, int money);
		void Sales(std::string date, int customer, int money, std::string person, std::string subject);
	};
}