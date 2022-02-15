/** Allows users to log in, reserve seats in three different auditoriums, view and update orders, and choose to display receipts.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include "Node.h"
#include "Seat.h"
#include "Auditorium.h"
#include "Hashmap.h"
using namespace std;

// Global Constants
const int PASSWORD_ATTEMPTS = 3;    // Number of attempts a user gets to input their password
const double ADULT_PRICE = 10.00;   // Price for adult tickets
const double CHILD_PRICE = 5.00;    // Price for child tickets
const double SENIOR_PRICE = 7.50;   // Price for senior tickets
const int CHAR_TO_INDEX = 65;       // Conversion from character value to index

// Functions
void displayAuditorium(Auditorium*);
void getInput(int& row, int& adultTick, int& childTick, int& seniorTick, char& letter, int numCols, int numRows);
bool checkAvailability(Auditorium*, int, char, int);
void reserveSeats(Auditorium*&, Order&, int, char, int, int, int);
Node<Seat>* getBestAvailable(Auditorium*, int);
void removeReservedSeat(Auditorium*&, User*&, vector<Seat>::iterator&, int, int, char);
double calculateDistance(double, double, double, double);
double printOrder(Order);
void displayReport(Auditorium*, int&, int&, int&, int&, int&, double&);

int main()
{
    // Declared HashMap and input file stream
    Hashmap* users = new Hashmap();
    ifstream infile;

    // Open and read in the file
    infile.open("userdb.dat");
    // If the file opened successfully
    if (infile.is_open())
    {
        string userInfo;
        // For every line in the file, insert the user into the hash table
        while (getline(infile, userInfo) && !userInfo.empty())
        {
            // Create a string stream to read in the information from the line
            istringstream stream(userInfo);
            string name, pw;
            // Read in the username, then the password
            stream >> name;
            stream >> pw;

            // Inserting user into the hash table
            users->hashInsert(name, pw);
        }
        infile.close();

        // Create all of the auditorium objects
        Auditorium* auditorium1 = new Auditorium("A1.txt");
        Auditorium* auditorium2 = new Auditorium("A2.txt");
        Auditorium* auditorium3 = new Auditorium("A3.txt");

        bool exitProgram = false;
        while (!exitProgram)
        {
            User* theUser;
            string name;
            bool loggedIn = false;
            // While the user has not logged in successfully
            while (!loggedIn)
            {
                // Prompt user for username
                cout << "Username: ";
                cin >> name;

                // Get the user from the hash table
                theUser = users->hashSearch(name);

                int count = 0;
                bool validPw = false;
                // While the password is not valid and the user has not exceeded the maximum
                //    number of attempts allowed
                while (!validPw && count < PASSWORD_ATTEMPTS)
                {
                    // Prompt user for password
                    string pw;
                    cout << "Password: ";
                    cin >> pw;

                    // Validate that the user's password is correct
                    if (theUser->password == pw)
                    {
                        validPw = true;
                        loggedIn = true;
                    }
                    // If it's not correct, increment the current number of attempts by one
                    else
                    {
                        cout << "Invalid password" << endl;
                        count++;
                    }

                    // If the number of attempts is equal to the max number of attempts, end the loop
                    // and start back at the username login
                    if (count == PASSWORD_ATTEMPTS)
                    {
                        cout << "You have run out of attempts." << endl;
                    }
                }
            }

            cout << endl;
            // If the user is not the admin
            if (name != "admin")
            {
                bool validMainChoice = false;
                while (!validMainChoice)
                {
                    // Display the main menu
                    cout << "Main Menu" << endl;
                    cout << "1. Reserve Seats" << endl;
                    cout << "2. View Orders" << endl;
                    cout << "3. Update Order" << endl;
                    cout << "4. Display Receipt" << endl;
                    cout << "5. Log Out" << endl;
                    cout << "Enter choice here: ";

                    // Get the user's choice
                    string choice;
                    cin >> choice;
                    cout << "CHOICE " << choice << endl;

                    // If the user's choice is 1, then they would like to reserve seats in an auditorium
                    if (choice == "1")
                    {
                        cout << endl;

                        // Display auditorium submenu
                        cout << "1. Auditorium 1" << endl;
                        cout << "2. Auditorium 2" << endl;
                        cout << "3. Auditorium 3" << endl;
                        cout << "Enter choice here: ";

                        Auditorium* selected;
                        // Declaring order struct
                        Order userOrder;
                        // Initializing the members of the order
                        userOrder.adults = 0;
                        userOrder.children = 0;
                        userOrder.seniors = 0;

                        bool validChoice = false;
                        while (!validChoice)
                        {
                            // Get the user's choice
                            cin >> choice;
                            cout << "CHOICE " << choice << endl;

                            // If the user's choice is 1, they would like to reserve seats in Auditorium 1
                            if (choice == "1")
                            {
                                // Display auditorium 1
                                displayAuditorium(auditorium1);
                                // Set the auditorium object being used to auditorium 1 and initialize the order's
                                // auditorium number to 1
                                selected = auditorium1;
                                userOrder.auditorium = 1;
                                validChoice = true;
                            }
                            // If the user's choice is 2, they would like to reserve seats in Auditorium 2
                            else if (choice == "2")
                            {
                                // Display auditorium 1
                                displayAuditorium(auditorium2);
                                // Set the auditorium object being used to auditorium 2 and initialize the order's
                                // auditorium number to 2
                                selected = auditorium2;
                                userOrder.auditorium = 2;
                                validChoice = true;
                            }
                            // If the user's choice is 3, they would like to reserve seats in Auditorium 3
                            else if (choice == "3")
                            {
                                // Display auditorium 1
                                displayAuditorium(auditorium3);
                                // Set the auditorium object being used to auditorium 3 and initialize the order's
                                // auditorium number to 3
                                selected = auditorium3;
                                userOrder.auditorium = 3;
                                validChoice = true;
                            }
                        }

                        // Declaring necessary variables to get input
                        int row, adultTick, childTick, seniorTick;
                        char letter;
                        // Getting the size of the auditorium to validate input
                        int numRows = selected->getNumRows();
                        int numCols = selected->getNumCols();

                        // Getting the input from the user
                        getInput(row, adultTick, childTick, seniorTick, letter, numCols, numRows);

                        // Checking if seats are available for the requested reservation
                        bool checkAvail = checkAvailability(selected, row, letter, (adultTick + childTick + seniorTick));
                        if (checkAvail)
                        {
                            // Reserving the seats
                            reserveSeats(selected, userOrder, row, letter, adultTick, childTick, seniorTick);
                            cout << "Your seats have been reserved!" << endl << endl;
                            // Adding the order to the user's vector of orders
                            theUser->ordersList.push_back(userOrder);
                        }
                        else
                        {
                            // If seats are not available, get the best available seats in user's requested row
                            Node<Seat>* checkSeat = getBestAvailable(selected,(adultTick + childTick + seniorTick));
                            // If there are no best available seats in that row
                            if (checkSeat == nullptr)
                            {
                                // Informing the user of no best available seats
                                cout << "There are no seats available for the chosen reservation." << endl;
                                cout << "There are also no best available seats in that row." << endl;
                            }
                            else
                            {
                                // If there are best available seats in that row, displays seats for the user
                                char seat = checkSeat->getValue().getSeat();
                                cout << "There are no seats available for the chosen reservation." << endl;

                                // Displaying best seats available
                                // If the reservation is only one seat
                                if ((adultTick + childTick + seniorTick) == 1)
                                {
                                    cout << "This is the best seat available: ";
                                    cout << checkSeat->getValue().getRow() << seat << endl;
                                }
                                // If the reservation is more than one seat
                                else
                                {
                                    cout << "These are the best seats available: ";
                                    char endSeat = static_cast<char>(seat + adultTick + childTick + seniorTick - 1);
                                    cout << checkSeat->getValue().getRow() << seat << "-" << checkSeat->getValue().getRow() << endSeat << endl;
                                }

                                bool exitReserved = false;
                                while (!exitReserved)
                                {
                                    // Prompting user for whether or not they would like to reserve best available
                                    cout << "Reserve these seats instead (Y/N)? ";
                                    cin >> choice;

                                    // If user would like to reserve the best available seats
                                    if (choice == "Y" || choice == "y")
                                    {
                                        //int row, char seat, int adultTick, int childTick, int seniorTick
                                        reserveSeats(selected, userOrder, checkSeat->getValue().getRow(), seat, adultTick, childTick, seniorTick);
                                        cout << "Your seats have been reserved." << endl;
                                        // Adding the order to the user's vector of orders
                                        theUser->ordersList.push_back(userOrder);
                                        exitReserved = true; // User's input was valid, so exit
                                    }
                                        // If user would not like to reserve the seats
                                    else if (choice == "N" || choice == "n")
                                    {
                                        cout << "Those seats were not reserved." << endl;
                                        exitReserved = true; // User's input was valid, so exit
                                    }
                                }
                            }
                        }
                        cout << endl;
                    }
                    // If the user's choice is 2, then they would like to view their orders
                    else if (choice == "2")
                    {
                        // Getting the user's list of orders
                        vector<Order> userOrders = theUser->ordersList;

                        // If the user does not have any orders, print "No orders"
                        if (userOrders.empty())
                        {
                            cout << "No orders." << endl << endl;
                        }
                        // Otherwise, print the orders
                        else
                        {
                            cout << endl;
                            vector<Order>::iterator iter;
                            // For every order in the vector, print it according to the format required
                            for (iter = userOrders.begin(); iter != userOrders.end(); iter++)
                            {
                                Order orderPrint = *iter;
                                printOrder(orderPrint);
                                cout << endl;
                            }
                        }
                    }
                        // If the user's choice is 3, then they would like to update an order
                    else if (choice == "3")
                    {
                        // Getting the user's list of orders
                        int count;

                        // If the user does not have any orders, print "No orders"
                        if (theUser->ordersList.empty())
                        {
                            cout << "No orders." << endl;
                        }
                        // Otherwise, print the orders
                        else
                        {
                            // Prompting the user to choose an order
                            cout << "Please choose one of the following orders to update." << endl;

                            vector<Order>::iterator iter;
                            // Counter variable to count the total number of orders
                            count = 1;
                            cout << endl;
                            // For every order in the vector, print it
                            for (iter = theUser->ordersList.begin(); iter != theUser->ordersList.end(); iter++)
                            {
                                // Getting the order
                                Order orderPrint = *iter;
                                // Printing the order
                                printOrder(orderPrint);
                                cout << endl;
                                // Incrementing the number of orders by one
                                count++;
                            }

                            bool validOrder = false;
                            int orderNum;
                            // Validating the order number
                            while (!validOrder)
                            {
                                // Getting the order
                                cout << "Enter order number here: ";
                                cin >> orderNum;
                                // If the order number is in the correct range of orders, then it is valid
                                if (orderNum > 0 || orderNum <= count)
                                {
                                    validOrder = true;
                                }
                            }

                            // Displaying update order submenu
                            cout << "1. Add tickets to order" << endl;
                            cout << "2. Delete tickets from order" << endl;
                            cout << "3. Cancel Order" << endl;
                            cout << "Enter choice here: ";

                            // Getting the user's choice
                            cin >> choice;
                            cout << "CHOICE " << choice << endl;

                            bool validChoice = false;
                            // Validating the choice chosen
                            while (!validChoice)
                            {
                                Auditorium *selected;
                                // Getting the auditorium of the order
                                if (theUser->ordersList.at(orderNum - 1).auditorium == 1)
                                {
                                    selected = auditorium1;
                                }
                                else if (theUser->ordersList.at(orderNum - 1).auditorium == 2)
                                {
                                    selected = auditorium2;
                                }
                                else
                                {
                                    selected = auditorium3;
                                }

                                // If the user chose 1, then they would like to add tickets to the order
                                if (choice == "1")
                                {
                                    validChoice = true;
                                    cout << endl;
                                    // Displaying the auditorium
                                    displayAuditorium(selected);

                                    // Declaring necessary variables for getting input
                                    int row, adultTick, childTick, seniorTick;
                                    char letter;
                                    int numRows = selected->getNumRows();
                                    int numCols = selected->getNumCols();

                                    // Getting input from the user
                                    getInput(row, adultTick, childTick, seniorTick, letter, numCols, numRows);

                                    // Checking if seats are available for the requested reservation
                                    bool checkAvail = checkAvailability(selected, row, letter,(adultTick + childTick + seniorTick));
                                    if (checkAvail)
                                    {
                                        // Reserving the seats
                                        reserveSeats(selected, theUser->ordersList.at(orderNum - 1), row, letter, adultTick, childTick, seniorTick);
                                        cout << "Your seats have been reserved!" << endl << endl;
                                    }
                                    else
                                    {
                                        cout << "There are no seats available for the chosen reservation." << endl;
                                    }
                                    cout << endl;
                                }
                                // If the user chose 2, then they would like to delete tickets in the order
                                else if (choice == "2")
                                {
                                    validChoice = true;

                                    // Prompting for the row number of the seat to unreserve
                                    cout << "Row number: ";
                                    string temp;
                                    int row = -1;

                                    // Clearing buffer
                                    getline(cin, temp);

                                    // Storing user's input in a temporary string
                                    getline(cin, temp);

                                    bool allDigits = true;
                                    // Checks that all characters are digits; if not, then input is not valid
                                    for (int i = 0; i < static_cast<int>(temp.size()) && allDigits; i++)
                                    {
                                        if (!isdigit(temp.at(i)))
                                        {
                                            allDigits = false;
                                        }
                                    }
                                    // If all characters are digits
                                    if (allDigits)
                                    {
                                        // Stores temp as an integer in row variable
                                        row = stoi(temp);
                                    }

                                    // Prompting for the seat letter of the seat to unreserve
                                    cout << "Starting seat letter: ";

                                    // Storing user's input in a temporary string
                                    getline(cin, temp);

                                    char letter = '.';
                                    // If the string is of size 1 and is an uppercase letter, then store the string in a character
                                    if(static_cast<int>(temp.size()) == 1 && isupper(temp.at(0)))
                                    {
                                        letter = temp.at(0);
                                    }

                                    // If the letter and row were not successfully initialized to valid values
                                    if (letter == '.' || row == -1)
                                    {
                                        cout << "Incorrect input: " << row << letter << endl;
                                    }
                                    // Otherwise, remove the seat from the seat list if it exists
                                    else
                                    {
                                        vector<Seat>::iterator iter2;
                                        bool correctSeat = false;
                                        // For every seat in the order
                                        for (iter2 = theUser->ordersList.at(orderNum - 1).seatsList.begin();
                                             !correctSeat && iter2 != theUser->ordersList.at(orderNum - 1).seatsList.end(); iter2++)
                                        {
                                            Seat check = *iter2;
                                            // If the seat is the same as the one the user would like to unreserve
                                            if (check.getSeat() == letter && check.getRow() == row)
                                            {
                                                // Cancelling the ticket order
                                                correctSeat = true;
                                                // Removing the seat from the order, and marking it as available in the auditorium
                                                removeReservedSeat(selected, theUser, iter2, orderNum, row, letter);
                                            }
                                        }

                                        // If the user didn't give a seat they've reserved
                                        if (!correctSeat)
                                        {
                                            cout << "Incorrect input: " << row << letter << endl;
                                        }
                                        // If the seat was successfully unreserved
                                        else
                                        {
                                            cout << "Your tickets have been deleted.";
                                            cout << row << letter << endl;
                                        }
                                    }
                                }
                                // If the user chose 3, then they would like to cancel the order
                                else if (choice == "3")
                                {
                                    validChoice = true;
                                    vector<Seat>::iterator iter2;

                                    // Getting the number of seats in the order to be cancelled
                                    int size = static_cast<int>(theUser->ordersList.at(orderNum - 1).seatsList.size());

                                    // For every seat in the order
                                    while (size > 0)
                                    {
                                        iter2 = theUser->ordersList.at(orderNum - 1).seatsList.begin();
                                        Seat check = *iter2;
                                        // Remove the seat from the order
                                        removeReservedSeat(selected, theUser, iter2, orderNum, check.getRow(), check.getSeat());
                                        // Decrement the size by 1
                                        size--;
                                    }

                                    // Inform the user that their seats were successfully cancelled
                                    cout << "Your order has been cancelled." << endl << endl;
                                }
                            }
                        }
                    }
                    // If the user's choice is 4, then they would like to see their receipt
                    else if (choice == "4")
                    {
                        double totalSpent = 0;
                        double orderCost;
                        // Getting the user's list of orders
                        vector<Order> userOrders = theUser->ordersList;
                        vector<Order>::iterator iter;

                        cout << endl;
                        // For every order in the vector, print it according to the format required
                        for (iter = userOrders.begin(); iter != userOrders.end(); iter++)
                        {
                            Order orderPrint = *iter;
                            // Printing order, storing the cost of the order, and adding to total spent
                            orderCost = printOrder(orderPrint);
                            totalSpent += orderCost;
                            // Printing the cost of the order
                            cout << "Order Total: ";
                            printf("$%-4.2f", orderCost);
                            cout << endl << endl;
                        }

                        // Printing the total amount spent (all order costs combined)
                        cout << "Customer Total: ";
                        printf("$%-4.2f", totalSpent);
                        cout << endl << endl;
                    }
                    // If the user's choice is 5, then they would like to exit the program
                    else if (choice == "5")
                    {
                        validMainChoice = true;
                    }
                }
            }
            // If the user is the admin
            else
            {
                bool validChoice = false;
                while (!validChoice)
                {
                    string choice;
                    // Print admin main menu
                    cout << "1. Print Report" << endl;
                    cout << "2. Logout" << endl;
                    cout << "3. Exit" << endl;
                    cout << "Enter choice here: ";
                    cin >> choice;
                    cout << "CHOICE " << choice << endl;

                    // If the choice is 1, then the admin would like to print the report
                    if (choice == "1")
                    {
                        cout << endl << "Full report: " << endl;
                        int openSeats = 0, resSeats = 0, adultSeats = 0, childSeats = 0, seniorSeats = 0;
                        double totalCost = 0;
                        // Display the report for auditorium 1
                        cout << "Auditorium 1\t";
                        displayReport(auditorium1, openSeats, resSeats, adultSeats,
                                      childSeats, seniorSeats, totalCost);
                        // Display the report for auditorium 2
                        cout << endl << "Auditorium 2\t";
                        displayReport(auditorium2, openSeats, resSeats, adultSeats,
                                      childSeats, seniorSeats, totalCost);
                        // Display the report for auditorium 3
                        cout << endl << "Auditorium 3\t";
                        displayReport(auditorium3, openSeats, resSeats, adultSeats,
                                      childSeats, seniorSeats, totalCost);
                        cout << endl;

                        // Displaying totals across all theaters
                        cout << "Total\t\t\t" << openSeats << "\t" << resSeats;
                        cout << "\t" << adultSeats << "\t" << childSeats << "\t" << seniorSeats;
                        printf("\t$%-4.2f", totalCost);
                        cout << endl << endl;
                    }
                    // If the choice is 2, then the admin would like to logout
                    else if (choice == "2")
                    {
                        validChoice = true;
                        cout << endl;
                    }
                    // If the choice is 3, then exit the program
                    else if (choice == "3")
                    {
                        validChoice = true;
                        exitProgram = true;
                    }
                }
            }
        }

        // Writing auditorium 1 to an output file
        ofstream outfile("A1Final.txt");
        Node<Seat> *place = auditorium1->getHead();
        Node<Seat> *above = place;
        // While there are more rows in linked list
        while (above != nullptr)
        {
            // While current node is not equal to null
            while (place != nullptr)
            {
                // Writing ticket type to file
                outfile << place->getValue().getTicketType();
                place = place->getRight();
            }
            outfile << endl;

            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
        outfile.close();

        // Writing auditorium 2 to an output file
        outfile.open("A2Final.txt");
        place = auditorium2->getHead();
        above = place;
        // While there are more rows in linked list
        while (above != nullptr)
        {
            // While current node is not equal to null
            while (place != nullptr)
            {
                // Writing ticket type to file
                outfile << place->getValue().getTicketType();
                place = place->getRight();
            }
            outfile << endl;

            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
        outfile.close();

        // Writing auditorium 3 to an output file
        outfile.open("A3Final.txt");
        place = auditorium3->getHead();
        above = place;
        // While there are more rows in linked list
        while (above != nullptr)
        {
            // While current node is not equal to null
            while (place != nullptr)
            {
                // Writing ticket type to file
                outfile << place->getValue().getTicketType();
                place = place->getRight();
            }
            outfile << endl;

            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
        outfile.close();
    }

    delete users;
    return 0;
}


/* Displays the auditorium, with column header and row numbers, showing which seats are
* reserved and which are free
*      Seats that are reserved are represented by a '#', and free seats are represented
*      by a '.'
* @param theater, Auditorium object that holds linked list of seating available in the theater
*/
void displayAuditorium(Auditorium* theater)
{
    // Displaying the column header
    cout << "  ";
    Node<Seat> *place = theater->getHead();
    Node<Seat> *above = place;

    // Printing the column header line by iterating through first row of theater
    // While current node isn't null, prints seat letter
    while(place != nullptr)
    {
        cout << place->getValue().getSeat();
        place = place->getRight();
    }
    cout << endl;

    // Resetting place
    place = above;

    /* Iterating through entire theater; For each row, displays the row number at the beginning,
     * and then displays each row, changing any non-periods to '#'
     */
    while (above != nullptr)
    {
        // Displaying the row number
        cout << above->getValue().getRow() << " ";

        // While current node is not equal to null
        while (place != nullptr)
        {
            // If seat is not reserved, prints '.'
            if (place->getValue().getTicketType() == '.')
            {
                cout << '.';
            }
                // If seat is reserved, prints '#'
            else
            {
                cout << '#';
            }
            // Setting place to next seat
            place = place->getRight();
        }
        cout << endl;

        // Resetting place to first seat of next row
        place = above->getDown();
        // Resetting above to first seat of this row
        above = place;
    }

    cout << endl;
}

