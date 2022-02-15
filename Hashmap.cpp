/** Kellyn Mendez - CS 2337.003, Project 4
 *      NetID: kpm200000
 */

#include "Hashmap.h"
#include <string>
#include <iostream>
#include <cmath>

Hashmap::Hashmap()
{
    // Creating the hash map with the start size
    hashTable = new User*[START_SIZE];
    hashSize = START_SIZE;
    filled = 0;
    // Initializing every index with null
    for (int i = 0; i < START_SIZE; i++)
    {
        hashTable[i] = nullptr;
    }
}

Hashmap::Hashmap(int size)
{
    // Creating the hash map with the given size
    hashTable = new User*[size];
    hashSize = size;
    filled = 0;
    // Initializing every index with null
    for (int i = 0; i < START_SIZE; i++)
    {
        hashTable[i] = nullptr;
    }
}

Hashmap::~Hashmap()
{
    // Visiting every index
    for (int i = 0; i < hashSize; i++)
    {
        // If that index holds a user
        if (hashTable[i] != nullptr)
        {
            // Deleting the user stored at the index
            delete hashTable[i];
        }
    }
    // Deleting the table
    delete hashTable;
}

int Hashmap::findKey(std::string name)
{
    int key = 0;
    double pwr = static_cast<double>(name.size());
    // Using formula to generate a unique code for strings
    for(int i = 0; i < static_cast<int>(name.size()); i++)
    {
        int s = static_cast<int>(name[i]);
        int add = s * static_cast<int>(pow(HASHCODE_CONST, pwr));
        key += add;
        pwr -= 1;
    }

    // Returning the key after it is calculated
    return abs(key);
}

int Hashmap::hashFn(int key)
{
    return key % hashSize;
}

void Hashmap::hashInsert(std::string name, std::string pw)
{
    // If the hashtable is filled is over the load factor, then resize
    if ((static_cast<double>(filled) / static_cast<double>(hashSize)) >= LOAD_FACTOR)
    {
        hashResize();
    }

    User* elem = new User();
    elem->username = name;
    elem->password = pw;
    int i = 0;
    // Getting the compressed key for the given name
    int k = hashFn(findKey(name));

    // Searching for an open spot to insert
    while (hashTable[(k+i*i) % hashSize] != nullptr)
    {
        i++;
    }

    // Inserting the user in the table
    int index = k + (i * i);
    hashTable[index % hashSize] = elem;
    filled++;
}

User* Hashmap::hashSearch(std::string name)
{
    int count = 0;
    int i = 0;
    int k = hashFn(findKey(name));
    bool exit = false;
    User* check;
    // While all of the spots in the hash table have not been checked and the user has not been found
    while (count <= hashSize && !exit)
    {
        // Getting the user at the index being checked
        check = hashTable[(k+i*i) % hashSize];
        // If the user has been found
        if (check != nullptr && check->username == name)
        {
            // Then the search ends, and the loop is exited
            exit = true;
        }
        else
        {
            // Otherwise, increment i and the number of spots in the table that have been checked
            i++;
            count++;
        }
    }

    // If the user was found, return the user
    if (exit)
    {
        return check;
    }
    // Otherwise, return null
    return nullptr;
}

void Hashmap::hashResize()
{
    int initialSize = hashSize;
    hashSize = hashSize * 2;
    // Getting the new size of the table (the next prime number that is greater
    //   than double the original hash table size)
    while (!isPrime(hashSize))
    {
        hashSize++;
    }

    // Copying the hashtable
    User** hashCopy = hashTable;
    // Creating a new hashtable in the original table variable
    hashTable = new User*[hashSize];
    // Initializing all of the spots in the new table to null
    for (int i = 0; i < hashSize; i++)
    {
        hashTable[i] = nullptr;
    }

    // For ever index in the copy, if there is a user at the index,
    //    then reinsert the user into the new table
    for (int i = 0; i < initialSize; i++)
    {
        if (hashCopy[i] != nullptr)
        {
            hashInsert(hashCopy[i]->username, hashCopy[i]->password);
        }
    }

    // Delete the copy table
    delete[] hashCopy;
    hashCopy = nullptr;
}

void Hashmap::hashPrint()
{
    // For every index in the hash table
    for (int index = 0; index < hashSize; index++)
    {
        // If the index does not contain a user, print "_"
        if (hashTable[index] == nullptr)
        {
            std::cout << "_";
        }
        else
        {
            // Otherwise, print the user's username
            User print = *(hashTable[index]);
            std::cout << print.username;
        }
        std::cout << " ";
    }
}

bool Hashmap::isPrime(int num)
{
    // Checking that the number given is not divisible by any other number
    //  (checking that it is a prime number)
    for (int div = 2; div < num; div++)
    {
        if (num % div == 0)
        {
            // If it is divisible by another number, then it is not prime
            return false;
        }
    }
    return true;
}


