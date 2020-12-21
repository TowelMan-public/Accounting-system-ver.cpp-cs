#include "DB.h"
#include "Encode.h"
//////データベース関係のlibファイルのロード////////
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn8.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn.lib")
#pragma conment(lib,"lib64\\vs14\\libcrypto.lib")
#pragma conment(lib,"lib64\\vs14\\libssl.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn8.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn8-static.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn-static.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn8.lib")

namespace DB {
	static sql::mysql::MySQL_Driver* driver = nullptr;//データベースへのドライバー
	static sql::Statement* stmt = nullptr;
	static sql::ResultSet* res = nullptr;
	static sql::Connection* con = nullptr;
}

DB::DB::DB(void){
	sqlStr = "";
	columnType = "";
}

void DB::DB::RunSql(void){
	if (sqlStr.find("SELECT") != -1) {
		if (res != nullptr) {
			res->close();
			delete res;
			res = nullptr;
		}
		res = stmt->executeQuery(MyTools::Encode::ShiftJis::UTF8(sqlStr).c_str());
	}
	else
		stmt->execute(MyTools::Encode::ShiftJis::UTF8(sqlStr).c_str());

	sqlStr = "";//初期化
}

std::string DB::DB::GetResultStr(void){
	
	if (res == nullptr || res->rowsCount() < 1) {
		columnType = "";
		return "";
	}
	
	std::string outStr="";
	for (int r = 0; r < res->rowsCount();r++) {
		res->next();
		for (int i = 0; i < columnType.length(); i++) {
			outStr += MyTools::Encode::UTF8::ShiftJis(res->getString(i + 1).c_str()) + ",";
		}
		outStr = outStr.substr(0, outStr.length() - 1) + '\n';
	}
	columnType = "";
	return outStr;	
}

void DB::DB::Conect(std::string name, std::string password) {
	driver = sql::mysql::get_mysql_driver_instance();//ドライバーの生成
	con = driver->connect("localhost", "root", "root");//ルートアカウント（アカウントを登録するためのやつ）に接続
	stmt = con->createStatement();
	stmt->execute("USE kaikei");//目的のデータベース
}

void DB::DB::CutEnd(void){
	if (con != nullptr) {
		con->close();
		delete con;
		con = nullptr;
	}
	if (res != nullptr) {
		res->close();
		delete res;
		res = nullptr;
	}
	if (stmt != nullptr) {
		stmt->close();
		delete stmt;
		stmt = nullptr;
	}
	if (driver != nullptr) {
		driver->threadEnd();
		driver->threadInit();
		delete driver;
		driver = nullptr;
	}
}

bool DB::DB::GetPassword(std::string userName, std::string password) {
	bool result;
	sqlStr = "SELECT pass FROM useri WHERE id='%ls'";
	sqlStr = (std::string)(MyTools::TString) MyTools::format( (MyTools::TString)sqlStr, ( (std::wstring)(MyTools::TString)userName ).c_str() );
	res = stmt->executeQuery(MyTools::Encode::ShiftJis::UTF8(sqlStr).c_str());
	if (res->next())
		result = res->getString(1).c_str() == password;
	else
		result = false;
	sqlStr = "";
	res->close();
	delete res;
	res = nullptr;
	return result;
}

