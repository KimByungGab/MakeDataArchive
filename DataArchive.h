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
		// �ʱ�ȭ
		readPos = 0;
	}

	// ���� ũ��� ��������
	template<typename T>
	inline DataArchive& operator<<(const T& data)
	{
		// ���� ������ ������ ũ�� ��������
		size_t dataSize = sizeof(T);

		// ���� ũ�� ������ (���� ������ + �߰��� ������)
		buffer.resize(buffer.size() + dataSize);

		// ���ۿ� ���� (���� ù �ּ� + ���� ������(������ �߰������� ũ�Ⱑ ��Ƣ�����) - �߰��� ������ = ���ݱ��� ������ ������)
		memcpy(buffer.data() + buffer.size() - dataSize, &data, dataSize);

		return *this;
	}

	template<typename T>
	inline DataArchive& operator>>(T& data)
	{
		// ���� '���� ��ġ + ������ ũ��'�� ������ ����� �Ѿ� ���ٸ� �װ��� ���� �� ��
		if (readPos + sizeof(T) > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		// �����Ϳ� ���� ��ġ���� �����ŭ �޸� ����
		memcpy(&data, buffer.data() + readPos, sizeof(T));

		// ���� ��ġ ������Ʈ
		readPos += sizeof(T);

		return *this;
	}

	// String���� ���������̱� ������ int������ size�� ���� ����ְ� data�� �ִ´�.
	inline DataArchive& operator<<(const wstring& data)
	{
		// ���ڿ� ������ �����ͼ� �ֱ�
		int length = data.size();
		*this << length;

		// wchar�� �׳� char�� �ٸ��� 2����Ʈ�̱� ������ ���ó�� ���ش�.
		size_t dataSize = length * sizeof(wchar_t);

		// ���� ������ ������ �� ����
		buffer.resize(buffer.size() + dataSize);
		memcpy(buffer.data() + buffer.size() - dataSize, data.c_str(), dataSize);

		return *this;
	}

	inline DataArchive& operator>>(wstring& data)
	{
		// ���ڿ� ������ ��������
		int length;
		*this >> length;

		// wchar�� �׳� char�� �ٸ��� 2����Ʈ�̱� ������ ���ó�� ���ش�.
		size_t dataSize = length * sizeof(wchar_t);

		// ���� '���� ��ġ + ������ ũ��'�� ������ ����� �Ѿ� ���ٸ� �װ��� ���� �� ��
		if (readPos + dataSize > buffer.size())
		{
			throw out_of_range("Not enough data in vector");
		}

		// �����Ϳ� ���� ��ġ���� �����ŭ �޸� ����
		// buffer.data() + readPos�� char*�� �ǹǷ� ����ȯ�� �����༭ assign�� �� �� �ְ� ������ش�.
		data.assign(reinterpret_cast<wchar_t*>(buffer.data() + readPos), length);

		// ���� ��ġ ������Ʈ
		readPos += dataSize;

		return *this;
	}

private:
	size_t readPos;			// �о���� ��ġ ���
	vector<char> buffer;	// ����. ���ͷ� �ؼ� �������� ��� �߰��� �� �� �ְ� �ߴ�.
};