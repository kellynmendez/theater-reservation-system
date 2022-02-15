/** Auditorium class header file
 */

#ifndef AUDITORIUM_H
#define AUDITORIUM_H
#include "Node.h"
#include "Seat.h"
#include <string>

class Auditorium
{
private:
    Node<Seat> *head;
    int numRows;
    int numCols;
public:
    // Constructors
    Auditorium(){head = nullptr; numRows = 0; numCols = 0;}
    Auditorium(std::string filename);
    ~Auditorium();

    // Accessor
    Node<Seat>* getHead() const {return head;}
    int getNumRows() const {return numRows;}
    int getNumCols() const {return numCols;}

    // Mutator
    void setHead(Node<Seat> *newHead){head = newHead;}
};

#endif //AUDITORIUM_H
