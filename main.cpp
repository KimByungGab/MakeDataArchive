#pragma once

#include <iostream>

#include "DataArchive.h"

using namespace std;

int main()
{
	// locale을 유니코드로 설정
	locale::global(locale(""));

	int tempA = 1;
	wstring tempB = L"Hello 병갑! Nice to meet you! 만나서 반가워! ㅋㅅㅋ";
	short tempC = 2;
	long tempD = 3;
	float tempE = 4;

	int a;
	wstring b;
	short c;
	long d;
	float e;

	DataArchive DA;

	DA << tempA << tempB << tempC << tempD << tempE;
	
	cout << "Data 삽입 완료!" << endl;

	DA >> a >> b >> c >> d >> e;

	wcout << L"a: " << a << L" / b: " << b << L" / c: " << c << L" / d: " << d << L" / e: " << e << endl;

	return 0;
}