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
private://メンバ変数
	MyTools::Hnet* hnet;//ホスト側の通信
	std::map<std::string, std::string> LoginMap;
	std::string ipAddress;

private://メンバ関数
	std::string GetRequest(void);//リクエストの取得等
	void DoRequest(std::string meg);//リクエストの実行
	bool IsLogin(void);//ログインされてるか
	/*メッセージ処理（ログイン）*/
	void Login(std::string meg);
	/*メッセージ処理（検索処理）*/
	void Select(std::string meg);
	/*メッセージ処理（登録処理）*/
	void Insert(std::string meg);
	void InsertCustomer(std::string meg);
	void InsertExpensesItem(std::string meg);
	void InsertExpenses(std::string meg);
	void InsertUnpaid(std::string meg);
	void InsertSales(std::string meg);
	/*メッセージ処理（更新処理）*/
#ifndef KAIKEI_1_UPDARE_NONE
	void Update(std::string meg);
	void UpdateCustomer(std::string meg);
	void UpdateExpensesItem(std::string meg);
	void UpdateExpenses(std::string meg);
	void UpdateUnpaid(std::string meg);
	void UpdateSales(std::string meg);
#endif
	/*メッセージ処理（削除処理）*/
#ifndef KAIKEI_1_DELETE_NONE
	void Delete(std::string meg);
	void DeleteCustomer(std::string meg);
	void DeleteExpensesItem(std::string meg);
	void DeleteExpenses(std::string meg);
	void DeleteUnpaid(std::string meg);
	void DeleteSales(std::string meg);
#endif
public:
	Kaikei();//初期化
	~Kaikei();//終了処理
	void Run(void);//エントリーポイント
};