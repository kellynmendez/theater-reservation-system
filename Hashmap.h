/** Hashmap class header file
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include "Seat.h"
#include <string>
#include <vector>

// Structs
struct Order
{
    int auditorium;
    int adults;
    int children;
    int seniors;
    std::vector<Seat> seatsList;
};

struct User
{
    std::string username;
    std::string password;
    std::vector<Order> ordersList;
};

class Hashmap
{
private:
    // Class constants
    const static int START_SIZE = 29;
    const double LOAD_FACTOR = 0.5;
    const double HASHCODE_CONST = 31.0;
    // Members
    User** hashTable;
    int hashSize;
    int filled;

public:
    // Constructors/Destructors
    Hashmap();
    Hashmap(int size);
    ~Hashmap();

    // Accessors
    User** getHashTable() const {return hashTable;}
    int getHashSize() const {return hashSize;}

    // Other functions
    int findKey(std::string name);
    int hashFn(int key);
    void hashInsert(std::string name, std::string pw);
    User* hashSearch(std::string name);
    void hashResize();
    void hashPrint();
    bool isPrime(int num);
};



#endif //HASHMAP_H