std::string DB::Select::ExpensesItem(int itemID, int allStert, int allFinish, bool isEffectiveness){
	columnType = "isii";
	sqlStr = "SELECT item , name, rate, is_effectiveness FROM expenses_item WHERE 1 ";
	if (itemID >= 0)
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item = %d ", itemID);
	sqlStr+= (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item >= %d ", allStert);
	if(allFinish>=0)
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item <= %d ", allStert);
	if (isEffectiveness)
		sqlStr += "AND effectiveness = 1 ";
	sqlStr += ";";
	RunSql();
	return GetResultStr();
}

std::string DB::Select::Expenses(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem){
	std::string groupC="";
	if (isGroup) {	
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item_id  = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND moon = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND year = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND effectiveness = 1 ";
		//ここからグループ化する
		if (isYear) {
			groupC += "year,";
			columnType += "i";
		}
		if (isMoon) {
			groupC += "moon,";
			columnType += "i";
		}
		if (isItem) {
			groupC += "item,";
			columnType += "s";
		}
		columnType += "i";
		sqlStr = "SELECT " + groupC + " SUM(money) FROM expenses_g WHERE 1 " + sqlStr + " GROUP BY " + groupC.substr(0,groupC.size() - 1);
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
	else {
		columnType = "idsi";
		sqlStr = "SELECT t1.id AS 'number', t1.day AS 'date', t2.name AS 'item', t1.money AS 'money' FROM expenses t1 INNER JOIN expenses_item t2	USING(item)	WHERE 1 ";
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.item = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND t2.effectiveness = 1 ";
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
}

std::string DB::Select::Customer(int itemID, int allStert, int allFinish, bool isEffectiveness){
	columnType = "isi";
	sqlStr = "SELECT item , name, is_effectiveness  FROM customer WHERE 1 ";
	if (itemID >= 0)
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item = %d ", itemID);
	sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item >= %d ", allStert);
	if (allFinish >= 0)
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item <= %d ", allStert);
	if (isEffectiveness)
		sqlStr += "AND effectiveness = 1 ";
	sqlStr += ";";
	RunSql();
	return GetResultStr();
}

std::string DB::Select::Sales(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	std::string groupC = "";
	if (isGroup) {		
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item_id  = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND moon = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND year = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND effectiveness = 1 ";
		//ここからグループ化する
		if (isYear) {
			groupC += "year,";
			columnType += "i";
		}
		if (isMoon) {
			groupC += "moon,";
			columnType += "i";
		}
		if (isItem) {
			groupC += "item,";
			columnType += "s";
		}
		columnType += "i";
		sqlStr = "SELECT " + groupC + " SUM(money)  FROM sales_g WHERE 1 " + sqlStr + " GROUP BY " + groupC.substr(0, groupC.size() - 1);
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
	else {		
		columnType = "idsifss";
		sqlStr = "SELECT t1.sale_id AS 'number', t1.day AS 'date', t2.name AS 'item', t1.money AS 'money', t1.tax AS 'tax', t1.person AS 'person', t1.subject AS 'subject'    FROM sales t1 INNER JOIN customer t2 USING(item) WHERE 1 ";
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.item = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND t2.effectiveness = 1 ";
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
}

std::string DB::Select::Paid(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	std::string groupC = "";
	if (isGroup) {
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item_id  = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND moon = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND year = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND effectiveness = 1 ";
		//ここからグループ化する
		if (isYear) {
			groupC += "year,";
			columnType += "i";
		}
		if (isMoon) {
			groupC += "moon,";
			columnType += "i";
		}
		if (isItem) {
			groupC += "item,";
			columnType += "s";
		}
		columnType += "i";
		sqlStr = "SELECT " + groupC + " SUM(money)  FROM paid_g WHERE 1 " + sqlStr + " GROUP BY " + groupC.substr(0, groupC.size() - 1);
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
	else {
		columnType = "idsifss";
		sqlStr = "SELECT t1.sale_id AS 'number', t1.day AS 'date', t2.name AS 'item', t3.money AS 'money', t1.tax AS 'tax', t1.person AS 'person', t1.subject AS 'subject'    FROM sales t1 INNER JOIN customer t2 USING(item) INNER JOIN paid t3 USING(sale_id) WHERE 1 ";
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.item = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND t2.effectiveness = 1 ";
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
}

std::string DB::Select::Unpaid(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	std::string groupC = "";
	if (isGroup) {		
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item_id  = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND moon = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND year = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND effectiveness = 1 ";
		//ここからグループ化する
		if (isYear) {
			groupC += "year,";
			columnType += "i";
		}
		if (isMoon) {
			groupC += "moon,";
			columnType += "i";
		}
		if (isItem) {
			groupC += "item,";
			columnType += "s";
		}
		columnType += "i";
		sqlStr = "SELECT " + groupC + " SUM(money)  FROM unpaid_g WHERE 1 " + sqlStr + " GROUP BY " + groupC.substr(0, groupC.size() - 1);
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
	else {		
		columnType = "idsifss";
		sqlStr = "SELECT t1.sale_id AS 'number', t1.day AS 'date', t2.name AS 'item', (t1.money - t3.money) AS 'money', t1.tax AS 'tax', t1.person AS 'person', t1.subject AS 'subject'    FROM sales t1	INNER JOIN customer t2 USING(item) INNER JOIN paid t3 USING(sale_id) WHERE 1 ";
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.item = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND t2.effectiveness = 1 ";
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
}

std::string DB::Select::ConsumptionTax(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	std::string groupC = "";
	if (isGroup) {		
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND item_id  = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND moon = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND year = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND effectiveness = 1 ";
		//ここからグループ化する
		if (isYear) {
			groupC += "year,";
			columnType += "i";
		}
		if (isMoon) {
			groupC += "moon,";
			columnType += "i";
		}
		if (isItem) {
			groupC += "item,";
			columnType += "s";
		}
		columnType += "i";
		sqlStr = "SELECT " + groupC + " SUM(money)  FROM tax_g WHERE 1 " + sqlStr + " GROUP BY " + groupC.substr(0, groupC.size() - 1);
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
	else {		
		columnType = "idsiss";
		sqlStr = "SELECT t1.sale_id AS 'number', t1.day AS 'date', t2.name AS 'item', (t1.money * (1 + t1.tax)) AS 'money', t1.person AS 'person', t1.subject AS 'subject'    FROM sales t1 INNER JOIN customer t2 USING(item) WHERE 1 ";
		if (itemID >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.item = %d ", itemID);
		if (moom > 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", moom);
		sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id >= %d ", allStert);
		if (allFinish >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND t1.sale_id <= %d ", allStert);
		if (year >= 0)
			sqlStr += (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"AND TIME_FORMAT(t1.day,'%c') = %d ", year);
		if (isEffectiveness)
			sqlStr += "AND t2.effectiveness = 1 ";
		sqlStr += ";";
		RunSql();
		return GetResultStr();
	}
}

std::string DB::Select::IncomeTax(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	//月指定がある場合のみ月ごとにし、月の指定まで出来るようにする
	//それ以外は、強制的に年ごとにする
	//値を指定するやつは、年のみとする
	//有効なもののみ指定の場合、それを経費のみに反映する
	//収入の計算方法は「売上（掛け売り）- 経費」とする
	//所得税は収入の額により税率を適応し、実数値で出す
	std::string salesSql, expensesSql;
	std::string whereSql;
	std::string incomeTaxSql;
	whereSql = "";
	if (isMoon) {
		//掛け売り（売上）の取得SQL
		salesSql = " SELECT CONCAT(year, '-' ,moon) AS day, SUM(money) AS gold FROM sales_g WHERE 1 %ls GROUP BY CONCAT(year, '-' ,moon) ";
		if (moom > 0)
			whereSql += " AND moon = " + std::to_string(moom) + " ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		salesSql = (MyTools::TString)MyTools::format((MyTools::TString)salesSql, ((MyTools::TString)whereSql).c_str());
		whereSql = "";
		//経費の取得SQL
		expensesSql = " SELECT CONCAT(year, '-' ,moon) AS day, SUM(money * rate) AS gold FROM expenses_g WHERE 1 %ls GROUP BY CONCAT(year, '-' ,moon) ";
		if (moom > 0)
			whereSql += " AND moon = " + std::to_string(moom) + " ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		if (isItem)
			whereSql += " AND is_effectiveness = 1 ";
		expensesSql = (MyTools::TString)MyTools::format((MyTools::TString)expensesSql, ((MyTools::TString)whereSql).c_str());
	}
	else {
		//掛け売り（売上）の取得SQL
		salesSql = " SELECT year AS day, SUM(money) AS gold FROM sales_g WHERE 1 %ls GROUP BY year ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		salesSql = (MyTools::TString)MyTools::format((MyTools::TString)salesSql, ((MyTools::TString)whereSql).c_str());
		whereSql = "";
		//経費の取得SQL
		expensesSql = " SELECT year AS day, SUM(money * rate) AS gold FROM expenses_g WHERE 1 %ls GROUP BY year ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		if (isItem)
			whereSql += " AND is_effectiveness = 1 ";
		expensesSql = (MyTools::TString)MyTools::format((MyTools::TString)expensesSql, ((MyTools::TString)whereSql).c_str());
	}
	salesSql = " ( " + salesSql + " ) ";
	expensesSql = " ( " + expensesSql + " ) ";
	//抽出する
	incomeTaxSql = "SELECT t1.day AS days ,(t1.gold - t2.gold) AS money FROM " + salesSql + " t1" +
		" INNER JOIN " + expensesSql + " t2 " + "ON t1.day = t2.day";
	sqlStr = "SELECT t3.days,(CASE WHEN TRUNCATE(t3.money,-3) > 18000000 THEN (0.4 * TRUNCATE(t3.money,-3) - 2796000) " +
		(std::string)" WHEN TRUNCATE(t3.money,-3) > 9000000 THEN (0.33 * TRUNCATE(t3.money,-3) - 1536000) " +
		" WHEN TRUNCATE(t3.money,-3) > 6950000 THEN (0.23 * TRUNCATE(t3.money,-3) - 636000) " +
		"WHEN TRUNCATE(t3.money,-3) > 3300000 THEN (0.2 * TRUNCATE(t3.money,-3) - 427500) " +
		"WHEN TRUNCATE(t3.money,-3) > 1950000 THEN (0.1 * TRUNCATE(t3.money,-3) - 97500) " +
		"ELSE  0.05 * TRUNCATE(t3.money,-3) "+
		"END ) FROM " + "( " + incomeTaxSql + " ) t3;";
	columnType = "ss";
	RunSql();
	return GetResultStr();
}

std::string DB::Select::Income(bool isGroup, int itemID, int moom, int allStert, int allFinish, int year, bool isEffectiveness, bool isMoon, bool isYear, bool isItem) {
	//月指定がある場合のみ月ごとにし、月の指定まで出来るようにする
	//それ以外は、強制的に年ごとにする
	//値を指定するやつは、年のみとする
	//有効なもののみ指定の場合、それを経費のみに反映する
	//収入の計算方法は「売上（掛け売り）- 経費」とする
	std::string salesSql, expensesSql;
	std::string whereSql;
	whereSql = "";
	if (isMoon) {
		//掛け売り（売上）の取得SQL
		salesSql = " SELECT CONCAT(year, '-' ,moon) AS day, SUM(money) AS gold FROM sales_g WHERE 1 %ls GROUP BY CONCAT(year, '-' ,moon) ";
		if (moom > 0)
			whereSql += " AND moon = " + std::to_string(moom) + " ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		salesSql = (MyTools::TString)MyTools::format((MyTools::TString)salesSql, ((MyTools::TString)whereSql).c_str());
		whereSql = "";
		//経費の取得SQL
		expensesSql = " SELECT CONCAT(year, '-' ,moon) AS day, SUM(money * rate) AS gold FROM expenses_g WHERE 1 %ls GROUP BY CONCAT(year, '-' ,moon) ";
		if (moom > 0)
			whereSql += " AND moon = " + std::to_string(moom) + " ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		if (isItem)
			whereSql += " AND is_effectiveness = 1 ";
		expensesSql = (MyTools::TString)MyTools::format((MyTools::TString)expensesSql, ((MyTools::TString)whereSql).c_str());
	}
	else {
		//掛け売り（売上）の取得SQL
		salesSql = " SELECT year AS day, SUM(money) AS gold FROM sales_g WHERE 1 %ls GROUP BY year ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		salesSql = (MyTools::TString)MyTools::format((MyTools::TString)salesSql, ((MyTools::TString)whereSql).c_str());
		whereSql = "";
		//経費の取得SQL
		expensesSql = " SELECT year AS day, SUM(money * rate) AS gold FROM expenses_g WHERE 1 %ls GROUP BY year ";
		if (year > 0)
			whereSql += " AND year = " + std::to_string(year) + " ";
		if (isItem)
			whereSql += " AND is_effectiveness = 1 ";
		expensesSql = (MyTools::TString)MyTools::format((MyTools::TString)expensesSql, ((MyTools::TString)whereSql).c_str());
	}
	salesSql = " ( " + salesSql + " ) ";
	expensesSql = " ( " + expensesSql + " ) ";
	//抽出する
	sqlStr = " SELECT t1.day ,(t1.gold - t2.gold) FROM " + salesSql + " t1 " +
		 " INNER JOIN " + expensesSql + " t2 " + " ON t1.day = t2.day;";
	//実行・出力
	columnType = "ss";
	RunSql();
	return GetResultStr();
}

void DB::Insert::Customer(std::string name){
	sqlStr = (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"INSERT INTO customer(name) VALUES('%ls');", ((std::wstring)(MyTools::TString)name).c_str());
	RunSql();
}

void DB::Insert::ExpensesItem(std::string name, double rate){
	sqlStr = (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"INSERT INTO expenses_item(name,rate) VALUES('%ls',%lf);", ((std::wstring)(MyTools::TString)name).c_str(),rate);
	RunSql();
}

void DB::Insert::Expenses(std::string date, int item, int money){
	sqlStr = (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"INSERT INTO expenses(day,item,money) VALUES('%ls',%d,%d);", ((std::wstring)(MyTools::TString)date).c_str(), item, money);
	RunSql();
}

void DB::Insert::Unpaid(int id, int money){//Paidのこと
	sqlStr = (MyTools::TString)MyTools::format((std::wstring)(MyTools::TString)"UPDATE paid SET money = money + %d WHERE sale_id=%d;", money, id);
	RunSql();
}

void DB::Insert::Sales(std::string date, int customer, int money, std::string person, std::string subject){
	std::string valueSql = "'" + date + "'," + std::to_string(customer) + "," + std::to_string(money) + ",'" + person + "'," + "'" + subject + "'";
	sqlStr = "INSERT INTO Sales(day,item,money,person,subject) VALUES(" + valueSql + ");";
	RunSql();
}

void DB::Delete::Customer(int id) {
	sqlStr = (MyTools::TString)MyTools::format(_TEXT("DELETE FROM customer WHERE item = %d;"),id);
	RunSql();
}
void DB::Delete::ExpensesItem(int id) {
	sqlStr = (MyTools::TString)MyTools::format(_TEXT("DELETE FROM expenses_item WHERE item = %d;"), id);
	RunSql();
}
void DB::Delete::Expenses(int id) {
	sqlStr = (MyTools::TString)MyTools::format(_TEXT("DELETE FROM expenses WHERE id = %d;"), id);
	RunSql();
}
void DB::Delete::Sales(int id){
	sqlStr = (MyTools::TString)MyTools::format(_TEXT("DELETE FROM sales WHERE sale_id = %d;"), id);
	RunSql();
}

void DB::Update::Customer(int id, std::string name, int isTrue) {
	sqlStr = "UPDATE customer SET name = '%ls',is_effectiveness = %d WHERE item = %d;";
	sqlStr = (MyTools::TString)MyTools::format((MyTools::TString)sqlStr, MyTools::Encode::ShiftJis::Unicode(name).c_str(), isTrue, id);
	RunSql();
}
void DB::Update::ExpensesItem(int id, std::string name, double rate, int isTrue) {
	sqlStr = "UPDATE expenses_item SET name = '%ls',rate = %lf,is_effectiveness = %d WHERE item = %d;";
	sqlStr = (MyTools::TString)MyTools::format((MyTools::TString)sqlStr, MyTools::Encode::ShiftJis::Unicode(name).c_str(), rate, isTrue, id);
	RunSql();
}
void DB::Update::Expenses(int id, std::string date, int item, int money) {
	sqlStr = "UPDATE expenses SET day = '%ls',item = %d,money = %d WHERE id = %d;";
	sqlStr = (MyTools::TString)MyTools::format((MyTools::TString)sqlStr, MyTools::Encode::ShiftJis::Unicode(date).c_str(), item, money,id);
	RunSql();
}
void DB::Update::Unpaid(int id, int money) {
	sqlStr = "UPDATE paid SET money = %d WHERE sale_id = %d;";
	sqlStr = (MyTools::TString)MyTools::format((MyTools::TString)sqlStr, money,id);
	RunSql();
}
void DB::Update::Sales(int id, std::string date, int customer, int money, std::string tax, std::string person, std::string subject) {
	sqlStr = "UPDATE sales SET day = '%ls',item = %d,money = %d,tax = %ls,person = '%ls',subject = '%ls' WHERE sale_id = %d;";
	sqlStr = (MyTools::TString)MyTools::format((MyTools::TString)sqlStr, MyTools::Encode::ShiftJis::Unicode(date).c_str(), customer, money, ((std::wstring)(MyTools::TString)tax).c_str(),
		MyTools::Encode::ShiftJis::Unicode(person).c_str(), MyTools::Encode::ShiftJis::Unicode(subject).c_str(), id);
	RunSql();
}