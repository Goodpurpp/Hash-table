#include "Dictionary.h"
#include <cassert>
#include <iostream>
#include "ArrayList.h"

using namespace std;

Dictionary::Dictionary()
{
	maxBucketCount = 17;
	threshold = (int)(loadFactor * maxBucketCount);
	bucketCount = 0;
	pBuckets = new Container *[maxBucketCount];
	memset(pBuckets, 0, sizeof(pBuckets[0]) * maxBucketCount);
}

Dictionary::~Dictionary()
{
	for (int i = 0; i < maxBucketCount; ++i)
	{
		Container* pCurrentBucket = pBuckets[i];
		if (pCurrentBucket == nullptr)
			continue;

		while (pCurrentBucket != nullptr)
		{
			Container* deleteContainer = pCurrentBucket;
			pCurrentBucket = pCurrentBucket->pNext;
			delete deleteContainer;
		}
	}
	delete[] pBuckets;
}


int Dictionary::Value(const std::string& key)
{
	int index = HashCode(key);
	Container* currentBucket = pBuckets[index];
	if (currentBucket == nullptr)
		return -1;

	while (currentBucket != nullptr)
	{
		if (currentBucket->key == key)
			return currentBucket->data;

		currentBucket = currentBucket->pNext;
	}

	return -1;
}

void Dictionary::Add(const std::string& key, int data)
{
	if (bucketCount == threshold)
		Resize();

	int index = HashCode(key);

	Container* currentBucket = pBuckets[index];
	if (currentBucket == nullptr)
	{
		currentBucket = new Container();
		currentBucket->data = data;
		currentBucket->key = key;
		currentBucket->pNext = nullptr;
		pBuckets[index] = currentBucket;
		bucketCount++;
		return;
	}

	Container* lastBucket = nullptr;
	while (currentBucket != nullptr)
	{
		if (currentBucket->key == key)
		{
			currentBucket->data = data;
			return;
		}
		if (currentBucket->pNext == nullptr)
			lastBucket = currentBucket;

		currentBucket = currentBucket->pNext;
	}

	Container* newData = new Container();
	newData->data = data;
	newData->key = key;
	newData->pNext = nullptr;
	lastBucket->pNext = newData;
}

void Dictionary::Remove(const std::string& key)
{
	int index = HashCode(key);
	Container* currentBucket = pBuckets[index];
	if (currentBucket == nullptr)
		return;

	/*
	if (currentBucket->key == key)
	{
		Container* nextBucket = currentBucket->pNext;
		delete currentBucket;
		pBuckets[index] = nextBucket;
	}
	*/

	Container* prevBucket = currentBucket;
	while (currentBucket != nullptr)
	{
		if (currentBucket->key == key && currentBucket == prevBucket)
		{
			Container* nextBucket = currentBucket->pNext;
			delete currentBucket;
			pBuckets[index] = nextBucket;
			return;
		}

		if (currentBucket->key == key)
		{
			Container* nextBucket = currentBucket->pNext;
			delete currentBucket;
			prevBucket->pNext = nextBucket;
			return;
		}

		prevBucket = currentBucket;		
		currentBucket = currentBucket->pNext;
	}
}

/*
int Dictionary::operator[](const std::string& key)
{
	return Value(key);
}
*/

int& Dictionary::operator[](const std::string& key)
{
	int index = 0;
	Container* currentBucket = pBuckets[index];
	if (currentBucket == nullptr)
	{
		currentBucket = new Container();
		currentBucket->data = 0;
		currentBucket->key = key;
		currentBucket->pNext = nullptr;
		pBuckets[index] = currentBucket;
		bucketCount++;
		return currentBucket->data;
	}

	while (currentBucket != nullptr)
	{
		if (currentBucket->key == key)
			return currentBucket->data;

		currentBucket = currentBucket->pNext;
	}
	//throw "Unreachable code";//	assert("very bad");
	cerr << "Unreachable code";
	return unreachableCode;
}


const int Dictionary::operator[](const std::string& key) const
{
	return 0;
}

std::string* Dictionary::Keys(int& size) const
{
	return nullptr;
}

int* Dictionary::Values(int& size) const
{
	ArrayList list;

	for (int i = 0; i < maxBucketCount; ++i)
	{
		Container* pCurrentBucket = pBuckets[i];
		if (pCurrentBucket == nullptr)
			continue;

		while (pCurrentBucket != nullptr)
		{
			list.Append(pCurrentBucket->data);
			pCurrentBucket = pCurrentBucket->pNext;
		}
	};
	return list.ToArray(size);
}

void Dictionary::PrintData(Container* pBucket)
{
	cout << "[" << pBucket->key << "]=" << pBucket->data << endl;
}

/*
Для любых x,y из O: x != y => H(x) != H(y) - слабая хэш функция


*/
int Dictionary::HashCode(const std::string& key)
{
	int hash = key[0] % maxBucketCount;
	return hash;
}

void Dictionary::Resize()
{
	int oldSize = maxBucketCount;
	maxBucketCount = maxBucketCount * 2;
	threshold = (int)(loadFactor * maxBucketCount);
	
	Container** pNewBuckets = new Container*[maxBucketCount];
	memset(pNewBuckets, 0, sizeof(pNewBuckets[0]) * maxBucketCount);
	Transfer(pNewBuckets, oldSize);
}

void Dictionary::Transfer(Container** pNewBuckets, int oldSize)
{
	for (int i  = 0; i < oldSize; i++)
	{
		Container* pBucket = pBuckets[i];
		if (pBucket == nullptr)
			continue;

		string key = pBucket->key;
		int index = HashCode(key);
		pNewBuckets[index] = pBucket;
	}
	delete[] pBuckets;
	pBuckets = pNewBuckets;
}

bool Dictionary::Contains(const std::string& key)
{	
	int index = HashCode(key);
	Container* pCurrentBucket = pBuckets[index];
	
	if (pCurrentBucket == nullptr)
		return false;

	while (pCurrentBucket != nullptr)
	{
		if (pCurrentBucket->key == key)
			return true;

		pCurrentBucket = pCurrentBucket->pNext;
	}
	return false;
}

void Dictionary::Print()
{
	for (int i = 0; i < maxBucketCount; ++i)
	{
		Container* pCurrentBucket = pBuckets[i];
		if (pCurrentBucket == nullptr)
			continue;

		while (pCurrentBucket != nullptr)
		{
			PrintData(pCurrentBucket);
			pCurrentBucket = pCurrentBucket->pNext;
		}
	}
}
