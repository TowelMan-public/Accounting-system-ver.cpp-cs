#include "kaikei.h"

Kaikei::Kaikei() {//初期化
	DB::DB db;
	db.Conect();//データベース接続
	hnet = new MyTools::Hnet(true);
}

Kaikei::~Kaikei() {//終了処理
	DB::DB db;
	db.CutEnd();//データベース接続の切断
	delete hnet;
}

void Kaikei::Run(void) {//エントリーポイント
	try {
		std::string meg;
		std::string bafIpAddress;

		while (true) {

			//接続要求を出すやつを受け入れ
			if (hnet->Accept(bafIpAddress)) {
				ipAddress = bafIpAddress;
				meg = GetRequest();//リクエストの取得等

				//有効なリクエストならリクエストを実行
				if (meg.length() > 0)
					DoRequest(meg);
			}
		}
	}
	catch (...) {//例外発生・終了する
		hnet->CutEnd();
	}
}

bool Kaikei::IsLogin() {
	auto itr = LoginMap.find(ipAddress);
	return itr != LoginMap.end();
}

std::string Kaikei::GetRequest() {//リクエストの取得等
	std::string meg;
	//メッセージ取得
	meg = hnet->Get();
	//正常なやつ
	if (IsLogin() || meg.find("LOGIN") != -1) {
		return meg;
	}
	else {//拒否るやつ
		hnet->CutEnd();
		return "";
	}
}

void Kaikei::DoRequest(std::string meg) {//リクエストの実行
	if (meg.find("SELECT") != -1)
		Select(meg);
	else if (meg.find("INSERT") != -1)
		Insert(meg);
	else if (meg.find("LOGIN") != -1)
		Login(meg);
#ifndef KAIKEI_1_UPDARE_NONE
	else if (meg.find("UPDATE") != -1)
		Update(meg);
#endif
#ifndef KAIKEI_1_DELETE_NONE
	else if (meg.find("DELETE") != -1)
		Delete(meg);
#endif
	else//不正
		hnet->CutEnd();
}

void Kaikei::Login(std::string meg) {
	DB::DB db;
	std::string userName;
	std::string password;
	std::smatch results;
	//ユーザー名取得
	if (!std::regex_match(meg, results, std::regex(R"(LOGIN\((.*),(.*)\))"))) {
		hnet->CutEnd();
		return;
	}
	userName = results[1].str();
	password = results[2].str();
	//判定
	if (db.GetPassword(userName, password)) {//ログイン成功
		LoginMap[ipAddress] = userName;
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->CutEnd();
	}
	else//失敗
		hnet->CutEnd();

}

