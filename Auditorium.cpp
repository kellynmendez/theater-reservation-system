/** Auditorium class
 */

#include "Auditorium.h"
#include <string>
#include <fstream>
#include <iostream>

Auditorium::Auditorium(std::string filename)
{
    // Opening file to read auditorium
    std::ifstream infile;
    infile.open(filename);

    // If file exists and opened successfully
    if(infile.is_open())
    {
        numRows = 0;
        numCols = 0;

        // Reading the first line of the auditorium
        std::string str;
        if (getline(infile, str) && !str.empty())
        {
            // Variables for creating seats
            int index = 0;
            char letter = 'A';
            int row = 1;

            // Storing the first seat in the auditorium and initializing head
            Seat firstSeat(row, letter, str.at(index));
            Node<Seat> *first = new Node<Seat>(firstSeat);
            head = first;
            numCols++;

            // Increment seat letter
            letter = static_cast<char>(letter + 1);

            Node<Seat> *place = head;
            // For every remaining column in first row
            for(index = 1; index < static_cast<int>(str.size()); index++)
            {
                // Create new node and store corresponding seat in node
                Seat tempSeat(row, letter, str.at(index));
                Node<Seat> *tempNode = new Node<Seat>(tempSeat);

                // Setting node's previous to last place in list
                tempNode->setLeft(place);
                // Setting last place in list's next to new node
                place->setRight(tempNode);
                // Storing new node as last place in list and incrementing number of seats
                place = tempNode;
                numCols++;

                // Incrementing seat letter
                letter = static_cast<char>(letter + 1);
            }

            // Incrementing row
            row++;

            // Reading remaining rows in auditorium
            Node<Seat> *above = head;
            while(getline(infile, str) && !str.empty())
            {
                letter = 'A';
                index = 0;

                // Creating new node and storing corresponding seat in node
                Seat newSeat(row, letter, str.at(index));
                Node<Seat> *firstNode = new Node<Seat>(newSeat);
                place = firstNode;

                // Setting new node's up to node above
                place->setUp(above);
                // Setting node above's down to new node
                above->setDown(place);
                // Setting above to second node in row
                above = above->getRight();

                // Incrementing seat letter
                letter = static_cast<char>(letter + 1);

                // For every remaining column in row
                for(index = 1; index < static_cast<int>(str.size()); index++)
                {
                    // Creating new node and storing corresponding seat in node
                    Seat tempSeat(row, letter, str[index]);
                    Node<Seat> *tempNode = new Node<Seat>(tempSeat);

                    // Setting node's previous to last place in list
                    tempNode->setLeft(place);
                    // Setting last place in list's next to new node
                    place->setRight(tempNode);
                    // Setting node's up to node above
                    tempNode->setUp(above);
                    // Setting node above's down to new node
                    above->setDown(tempNode);

                    // Storing new node as last place in list
                    place = tempNode;

                    // Moving above node to its next
                    above = above->getRight();

                    // Incrementing seat letter
                    letter = static_cast<char>(letter + 1);
                }

                // Incrementing row
                row++;
                // Setting above to first node in finished row
                above = firstNode;
            }

            // Setting number of rows
            numRows = row - 1;

        }
        else // If file is empty, initializes instance variables to default values
        {
            head = nullptr;
            numRows = 0;
            numCols = 0;
        }
    }
    else // If file is empty, initializes instance variables to default values
    {
        head = nullptr;
        numRows = 0;
        numCols = 0;
    }

    // Closing file
    infile.close();
}

Auditorium::~Auditorium()
{
    // Setting place to the second seat in the row
    Node<Seat> *place = head->getRight();
    Node<Seat> *above = head->getDown();
    bool keepGoing = false;

    // While there are more rows in linked list, deletes entire row except for the first seat
    while (above != nullptr || keepGoing)
    {
        // While current node is not equal to null
        while (place != nullptr)
        {
            Node<Seat>* toDel = place;
            place = place->getRight();
            delete toDel;
        }

        // If above is null, then there are no more rows to delete
        if (above == nullptr)
        {
            keepGoing = false;
        }
            // If above's down is null, still need to delete the last row
        else if (above->getDown() == nullptr)
        {
            keepGoing = true;
            above = above->getDown();
        }
        else
        {
            // Resetting above to first seat of this row
            above = above->getDown();
            // Resetting place to second seat of next row
            place = above->getRight();
        }
    }

    // Deleting all of the first seats
    while(head != nullptr)
    {
        Node<Seat>* temp = head;
        head = head->getDown();
        delete temp;
    }

}
