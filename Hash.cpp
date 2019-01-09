/*
    CSE 109: Spring 2018
    Dylan Spector
    drs320
	CPP file for Hash_t obj
    Program #7
*/

#include<openssl/evp.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<string.h>
#include<math.h>
#include"Hash.h"
#include<sstream>

using namespace std;

Hash_t::Hash_t()
:hashfunc(defaultHash), _size(0), bucketList(10)
{
}

Hash_t::Hash_t(size_t buckets)
: hashfunc(defaultHash), _size(0), bucketList(buckets)
{
}

Hash_t::Hash_t(size_t buckets, int(*hashfunc)(void*, size_t))
: hashfunc(hashfunc), _size(0), bucketList(buckets)
{
}

Hash_t::~Hash_t()
{
}

Hash_t::Hash_t(Hash_t& src)
: hashfunc(src.hashfunc), _size(src._size), bucketList(src.bucketList)
{
}

Hash_t& Hash_t::operator=(const Hash_t& rhs)
{
	hashfunc = rhs.hashfunc;
	_size = rhs.size();

	bucketList.clear();
	vector<unsigned int> temp;
	for(size_t i = 0; i < rhs.bucketList.size(); i++)
	{
		for(size_t j = 0; j < rhs.bucketList[i].size(); j++)
		{
			temp.push_back(rhs.bucketList[i][j]);
		}
		bucketList.push_back(temp);
		temp.clear();
	}
	return *this;
}


bool Hash_t::insert(unsigned int val)
{
	int index = getIndex(val);

	// for every element in the perspective bucket
	for(size_t i = 0; i < bucketList[index].size(); i++)
	{
		// if the element contains the value already
		if(bucketList[index][i] == val)
		{
			return false;
		}
	}
	bucketList[index].push_back(val);
	_size++;
	return true;
}




bool Hash_t::insert(char val)
{
    int index = getIndex(val);

	ssize_t tempInserti = -1;
	ssize_t tempInsertj = -1;

    // for every element in the perspective bucket
    for(size_t i = 0; i < bucketList[index].size(); i++)
    {
        unsigned int tempVal = bucketList[index][i];
        // check each unsigned int's char flags
        for(size_t j = 0; j < 4; j++)
        {
			// if space is empty, insert there
            unsigned int flag = (tempVal & (unsigned int)pow(2.0, (double)j));
			if(flag)
            {
                if(j == 0) // Looking at: 0000000 0000000 0000000 1111111 0000
                {
                    if((char)((tempVal & 0x7F0) >> 4) == val)
                    {
                        return false;
                    }
                }
                else if(j == 1) // Looking at: 0000000 0000000 1111111 0000000 0000
                {
                    if((char)((tempVal & 0x3F800) >> 11) == val)
                    {
                        return false;
                    }
                }
                else if(j == 2) // Looking at: 0000000 1111111 0000000 0000000 0000
                {
                    if((char)((tempVal & 0x1FC0000) >> 18) == val)
                    {
                        return false;
                    }
                }
                else if(j == 3) // Looking at: 1111111 0000000 0000000 0000000 0000
                {
                    if((char)((tempVal & 0xFE000000) >> 25) == val)
                    {
                        return false;
                    }
                }
            }
			else // flag is 0
			{
				tempInserti = (ssize_t)i;
				tempInsertj = (ssize_t)j;
			}
        }
    }

	// if we reach here, we can insert at the end.
	if(tempInserti != -1)
	{
		int i = tempInserti;
		if(tempInsertj == 0) // Looking at: 0000000 0000000 0000000 1111111 0000
        {
			bucketList[index][i] |= 0x7F0;
			bucketList[index][i] ^= 0x7F0;
			bucketList[index][i] |= (val << 4);

			bucketList[index][i] |= 1;
       	}
        else if(tempInsertj == 1) // Looking at: 0000000 0000000 1111111 0000000 0000
        {
			bucketList[index][i] |= 0x3F800;
            bucketList[index][i] ^= 0x3F800;
            bucketList[index][i] |= (val << 11);

			bucketList[index][i] |= 2;
        }
        else if(tempInsertj == 2) // Looking at: 0000000 1111111 0000000 0000000 0000
        {
			bucketList[index][i] |= 0x1FC0000;
            bucketList[index][i] ^= 0x1FC0000;
            bucketList[index][i] |= (val << 18);

		 	bucketList[index][i] |= 4;
      	}
       	else if(tempInsertj == 3) // Looking at: 1111111 0000000 0000000 0000000 0000
       	{
			bucketList[index][i] |= 0xFE000000;
            bucketList[index][i] ^= 0xFE000000;
            bucketList[index][i] |= (val << 25);

			bucketList[index][i] |= 8;
        }
	}
	else
	{
		unsigned int newCharSet = 0;
        newCharSet |= (val << 4);
        newCharSet |= 1;
		bucketList[index].push_back(newCharSet);
	}

	_size++;
	return true;
}






Hash_t& Hash_t::operator+=(unsigned int val)
{
	this->insert(val);
	return *this;
}