/** Getting all the input necessary to reserve seats in the auditorium, initializes all fo the
*      parameters except for numRows and numCols
* @param row, the row to be reserved
* @param adultTick, the number of adult tickets to be reserved
* @param childTick, the number of child tickets to be reserved
* @param seniorTick, the number of senior tickets to be reserved
* @param letter, the starting seat letter of the reservation
* @param numCols, the number of seats in a row in the auditorium
* @param numRows, the number of rows in the auditorium
*/
void getInput(int& row, int& adultTick, int& childTick, int& seniorTick, char& letter, int numCols, int numRows)
{
    bool checkInput = false;

    // Clearing buffer
    string temp;
    getline(cin, temp);

    // Prompting user for row and validating input
    while (!checkInput)
    {
        cout << "Row number: ";
        // Storing user's input in a temporary string
        getline(cin, temp);
        bool allDigits = true;
        // Checks that all characters are digits; if not, then input is not valid
        for (int i = 0; i < static_cast<int>(temp.size()) && allDigits; i++)
        {
            if (!isdigit(temp.at(i)))
            {
                allDigits = false;
            }
        }
        // If all characters are digits
        if (allDigits)
        {
            // Stores temp as an integer in row variable
            row = stoi(temp);
            // If row is a valid row number in the theater, then input is valid
            if (row > 0 && row <= numRows)
            {
                checkInput = true;
            }
        }
    }

    // Prompting user for seat letter and validating input
    checkInput = false;
    while (!checkInput)
    {
        cout << "Starting seat letter: ";
        // Storing user's input in a temporary string
        getline(cin, temp);

        if(static_cast<int>(temp.size()) == 1 && isupper(temp.at(0)))
        {
            letter = temp.at(0);
            char endSeat = static_cast<char>('A' + (numCols - 1));
            if (letter >= 'A' && letter <= endSeat)
                checkInput = true;
        }
    }

    // Prompting user for number of adult tickets and validating input
    checkInput = false;
    while (!checkInput)
    {
        cout << "Number of adult tickets: ";
        // Storing user's input in a temporary string
        getline(cin, temp);

        bool allDigits = true;
        // Checks that all characters are digits; if not, then input is not valid
        for (int i = 0; i < static_cast<int>(temp.size()) && allDigits; i++)
        {
            if (!isdigit(temp.at(i)))
            {
                allDigits = false;
            }
        }
        // If all characters are digits
        if (allDigits)
        {
            // Stores temp as an integer in ticket variable
            adultTick = stoi(temp);
            // If number of tickets to reserve is not negative, then ticket number is valid
            if (adultTick >= 0)
            {
                checkInput = true;
            }
        }
    }

    // Prompting user for number of child tickets and validating input
    checkInput = false;
    while (!checkInput)
    {
        cout << "Number of child tickets: ";
        // Storing user's input in a temporary string
        getline(cin, temp);

        bool allDigits = true;
        // Checks that all characters are digits; if not, then input is not valid
        for (int i = 0; i < static_cast<int>(temp.size()) && allDigits; i++)
        {
            if (!isdigit(temp.at(i)))
            {
                allDigits = false;
            }
        }
        // If all characters are digits
        if (allDigits)
        {
            // Stores temp as an integer in ticket variable
            childTick = stoi(temp);
            // If number of tickets to reserve is not negative, then ticket number is valid
            if (childTick >= 0)
            {
                checkInput = true;
            }
        }
    }

    // Prompting user for number of senior tickets and validating input
    checkInput = false;
    while (!checkInput)
    {
        cout << "Number of senior tickets: ";
        // Storing user's input in a temporary string
        getline(cin, temp);

        bool allDigits = true;
        // Checks that all characters are digits; if not, then input is not valid
        for (int i = 0; i < static_cast<int>(temp.size()) && allDigits; i++)
        {
            if (!isdigit(temp.at(i)))
            {
                allDigits = false;
            }
        }
        // If all characters are digits
        if (allDigits)
        {
            // Stores temp as an integer in ticket variable
            seniorTick = stoi(temp);
            // If number of tickets to reserve is not negative, then ticket number is valid
            if (seniorTick >= 0)
            {
                checkInput = true;
            }
        }
    }
}

