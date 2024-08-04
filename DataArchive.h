#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

#define MAX_BUFFER_SIZE 1024	// ��Ŷ�� ���� ũ�⸦ ���󰡸� ���� ��

using namespace std;

class DataArchive
{
public:
	DataArchive()
	{
		readPos = 0;
	}
	template<typename T>
	inline DataArchive& operator<<(const T& data)
	{
		size_t dataSize = sizeof(T);
		buffer.resize(buffer.size() + dataSize);
		memcpy(buffer.data() + buffer.size() - dataSize, &data, dataSize);

		return *this;
	}

	template<typename T>
	inline DataArchive& operator>>(T& data)
	{
		if (readPos + sizeof(T) > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		memcpy(&data, buffer.data() + readPos, sizeof(T));
		readPos += sizeof(T);

		return *this;
	}

	// String���� ���������̱� ������ int������ size�� ���� ����ְ� data�� �ִ´�.
	inline DataArchive& operator<<(const wstring& data)
	{
		int length = data.size();
		*this << length;
		size_t dataSize = length * sizeof(wchar_t);
		buffer.resize(buffer.size() + dataSize);
		memcpy(buffer.data() + buffer.size() - dataSize, data.c_str(), dataSize);

		return *this;
	}

	inline DataArchive& operator>>(wstring& data)
	{
		int length;
		*this >> length;
		size_t dataSize = length * sizeof(wchar_t);
		if (readPos + dataSize > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		data.assign(reinterpret_cast<wchar_t*>(buffer.data() + readPos), length);
		readPos += dataSize;

		return *this;
	}

private:
	size_t readPos;
	vector<char> buffer;
};