Hash_t& Hash_t::operator+=(char val)
{
    this->insert(val);
	return *this;
}














bool Hash_t::find(unsigned int val) const
{
	int index = getIndex(val);

    // for every element in the perspective bucket
    for(size_t i = 0; i < bucketList[index].size(); i++)
    {
        // if the element contains the value already
        if(bucketList[index][i] == val)
        {
            return true;
        }
    }
	return false;
}

bool Hash_t::find(char val) const
{
	int index = getIndex(val);

	// for every element in the persepctive bucket
	for(size_t i = 0; i < bucketList[index].size(); i++)
	{
		unsigned int tempVal = bucketList[index][i];
		// check each unsigned int's char flags
		for(size_t j = 0; j < 4; j++)
		{
			unsigned int flag = (tempVal & (unsigned int)pow(2.0, (double)j));
			if(flag)
			{
				if(j == 0) // Looking at: 0000000 0000000 0000000 1111111 0000
				{
					if((char)((tempVal & 0x7F0) >> 4) == val)
					{
						return true;
					}
				}
				else if(j == 1) // Looking at: 0000000 0000000 1111111 0000000 0000
				{
					if((char)((tempVal & 0x3F800) >> 11) == val)
                    {
                        return true;
                    }
				}
				else if(j == 2) // Looking at: 0000000 1111111 0000000 0000000 0000
				{
					if((char)((tempVal & 0x1FC0000) >> 18) == val)
                    {
                        return true;
                    }
				}
				else if(j == 3) // Looking at: 1111111 0000000 0000000 0000000 0000
				{
					if((char)((tempVal & 0xFE000000) >> 25) == val)
                    {
                        return true;
                    }
				}
			}
		}
	}
	return false;
}

bool Hash_t::remove(unsigned int val)
{
    int index = getIndex(val);

    // for every element in the perspective bucket
    for(size_t i = 0; i < bucketList[index].size(); i++)
    {
        // if the element contains the value already
        if(bucketList[index][i] == val)
        {
			bucketList[index].erase(bucketList[index].begin() + i);
			_size--;
            return true;
        }
    }
    return false;
}

// if there are 4 empty status bits, compress????????????????????????????
bool Hash_t::remove(char val)
{
	int index = getIndex(val);

	int emptiesFound = 0; // if 4, reoder at the end of remove
	int e1i = 0;
	int e1j = 0;
	int e2i = 0;
	int e2j = 0;
	int e3i = 0;
	int e3j = 0;
	int destroyedi = 0;
	int destroyedj = 0;

	bool retVal = false;


	// for every element in the perspective bucket
    for(size_t i = 0; i < bucketList[index].size(); i++)
    {
        unsigned int tempVal = bucketList[index][i];
        // check each unsigned int's char flags
        for(size_t j = 0; j < 4; j++)
        {
            unsigned int flag = (tempVal & (unsigned int)pow(2.0, (double)j));
            if(flag)
            {
                if(j == 0) // Looking at: 0000000 0000000 0000000 1111111 0000
                {
                    if((char)((tempVal & 0x7F0) >> 4) == val)
                    {
                        bucketList[index][i] ^= 1U << j; // sets in use flag to 0
                		retVal = true;

						destroyedi = i;
						destroyedj = j;
						emptiesFound++;
					}
				}
                else if(j == 1) // Looking at: 0000000 0000000 1111111 0000000 0000
                {
                    if((char)((tempVal & 0x3F800) >> 11) == val)
                    {
                        bucketList[index][i] ^= 1U << j; // sets in use flag to 0
                        retVal = true;

						destroyedi = i;
                        destroyedj = j;
                        emptiesFound++;
                    }
                }
                else if(j == 2) // Looking at: 0000000 1111111 0000000 0000000 0000
                {
                    if((char)((tempVal & 0x1FC0000) >> 18) == val)
                    {
						bucketList[index][i] ^= 1U << j; // sets in use flag to 0
	                    retVal = true;

                        destroyedi = i;
                        destroyedj = j;
                        emptiesFound++;
					}
                }
                else if(j == 3) // Looking at: 1111111 0000000 0000000 0000000 0000
                {
                    if((char)((tempVal & 0xFE000000) >> 25) == val)
                    {
						bucketList[index][i] ^= 1U << j; // sets in use flag to 0
                        retVal = true;

                        destroyedi = i;
                        destroyedj = j;
                        emptiesFound++;
                    }
                }
            }
			else
			{
				if(emptiesFound <= 1)
				{
					int e1i = i;
					int e1j = j;
				}
				else if(emptiesFound <= 2)
				{
					int e2i = i;
					int e2j = j;
				}
				else if(emptiesFound <= 3)
				{
					int e3i = i;
					int e3j = j;
				}
				emptiesFound++;
			}
        }
    }
	if(emptiesFound == 4)
	{
		// Insert the smallest i's char's into the array.
			// temporarily store them, manually delete them, call insert to add them in from the bottom.
		// This is where I ran out of time.
	}
    return retVal;
}