/** Checks whether or not the seats being requested are available for reservation in the theater
* @param theater, Auditorium object that holds linked list of seating available in the theater
* @param row, the row number to be reserved
* @param seat, the seat letter to be reserved
* @param total, the total number of tickets in the reservation
* @return whether or not the seats requested are available for reservation
*/
bool checkAvailability(Auditorium* theater, int row, char seat, int total)
{
    // Finding first seat in reservation
    Node<Seat> *place = theater->getHead();
    Node<Seat> *above = place;
    bool found = false;

    // While there are more rows in linked list
    while (above != nullptr && !found)
    {
        // While current node is not equal to null
        while (place != nullptr && !found)
        {
            // If current seat is the first seat in the reservation
            if (place->getValue().getSeat() == seat && place->getValue().getRow() == row)
            {
                found = true;
            }
            else
            {
                // Updating current node to next node in row
                place = place->getRight();
            }
        }

        if (!found)
        {
            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
    }

    // Calculating total number of seats to be reserved
    int count = 0;

    /** Checking whether the reservation continues into already reserved seats
     *      While the current node is before the ending node of the reservation,
     *      check if the current seat is reserved; if reserved, returns false
     */
    while (count < total)
    {
        if (place->getValue().getTicketType() != '.')
        {
            return false; // If seat is reserved, returns false
        }
        // Setting node being checked to next node
        if (place->getRight() == nullptr && (count + 1) == total)
        {
            return true;
        }
        place = place->getRight();
        if (place == nullptr)
        {
            return false;
        }
        count++;
    }

    // If no seats in the requested reservation were previously reserved, returns true
    return true;
}

/** Reserves seats in the theater based on given user input that provides information about seats
*  to be reserved (seats are reserved as adults first, then children, then seniors)
* @param theater, Auditorium object that holds linked list of seating available in the theater
* @param userOrder, the order of seats to be resrved
* @param row, the row number to be reserved
* @param seat, the seat letter to be reserved
* @param adultTick, the number of adult tickets
* @param childTick, the number of child tickets
* @param seniorTick, the number of senior tickets
*/
void reserveSeats(Auditorium*& theater, Order& userOrder, int row, char seat, int adultTick, int childTick, int seniorTick)
{
    // Initializing each of the order's members
    userOrder.adults += adultTick;
    userOrder.children += childTick;
    userOrder.seniors += seniorTick;

    // Finding first seat in reservation
    Node<Seat> *place = theater->getHead();
    Node<Seat> *above = place;
    bool found = false;

    // While there are more rows in linked list
    while (above != nullptr && !found)
    {
        // While current node is not equal to null
        while (place != nullptr && !found)
        {
            // If current seat is the first seat in the reservation
            if (place->getValue().getSeat() == seat && place->getValue().getRow() == row)
            {
                found = true;
            }
            else
            {
                // Updating current node to next node in row
                place = place->getRight();
            }
        }

        if (!found)
        {
            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
    }

    // Calculating total number of seats to be reserved
    int total = adultTick + childTick + seniorTick;
    int count = 0;

    // Checking if the list is empty or not, in order to insert the seat in the correct order
    bool inserted;
    if (userOrder.seatsList.empty())
    {
        inserted = false;
    }
    else
    {
        inserted = true;
    }

    /* Reserving seats by starting at row/seat asked for
     *      While the amount of reservations made is less than the total amount of reservations to be made,
     *      reserves adult, then child, then senior ticket
     */
    while (count < total)
    {
        // First reserving adult tickets
        if (adultTick > 0)
        {
            // Creating seat object to add to theater
            Seat s(place->getValue().getRow(), place->getValue().getSeat(), 'A');
            // Adding seat to theater and adding seat to order's seat vector
            place->setValue(s);
            // If the seat list is empty
            if (!inserted)
            {
                // Add the seat to the order
                userOrder.seatsList.push_back(s);
                inserted = true;
            }
            // If the seat list is not empty
            else
            {
                // Getting the size of the seat vector, and declaring an iterator in order to traverse the vector
                int size = static_cast<int>(userOrder.seatsList.size());
                vector<Seat>::iterator iter = userOrder.seatsList.begin();

                // While the size of the list is greater than 0, and the row of the seat to be inserted is greater than the
                //  current position in the vector being checked
                while (size > 0 && s.getRow() > (*iter).getRow())
                {
                    // Move position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // While the size of the list is greater than 0, and the row of the seat to be inserted is equal to the
                //  current position in the vector being checked, keep looking for the insertion point until the
                //  the letter of the seat being inserted is less than that of the position being currently checked
                while (size > 0 && s.getRow() == (*iter).getRow() && s.getSeat() > (*iter).getSeat())
                {
                    // Move the position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // Insert the seat at the position
                userOrder.seatsList.insert(iter, s);
            }
            // Decrementing remaining adult tickets to be reserved
            adultTick--;
            count++;
        }
            // Then reserving child tickets
        else if (childTick > 0)
        {
            // Creating seat object to add to theater
            Seat s(place->getValue().getRow(), place->getValue().getSeat(), 'C');
            // Adding seat to theater and adding seat to order's seat vector
            place->setValue(s);
            if (!inserted)
            {
                userOrder.seatsList.push_back(s);
                inserted = true;
            }
            else
            {
                // Getting the size of the seat vector, and declaring an iterator in order to traverse the vector
                int size = static_cast<int>(userOrder.seatsList.size());
                vector<Seat>::iterator iter = userOrder.seatsList.begin();

                // While the size of the list is greater than 0, and the row of the seat to be inserted is greater than the
                //  current position in the vector being checked
                while (size > 0 && s.getRow() > (*iter).getRow())
                {
                    // Move position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // While the size of the list is greater than 0, and the row of the seat to be inserted is equal to the
                //  current position in the vector being checked, keep looking for the insertion point until the
                //  the letter of the seat being inserted is less than that of the position being currently checked
                while (size > 0 && s.getRow() == (*iter).getRow() && s.getSeat() > (*iter).getSeat())
                {
                    // Move the position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // Insert the seat at the position
                userOrder.seatsList.insert(iter, s);
            }
            // Decrementing remaining child tickets to be reserved
            childTick--;
            count++;
        }
            // Senior tickets are reserved last
        else if (seniorTick > 0)
        {
            // Creating seat object to add to theater
            Seat s(place->getValue().getRow(), place->getValue().getSeat(), 'S');
            // Adding seat to theater and adding seat to order's seat vector
            place->setValue(s);
            if (!inserted)
            {
                userOrder.seatsList.push_back(s);
                inserted = true;
            }
            else
            {
                // Getting the size of the seat vector, and declaring an iterator in order to traverse the vector
                int size = static_cast<int>(userOrder.seatsList.size());
                vector<Seat>::iterator iter = userOrder.seatsList.begin();

                // While the size of the list is greater than 0, and the row of the seat to be inserted is greater than the
                //  current position in the vector being checked
                while (size > 0 && s.getRow() > (*iter).getRow())
                {
                    // Move position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // While the size of the list is greater than 0, and the row of the seat to be inserted is equal to the
                //  current position in the vector being checked, keep looking for the insertion point until the
                //  the letter of the seat being inserted is less than that of the position being currently checked
                while (size > 0 && s.getRow() == (*iter).getRow() && s.getSeat() > (*iter).getSeat())
                {
                    // Move the position forward by one
                    iter++;
                    // Decrement the size by one
                    size--;
                }
                // Insert the seat at the position
                userOrder.seatsList.insert(iter, s);
            }
            // Decrementing remaining senior tickets to be reserved
            seniorTick--;
            count++;
        }
        place = place->getRight();
    }
}

/* Gets the best available seats that meet the user's criteria in the entire auditorium; best
*   seats available are the seats closest to the middle of the auditorium
* @param theater, the Auditorium object containing the theater and its reservations
* @param totalRes, the total number of seats to be reserved
* @return the starting seat of the best available reservation
*/
Node<Seat>* getBestAvailable(Auditorium* theater, int totalRes)
{
    // Calculating the coordinates of the center of the auditorium
    int centerRow = (theater->getNumRows() + 1) / 2;
    double centerSeat = ((theater->getNumCols() + 1) / 2.0);

    // Declaring and intializing the necessary variables
    double smallestDistance = -1;
    Node<Seat> *firstRes = theater->getHead();
    Node<Seat> *above = firstRes;
    Node<Seat>* closest = nullptr;

    // While there are more rows in linked list
    while (above != nullptr)
    {
        // While current node is not equal to null
        while (firstRes != nullptr)
        {
            // Check if reservation is available at current node
            bool checkAvail = checkAvailability(theater, firstRes->getValue().getRow(), firstRes->getValue().getSeat(), totalRes);
            if (checkAvail)
            {
                Node<Seat>* lastRes = firstRes;
                // Finding the last seat in the reservation
                for (int count = 1; count < totalRes; count++)
                {
                    lastRes = lastRes->getRight();
                }
                /* Calculating the position of the middle of the reservation in the grid */
                double resMiddle;
                // Calculating the beginning of the row to the first seat of the reservation
                if (totalRes == 1 || theater->getNumCols() % 2 == 0)
                {
                    resMiddle = static_cast<double>(firstRes->getValue().getSeat() - CHAR_TO_INDEX + 1);
                }
                else
                {
                    resMiddle = static_cast<double>(firstRes->getValue().getSeat() - CHAR_TO_INDEX);
                }

                // Adding the extra distance from the first seat in the reservation to the middle of the reservation
                resMiddle += static_cast<double>(totalRes) / 2.0;

                // Calculating distance between center of auditorium and center of reservation
                double distance = calculateDistance(firstRes->getValue().getRow(), resMiddle, centerRow, centerSeat);
                //cout << "checking distance for seats " << firstRes->getValue().getRow() << firstRes->getValue().getSeat() << "-";
                //cout << lastRes->getValue().getRow() << lastRes->getValue().getSeat() << " = " << distance << endl;
                // If no previous free reservation has been found or this distance is smaller than the smallest distance found so far,
                //      then set the smallest distance equal to the found distance and save the first seat in the reservation
                if (smallestDistance == -1 || distance < smallestDistance)
                {
                    closest = firstRes;
                    smallestDistance = distance;
                }
            }

            // Changing current node to the next node in row
            firstRes = firstRes->getRight();
        }
        // Resetting place to first seat of next row
        firstRes = above->getDown();
        // Resetting above to first seat of this row
        above = firstRes;

    }

    return closest;
}

/** Unreserves a seat in the given theater based on given user input that provides information about seats
*  to be deleted
* @param theater, Auditorium object that holds linked list of seating available in the theater
* @param row, the row number of the seat to be unreserved
* @param seat, the seat letter of the seat to be unreserved
*/
void removeReservedSeat(Auditorium*& theater, User*& theUser, vector<Seat>::iterator& iter, int orderNum, int row, char seat)
{
    Seat check = *iter;
    char type = check.getTicketType();
    // Decrementing either the adult, child, or senior ticket number by one depending on the
    //  ticket type of the seat to being removed
    if (type == 'A')
    {
        theUser->ordersList.at(orderNum - 1).adults -= 1;
    }
    else if (type == 'C')
    {
        theUser->ordersList.at(orderNum - 1).children -= 1;
    }
    else
    {
        theUser->ordersList.at(orderNum - 1).seniors -= 1;
    }
    // Removing the seat from the order's vector of seats
    theUser->ordersList.at(orderNum - 1).seatsList.erase(iter);

    // If the size of the seat vector is 0, then remove the order from the order vector
    if (theUser->ordersList.at(orderNum - 1).seatsList.empty())
    {
        // Setting the iterator to the beginning of the vector
        vector<Order>::iterator iter2 = theUser->ordersList.begin();
        // Using the erase function to remove the order from the vector
        theUser->ordersList.erase(iter2 + (orderNum - 1));
    }

    // Finding the seat to be removed in the theater
    Node<Seat> *place = theater->getHead();
    Node<Seat> *above = place;
    bool found = false;

    // While there are more rows in linked list
    while (above != nullptr && !found)
    {
        // While the current node is not equal to null
        while (place != nullptr && !found)
        {
            // If current seat is the seat to be removed
            if (place->getValue().getSeat() == seat && place->getValue().getRow() == row)
            {
                found = true;
            }
            else
            {
                // Updating current node to next node in row
                place = place->getRight();
            }
        }

        if (!found)
        {
            // Resetting place to first seat of next row
            place = above->getDown();
            // Resetting above to first seat of this row
            above = place;
        }
    }

    // Once the seat is found, mark the seat as available
    Seat s(row, seat, '.');
    place->setValue(s);
}

/* Calculates the distance between two points in the theater
* @param x1, the row of the reservation being checked
* @param y1, the middle point of the reservation being checked
* @param x2, the row of the center of the auditorium
* @param y2, the middle of the center row
* @return the distance
*/
double calculateDistance(double x1, double y1, double x2, double y2)
{
    // Uses the distance formula to calculate the distance between two points
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

/** Prints the given order
 * @param orderPrint, the order to print
 * @return the cost of the order
 */
double printOrder(Order orderPrint)
{
    // Displaying the auditorium number
    cout << "Auditorium " << orderPrint.auditorium << ", ";
    // Getting the list of seats reserved in the order
    vector<Seat> resSeats = orderPrint.seatsList;
    // For every seat in the order, print the row then seat letter
    vector<Seat>::iterator iter;
    for (iter = resSeats.begin(); iter != resSeats.end(); iter++)
    {
        Seat seatPrint = *iter;
        cout << seatPrint.getRow() << seatPrint.getSeat();
        // If this is not the last seat, then print a comma
        if ((iter + 1) != resSeats.end())
        {
            cout << ",";
        }
    }
    cout << endl;
    // Displaying the ticket types
    cout << orderPrint.adults << " adult, ";
    cout << orderPrint.children << " child, ";
    cout << orderPrint.seniors << " senior" << endl;

    // Calculating the total cost of the order
    double totalCost = 0;
    // For each ticket type, add the appropriate amount of money spent
    totalCost += (orderPrint.adults * ADULT_PRICE);
    totalCost += (orderPrint.children * CHILD_PRICE);
    totalCost += (orderPrint.seniors * SENIOR_PRICE);

    return totalCost;
}

/* Displays a report for the admin of all of the information about number of seats in the theater,
* total seats reserved in the theater, how many of each type of ticket were sold, and total sales
* @param theater, Auditorium object that contains theater reservations
* @param open, total seats that are not reserved in ALL theaters
* @param reserved, total seats reserved in ALL theaters
* @param adult, total adult seats reserved in ALL theaters
* @param child, total child seats reserved in ALL theaters
* @param senior, total senior seats reserved in ALL theaters
* @param total, total cost of all the reservations in the theater in ALL theaters
*/
void displayReport(Auditorium* theater, int& open, int& reserved, int& adult, int& child,
                   int& senior, double& total)
{
    int totalSeats = 0, adultSold = 0, childSold = 0, seniorSold = 0;
    double totalSales = 0;

    // Getting theater information
    Node<Seat> *place = theater->getHead();
    Node<Seat> *above = place;

    /* Iterating through theater and keeping track of the number of seats, how many of
     * each of the types of tickets were sold, and calculating the total sales made
     */
    while (above != nullptr)
    {
        while (place != nullptr)
        {
            // Checking for adult tickets reserved
            if (place->getValue().getTicketType() == 'A')
            {
                adultSold++;
                totalSales += ADULT_PRICE;
                totalSeats++;
            }
                // Checking for child tickets reserved
            else if (place->getValue().getTicketType() == 'C')
            {
                childSold++;
                totalSales += CHILD_PRICE;
                totalSeats++;
            }
                // Checking for senior tickets reserved
            else if (place->getValue().getTicketType() == 'S')
            {
                seniorSold++;
                totalSales += SENIOR_PRICE;
                totalSeats++;
            }
                // Checking if the seat is not reserved
            else if (place->getValue().getTicketType() == '.')
            {
                totalSeats++;
            }

            // Setting place to next seat
            place = place->getRight();
        }

        // Resetting place to first seat of next row
        place = above->getDown();
        // Resetting above to first seat of this row
        above = place;
    }

    int addOpen = (totalSeats - (adultSold + childSold + seniorSold));
    int addRes = (adultSold + childSold + seniorSold);
    // Printing report
    cout << addOpen << "\t" << addRes;
    cout << "\t" << adultSold << "\t" << childSold << "\t" << seniorSold << "\t";
    printf("$%-4.2f", totalSales);

    // Adding all of the numbers to the TOTAL numbers across all auditoriums
    open += addOpen;
    reserved += addRes;
    adult += adultSold;
    child += childSold;
    senior += seniorSold;
    total += totalSales;
}

