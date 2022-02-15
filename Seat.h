/** Kellyn Mendez - CS 2337.003, Project 4
 *      NetID: kpm200000
 */

#ifndef SEAT_H
#define SEAT_H

#include <ostream>

class Seat
{
private:
    int row;
    char seat;
    char type;

public:
    // Constructors
    Seat() {row = 0; seat = ' '; type = ' ';}
    Seat(int r, char s, char t);

    // Accessors
    int getRow() const {return row;}
    char getSeat() const {return seat;}
    char getTicketType() const {return type;}

    // Mutators
    void setRow(int r) {row = r;}
    void setSeat(char s) {seat = s;}
    void setTicketType(char t) {type = t;}
};

#endif //SEAT_H
