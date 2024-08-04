#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

using namespace std;

class DataArchive
{
public:
	DataArchive()
	{
		// 초기화
		readPos = 0;
	}

	// 고정 크기는 이쪽으로
	template<typename T>
	inline DataArchive& operator<<(const T& data)
	{
		// 고정 길이의 데이터 크기 가져오기
		size_t dataSize = sizeof(T);

		// 버퍼 크기 재조정 (원래 사이즈 + 추가될 사이즈)
		buffer.resize(buffer.size() + dataSize);

		// 버퍼에 복사 (버퍼 첫 주소 + 버퍼 사이즈(위에서 추가됐으니 크기가 뻥튀기됐음) - 추가될 사이즈 = 지금까지 저장한 사이즈)
		memcpy(buffer.data() + buffer.size() - dataSize, &data, dataSize);

		return *this;
	}

	template<typename T>
	inline DataArchive& operator>>(T& data)
	{
		// 만약 '읽은 위치 + 사이즈 크기'가 버퍼의 사이즈를 넘어 선다면 그것은 말이 안 됨
		if (readPos + sizeof(T) > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		// 데이터에 읽은 위치부터 사이즈만큼 메모리 복사
		memcpy(&data, buffer.data() + readPos, sizeof(T));

		// 읽은 위치 업데이트
		readPos += sizeof(T);

		return *this;
	}

	// String쪽은 가변길이이기 때문에 int형으로 size를 먼저 집어넣고 data를 넣는다.
	inline DataArchive& operator<<(const wstring& data)
	{
		// 문자열 사이즈 가져와서 넣기
		int length = data.size();
		*this << length;

		// wchar는 그냥 char와 다르게 2바이트이기 때문에 배수처리 해준다.
		size_t dataSize = length * sizeof(wchar_t);

		// 버퍼 사이즈 재조정 후 복사
		buffer.resize(buffer.size() + dataSize);
		memcpy(buffer.data() + buffer.size() - dataSize, data.c_str(), dataSize);

		return *this;
	}

	inline DataArchive& operator>>(wstring& data)
	{
		// 문자열 사이즈 가져오기
		int length;
		*this >> length;

		// wchar는 그냥 char와 다르게 2바이트이기 때문에 배수처리 해준다.
		size_t dataSize = length * sizeof(wchar_t);

		// 만약 '읽은 위치 + 사이즈 크기'가 버퍼의 사이즈를 넘어 선다면 그것은 말이 안 됨
		if (readPos + dataSize > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		// 데이터에 읽은 위치부터 사이즈만큼 메모리 복사
		// buffer.data() + readPos가 char*가 되므로 형변환을 시켜줘서 assign을 할 수 있게 만들어준다.
		data.assign(reinterpret_cast<wchar_t*>(buffer.data() + readPos), length);

		// 읽은 위치 업데이트
		readPos += dataSize;

		return *this;
	}

private:
	size_t readPos;			// 읽어들인 위치 헤더
	vector<char> buffer;	// 버퍼. 벡터로 해서 동적으로 계속 추가를 할 수 있게 했다.
};