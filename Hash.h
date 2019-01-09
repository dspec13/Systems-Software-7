/*
	CSE 109: Spring 2018
	Dylan Spector
	drs320
	Header file for Hash_t obj
	Program #7
*/

#include<openssl/evp.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<string.h>
#include<sstream>

using namespace std;

#ifndef HASH_H
#define HASH_H

int defaultHash(void* data, size_t size);
int basic_hash(void*, size_t);

class Hash_t
{
public:
	Hash_t();				// buckets = 10; default hash function
	Hash_t(size_t buckets); // this->buckets = buckets; default hash function
	Hash_t(size_t buckets, int(*hashfunc)(void*, size_t)); // this->buckets = buckets; use hashfunc from parameters
	~Hash_t();

	Hash_t(Hash_t&); // copy constructor (deep copy)

	Hash_t& operator=(const Hash_t&);

	bool insert(unsigned int);

	bool insert(char);

	Hash_t& operator+=(unsigned int);

	Hash_t& operator+=(char);

	bool find(unsigned int) const;

	bool find(char) const;

	bool remove(unsigned int);

	bool remove(char);

	Hash_t& operator-=(unsigned int);

	Hash_t& operator-=(char);

	string toString() const;

	string toString(unsigned int) const;

	string toString(char) const;

	size_t size() const;

	friend ostream& operator<<(ostream& lhs, const Hash_t& rhs);

private:
	int(*hashfunc)(void*, size_t); // ? how to use pointer properly
	size_t _size;                             // tracks # of elements within the hash
	vector<vector<unsigned int>> bucketList;  // Chaining Hash Table

	int getIndex(unsigned int) const;
	int getIndex(char) const;
};

#endif