Hash_t& Hash_t::operator-=(unsigned int rhs)
{
	this->remove(rhs);
	return *this;
}

Hash_t& Hash_t::operator-=(char rhs)
{
	this->remove(rhs);
	return *this;
}

string Hash_t::toString() const
{
	string retVal("");

	size_t largestNumDigits = to_string((int)(bucketList.size())).size();


	// for each bucket
	for(size_t i = 0; i < bucketList.size(); i++)
	{
		// add the bucket number with leading " "'s as necessary
		for(size_t j = 0; j < largestNumDigits - to_string(i).size(); j++)
		{
			retVal = retVal + " ";
		}
		retVal = retVal + to_string(i) + ": ";

		// add contents of bucket at index i
		if(bucketList[i].size() == 0)
		{
			retVal = retVal + "(Empty)\n";
		}
		else
		{
			for(size_t j = 0; j < bucketList[i].size(); j++)
			{
				retVal = retVal + to_string(bucketList[i][j]);

				// add space or new line after number
				if(j != bucketList[i].size() - 1)
				{
					retVal = retVal + " ";
				}
				else
				{
					if(i != bucketList.size()-1)
					{
	                    retVal = retVal + "\n";
					}
				}
			}
		}
	}
	return retVal;
}


string Hash_t::toString(char) const
{
	string retVal("");

    size_t largestNumDigits = to_string((int)(bucketList.size())).size();

    // for each bucket
    for(size_t i = 0; i < bucketList.size(); i++)
    {
        // add the bucket number with leading " "'s as necessary
        for(size_t j = 0; j < largestNumDigits - to_string(i).size(); j++)
        {
            retVal = retVal + " ";
        }
        retVal = retVal + to_string(i) + ": ";

        // add contents of bucket at index i
        if(bucketList[i].size() == 0)
        {
            retVal = retVal + "(Empty)\n";
        }
        else
        {
            for(size_t j = 0; j < bucketList[i].size(); j++)
            {
	            unsigned int tempVal = bucketList[i][j];
        		// check each unsigned int's char flags
        		for(size_t k = 0; k < 4; k++)
        		{
            		unsigned int flag = (tempVal & (unsigned int)pow(2.0, (double)k));
            		if(flag)
            		{
                		if(k == 0) // Looking at: 0000000 0000000 0000000 1111111 0000
                		{
							string tempString = string(1, (char)((tempVal & 0x7F0) >> 4));
                    		retVal = retVal + tempString;
                		}
                		else if(k == 1) // Looking at: 0000000 0000000 1111111 0000000 0000
                		{
                            string tempString = string(1, (char)((tempVal & 0x3F800) >> 11));
                            retVal = retVal + tempString;
                		}
                		else if(k == 2) // Looking at: 0000000 1111111 0000000 0000000 0000
                		{
                            string tempString = string(1, (char)((tempVal & 0x1FC0000) >> 18));
                            retVal = retVal + tempString;
                		}
                		else if(k == 3) // Looking at: 1111111 0000000 0000000 0000000 0000
                		{
                            string tempString = string(1, (char)((tempVal & 0xFE000000) >> 25));
                            retVal = retVal + tempString;
                		}
            		}
				}
                // add space or new line after char
                if(j != bucketList[i].size() - 1)
                {
                    retVal = retVal + " ";
                }
                else
                {
					if(i != bucketList.size()-1)
					{
	                    retVal = retVal + "\n";
					}
                }
            }
        }
    }
    return retVal;
}

string Hash_t::toString(unsigned int) const
{
	return toString();
}

size_t Hash_t::size() const
{
	return _size;
}

// ??????? getting error on compile.
ostream& Hash_t::operator<<(ostream& lhs, Hash_t& rhs)
{
	lhs << rhs.toString();
	return lhs;
}

int Hash_t::getIndex(unsigned int val) const
{
	return hashfunc(&val, sizeof(unsigned int)) % bucketList.size();
}

int Hash_t::getIndex(char val) const
{
	return hashfunc(&val, sizeof(char)) % bucketList.size();
}

int defaultHash(void* data, size_t size)
{
	int bytes = 0;
	for(size_t i=0; i<size; i++)
	{
		bytes += *(((char *)data) + i);
	}
	return bytes;
}

int basic_hash(void* data, size_t size)
{
	EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
  	unsigned  int md_len, i;
	md_len = 0;
	i = 0;
	unsigned int* ptr = (unsigned int*) md_value;

    OpenSSL_add_all_digests();

	if(!"sha1")
	{
    	printf("Usage: mdtest digestname\n");
        exit(1);
    }

    md = EVP_get_digestbyname("sha1");

    if(!md)
	{
    	printf("Unknown message digest %s\n", "sha1");
        exit(1);
    }

    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, size);
    EVP_DigestFinal_ex(mdctx, md_value,&md_len);
    EVP_MD_CTX_destroy(mdctx);

	unsigned int retVal = 0;
	md_len = md_len/4;
	for(i = 0; i < md_len; i++)
	{
		retVal ^= ptr[i];
	}
	return retVal;
}
