#include "kaikei.h"

Kaikei::Kaikei() {//������
	DB::DB db;
	db.Conect();//�f�[�^�x�[�X�ڑ�
	hnet = new MyTools::Hnet(true);
}

Kaikei::~Kaikei() {//�I������
	DB::DB db;
	db.CutEnd();//�f�[�^�x�[�X�ڑ��̐ؒf
	delete hnet;
}

void Kaikei::Run(void) {//�G���g���[�|�C���g
	try {
		std::string meg;
		std::string bafIpAddress;

		while (true) {

			//�ڑ��v�����o������󂯓���
			if (hnet->Accept(bafIpAddress)) {
				ipAddress = bafIpAddress;
				meg = GetRequest();//���N�G�X�g�̎擾��

				//�L���ȃ��N�G�X�g�Ȃ烊�N�G�X�g�����s
				if (meg.length() > 0) {
					std::cout << meg << " ";
					DoRequest(meg);
				}
				std::cout << std::endl;
			}
		}
	}
	catch (...) {//��O�����E�I������
		hnet->CutEnd();
	}
}

bool Kaikei::IsLogin() {
	auto itr = LoginMap.find(ipAddress);
	std::cout << ipAddress << " ";
	if (itr != LoginMap.end())
		std::cout << LoginMap[ipAddress] << " ";
	return itr != LoginMap.end();
}

std::string Kaikei::GetRequest() {//���N�G�X�g�̎擾��
	std::string meg;
	//���b�Z�[�W�擾
	meg = hnet->Get();
	//����Ȃ��
	if (IsLogin() || meg.find("LOGIN") != -1) {
		return meg;
	}
	else {//���ۂ���
		hnet->CutEnd();
		return "";
	}
}

void Kaikei::DoRequest(std::string meg) {//���N�G�X�g�̎��s
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
	else//�s��
		hnet->CutEnd();
}

void Kaikei::Login(std::string meg) {
	DB::DB db;
	std::string userName;
	std::string password;
	std::smatch results;
	//���[�U�[���擾
	if (!std::regex_match(meg, results, std::regex(R"(LOGIN\((.*),(.*)\))"))) {
		hnet->CutEnd();
		return;
	}
	userName = results[1].str();
	password = results[2].str();
	//����
	if (db.GetPassword(userName, password)) {//���O�C������
		LoginMap[ipAddress] = userName;
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->CutEnd();
	}
	else//���s
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
	/*���b�Z�[�W�̎擾*/
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
	//�O���[�v��
	if (meg.find("GROUP") != -1) {
		isGroup = true;
		if (meg.find("KOUMOKU") != -1)
			isItem = true;
		if (meg.find("MOOM") != -1)
			isMoon = true;;
		if (meg.find("YERA") != -1)
			isYear = true;
	}
	//�L���Ȃ��̂̂�
	if (meg.find("TRUE") != -1)
		isEffectiveness = true;
	/*�U�蕪��*/
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
	else {//�s��
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
	else if (meg.find("PAID") != -1)
		InsertUnpaid(meg);
	else if (meg.find("KAKEURI") != -1)
		InsertSales(meg);
	else//�s��
		hnet->CutEnd();
}

void Kaikei::InsertCustomer(std::string meg) {
	DB::Insert db;
	std::string name;
	std::smatch results;
	//TODO ���b�Z�[�W�̎擾
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
	//TODO ���b�Z�[�W�̎擾
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
	//TODO ���b�Z�[�W�̎擾
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
	//TODO ���b�Z�[�W�̎擾
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
	//TODO ���b�Z�[�W�̎擾
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

//�o�^
void Kaikei::Update(std::string meg) {
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
	else//�s��
		hnet->CutEnd();
}

void Kaikei::UpdateCustomer(std::string meg) {
	DB::Update db;
	std::smatch results;
	int id,isTrue;
	std::string name;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(UPDATE_CUSTOMER\(([0-9]*),(.*),([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
			name = results[2].str();
			isTrue = stoi(results[3].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Customer(id,name,isTrue);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
void Kaikei::UpdateExpensesItem(std::string meg) {
	DB::Update db;
	std::smatch results;
	int id, isTrue;
	double rate;
	std::string name;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(UPDATE_JUNL\(([0-9]*),(.*),([0-9]*\.[0-9]*),([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
			name = results[2].str();
			rate = stof(results[3].str());
			isTrue = stoi(results[4].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.ExpensesItem(id, name,rate, isTrue);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
//TODO�@��������
void Kaikei::UpdateExpenses(std::string meg) {
	DB::Update db;
	std::smatch results;
	int id, item,money;
	std::string date;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(UPDATE_KEIHI\(([0-9]*),(.*),([0-9]*),([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
			date = results[2].str();
			item = stoi(results[3].str());
			money = stoi(results[4].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Expenses(id, date,item,money);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
void Kaikei::UpdateUnpaid(std::string meg) {
	DB::Update db;
	std::smatch results;
	int id, money;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(UPDATE_PAID\(([0-9]*),([0-9]*)\))"))) {
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

void Kaikei::UpdateSales(std::string meg) {
	DB::Update db;
	std::smatch results;
	std::string strBaf;
	int id, money, customer;
	std::string date, person, subject;
	std::string tax;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(UPDATE_KAKEURI\(([0-9]*),(.*),([0-9]*),([0-9]*),([0-9]*\.[0-9]*),(.*),(.*)\))"))) {
		try {			
			id = stoi(results[1].str());
			date = results[2].str();
			customer = stoi(results[3].str());
			money = stoi(results[4].str());
			tax = results[5].str();
			person = results[6].str();
			subject = results[7].str();
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Sales(id,date,customer,money,tax,person,subject);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}

//�폜
void Kaikei::Delete(std::string meg) {
	if (meg.find("DELETE_CUSTOMER") != -1)
		DeleteCustomer(meg);
	else if (meg.find("DELETE_JUNL") != -1)
		DeleteExpensesItem(meg);
	else if (meg.find("DELETE_KEIHI") != -1)
		DeleteExpenses(meg);
	else if (meg.find("DELETE_KAKEURI") != -1)
		DeleteSales(meg);
	else//�s��
		hnet->CutEnd();
}

void Kaikei::DeleteCustomer(std::string meg) {
	DB::Delete db;
	std::smatch results;
	int id;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(DELETE_CUSTOMER\(([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Customer(id);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
void Kaikei::DeleteExpensesItem(std::string meg) {
	DB::Delete db;
	std::smatch results;
	int id;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(DELETE_JUNL\(([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.ExpensesItem(id);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
void Kaikei::DeleteExpenses(std::string meg) {
	DB::Delete db;
	std::smatch results;
	int id;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(DELETE_KEIHI\(([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Expenses(id);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}
void Kaikei::DeleteSales(std::string meg) {
	DB::Delete db;
	std::smatch results;
	int id;
	//���b�Z�[�W�̎擾
	if (std::regex_match(meg, results, std::regex(R"(DELETE_KAKEURI\(([0-9]*)\))"))) {
		try {
			id = stoi(results[1].str());
		}
		catch (...) {
			hnet->CutEnd();
			return;
		}
		db.Sales(id);
		hnet->Send(NetMeg::ANS_SUCCESS);
		hnet->Get();
	}
	hnet->CutEnd();
}