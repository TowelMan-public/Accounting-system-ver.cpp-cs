#include "TString.h"

namespace MyTools {
	/*文字列の変換(std::string、std::wstring 間)*/
	std::string WStringToString(std::wstring istr) {
		int size = WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, (char*)NULL, 0, NULL, NULL);
		char* ostr = new char[size];
		WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, ostr, size, NULL, NULL);
		std::string rtn = ostr;
		delete[] ostr;
		return rtn;
	}
	std::wstring StringToWString(std::string istr) {
		size_t size;
		wchar_t* ostr = new wchar_t[istr.length() + 2];
		mbstowcs_s(&size, ostr, istr.length() + 1, istr.c_str(), _TRUNCATE);
		std::wstring rtn = ostr;
		delete[] ostr;
		return rtn;
	}

#define CP_SJIS 932
	std::string UTF8toSJIS(const std::string strUTF8)
	{
		// UTF-8 → UNICODE
		const int sizeUNICODE = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
		wchar_t* pUNICODE = new wchar_t[sizeUNICODE];
		MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pUNICODE, sizeUNICODE);

		// UNICODE → Shift_JIS
		const int sizeSJIS = WideCharToMultiByte(CP_SJIS, 0, pUNICODE, -1, NULL, 0, NULL, NULL);
		char* pSJIS = new char[sizeSJIS];
		WideCharToMultiByte(CP_SJIS, 0, pUNICODE, -1, pSJIS, sizeSJIS, NULL, NULL);

		std::string strSJIS(pSJIS);

		// バッファ削除
		delete[] pSJIS;
		pSJIS = NULL;
		delete[] pUNICODE;
		pUNICODE = NULL;

		return strSJIS;
	}

	/*TStringのコンストラクタたち*/

	TString::TString(void)	{
		//何もしない
	}

	TString::TString(std::MY_OTHER_STRING_0028 str) {
		this->operator=(str);
	}

	TString::TString(MY_OTHER_CHAR ch) {
		std::MY_OTHER_STRING_0028 str = &ch;
		this->operator=(str);
	}

	TString::TString(MY_OTHER_CHAR* str) {
		std::MY_OTHER_STRING_0028 str2 = str;
		this->operator=(str2);
	}

	TString::TString(std::MY_STRING_0028 str) {
		this->swap(str);
	}

	/*TStringのキャスト変換たち*/

	TString::operator std::MY_OTHER_STRING_0028() const{
		std::MY_STRING_0028 str = this->c_str();
		std::MY_OTHER_STRING_0028 str2;
		//逆になる
#ifndef _UNICODE
		str2 = StringToWString(str);
#else
		str2 = WStringToString(str);
#endif
		return str2;
	}

	TString::operator CString() const{
		CString  str = this->c_str();
		return str;
	}

	/*TStringの演算子=のオーバーロードたち*/

	TString TString::operator=(std::MY_STRING_0028 str) {
		this->swap(str);
		return *this;
	}
	TString TString::operator=(std::MY_OTHER_STRING_0028 str) {
		std::MY_STRING_0028 str2;
#ifdef _UNICODE
		str2 = StringToWString(str);
#else
		str2 = WStringToString(str);
#endif
		this->swap(str2);//反映
		return *this;
	}
	TString TString::operator=(CString str) {
		std::MY_STRING_0028 str2 = str.GetBuffer();
		this->swap(str2);
		return *this;
	}
	TString TString::operator=(MY_OTHER_CHAR ch) {
		std::MY_OTHER_STRING_0028 str = &ch;
		std::MY_STRING_0028 str2 = this->operator=(str);
		this->swap(str2);
		return *this;
	}
	TString TString::operator=(MY_OTHER_CHAR* str) {
		std::MY_OTHER_STRING_0028 str2 = str;
		std::MY_STRING_0028 str3 = this->operator=(str2);
		this->swap(str3);
		return *this;
	}

	template <typename ... Args>
	std::MY_STRING_0028 TString::format(const std::MY_STRING_0028 fmt, Args ... args) {
		CString rtn;
		rtn.Format(fmt.c_str(), args...);
		this->operator=((std::MY_STRING_0028)rtn.GetBuffer());
		return *this;
	}
}