void Kaikei::Select(std::string meg) {
	DB::Select db;
	std::smatch results;
	std::string sendMeg;
	int itemID = -2;
	int moom = -2;
	int allStert = -2;
	int allFinish = -2;
	int year = -2;
	bool isGroup = false;
	bool isEffectiveness = false;
	bool isItem = false;
	bool isMoon = false;
	bool isYear = false;
	/*メッセージの取得*/
	if (std::regex_match(meg, results, std::regex(R"(.*\(([0-9]*),([0-9]*),([0-9]*),([0-9]*),([0-9]*),.*\))"))) {

		try {
			itemID = stoi(results[1].str());
		}
		catch (...) {
			itemID = -1;
		}
		try {
			moom = stoi(results[2].str());
			if (moom < 1 || moom>12)
				moom = -1;
		}
		catch (...) {
			moom = -1;
		}
		try {
			allStert = stoi(results[3].str());
		}
		catch (...) {
			allStert = 0;
		}
		try {
			allFinish = stoi(results[4].str());
		}
		catch (...) {
			allFinish = -1;
		}
		try {
			year = stoi(results[5].str());
		}
		catch (...) {
			year = -1;
		}
	}
	//グループ化
	if (meg.find("GROUP") != -1) {
		isGroup = true;
		if (meg.find("KOUMOKU") != -1)
			isItem = true;
		if (meg.find("MOOM"))
			isMoon = true;;
		if (meg.find("YERA"))
			isYear = true;
	}
	//有効なもののみ
	if (meg.find("TRUE") != -1)
		isEffectiveness = true;
	/*振り分け*/
	if (meg.find("KEIHI_KOU") != -1) {
		sendMeg = db.ExpensesItem(itemID, allStert, allFinish, isEffectiveness);
		hnet->Send(sendMeg);
	}
	else if (meg.find("KEIHI") != -1) {
		sendMeg = db.Expenses(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("KAISYA") != -1) {
		sendMeg = db.Customer(itemID, allStert, allFinish, isEffectiveness);
		hnet->Send(sendMeg);
	}
	else if (meg.find("KAKEURI") != -1) {
		sendMeg = db.Sales(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("PAYD") != -1) {
		sendMeg = db.Paid(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("UNPAYD") != -1) {
		sendMeg = db.Unpaid(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("SYOUHI_TAX") != -1) {
		sendMeg = db.ConsumptionTax(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("SYOTOKU_TAX") != -1) {
		sendMeg = db.IncomeTax(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else if (meg.find("SYOTOKU") != -1) {
		sendMeg = db.Income(isGroup, itemID, moom, allStert, allFinish, year, isEffectiveness, isMoon, isYear, isItem);
		hnet->Send(sendMeg);
	}
	else {//不正
		hnet->CutEnd();
		return;
	}
	hnet->Get();
	hnet->CutEnd();
}

void Kaikei::Insert(std::string meg) {
	if (meg.find("CUSTOMER") != -1)
		InsertCustomer(meg);
	else if (meg.find("JUNL") != -1)
		InsertExpensesItem(meg);
	else if (meg.find("KEIHI") != -1)
		InsertExpenses(meg);
	else if (meg.find("UNPAID") != -1)
		InsertUnpaid(meg);
	else if (meg.find("KAKEURI") != -1)
		InsertSales(meg);
	else//不正
		hnet->CutEnd();
}

void Kaikei::InsertCustomer(std::string meg) {
	DB::Insert db;
	std::string name;
	std::smatch results;
	//TODO メッセージの取得
	if (std::regex_match(meg, results, std::regex(R"(INSERT_CUSTOMER\((.*)\))"))) {
		name = results[1].str();
		db.Customer(name);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

void Kaikei::InsertExpensesItem(std::string meg) {
	DB::Insert db;
	std::smatch results;
	std::string name;
	double rate;
	//TODO メッセージの取得
	if (std::regex_match(meg, results, std::regex(R"(INSERT_JUNL\((.*),([0-1]\.{0,1}[0-9]*)\))"))) {
		name = results[1].str();
		try {
			rate = stof(results[2].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.ExpensesItem(name, rate);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

void Kaikei::InsertExpenses(std::string meg) {
	std::smatch results;
	DB::Insert db;
	std::string date;
	int item;
	int money;
	//TODO メッセージの取得
	//
	if (std::regex_match(meg, results, std::regex(R"(INSERT_KEIHI\(([0-9]{4}-[0-9]{1,2}-[0-9]{1,2}),([0-9]*),([0-9]*)\))"))) {
		date = results[1].str();
		try {
			item = stoi(results[2].str());
			money = stoi(results[3].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Expenses(date, item, money);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

void Kaikei::InsertUnpaid(std::string meg) {
	DB::Insert db;
	std::smatch results;
	int id;
	int money;
	//TODO メッセージの取得
	if (std::regex_match(meg, results, std::regex(R"(INSERT_PAID\(([0-9]*),([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
			money = stoi(results[2].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Unpaid(id, money);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

void Kaikei::InsertSales(std::string meg) {
	DB::Insert db;
	std::smatch results;
	std::string date;
	int customer;
	int money;
	std::string person;
	std::string subject;
	//TODO メッセージの取得
	if (std::regex_match(meg, results, std::regex(R"(INSERT_KAKEURI\(([0-9]{4}-[0-9]{1,2}-[0-9]{1,2}),([0-9]*),([0-9]*),(.*),(.*)\))"))) {
		try {
			date = results[1].str();
			customer = stoi(results[2].str());
			money = stoi(results[3].str());
			person = results[4].str();
			subject = results[5].str();
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Sales(date, customer, money, person, subject);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

#ifndef KAIKEI_1_UPDARE_NONE
void Update(std::string meg) {
	if (meg.find("UPDATE_CUSTOMER") != -1)
		UpdateCustomer(meg);
	else if (meg.find("UPDATE_JUNL") != -1)
		UpdateExpensesItem(meg);
	else if (meg.find("UPDATE_KEIHI") != -1)
		UpdateExpenses(meg);
	else if (meg.find("UPDATE_PAID") != -1)
		UpdateUnpaid(meg);
	else if (meg.find("UPDATE_KAKEURI") != -1)
		UpdateSales(meg);
	else//不正
		hnet->CutEnd();
}

#endif

#ifndef KAIKEI_1_DELETE_NONE
void Delete(std::string meg) {
	if (meg.find("DELETE_CUSTOMER") != -1)
		DeleteCustomer(meg);
	else if (meg.find("DELETE_JUNL") != -1)
		DeleteExpensesItem(meg);
	else if (meg.find("DELETE_KEIHI") != -1)
		DeleteExpenses(meg);
	else if (meg.find("DELETE_UNPAID") != -1)
		DeleteUnpaid(meg);
	else if (meg.find("DELETE_KAKEURI") != -1)
		DeleteSales(meg);
	else//不正
		hnet->CutEnd();
}

#endif