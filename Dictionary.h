#pragma once

#include <string>

struct Container
{
	int data;
	std::string key;
	Container* pNext;

	/*
	~Container()
	{
		if (pNext != nullptr)
			delete pNext;
	}
	*/
};


class Dictionary
{
public:
	Dictionary();
	~Dictionary();

	bool Contains(const std::string& key);
	void Print();

	int Value(const std::string& key);
	void Add(const std::string& key, int data);
	void Remove(const std::string& key);
	int& operator[](const std::string& key);

	const int operator[](const std::string& key) const;

	std::string* Keys(int& size) const;
	int* Values(int& size) const;


private:
	void PrintData(Container* pBucket);
	int HashCode(const std::string& key);
	void Resize();
	void Transfer(Container** pNewBuckets, int oldSize);


	int unreachableCode = 0;
	double loadFactor = 0.75;

	int threshold;
	int maxBucketCount;
	Container** pBuckets;
	int bucketCount;
};