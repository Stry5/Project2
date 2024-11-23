// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "game.h"


int main()
{
    srand(time(nullptr)); // seeds all RNGs

    short difficultyOption; // difficulty input from the user
    game::Level difficulty; // difficulty enum
    std::cout << "Welcome to Hangman! Type 0 to quit.\n"
        << "Please select your difficulty level.\n";
    
     game::displayMenu();
     std::cin >> difficultyOption;

     // why does this give me an infinite loop?
     while (std::cin.fail()) {
         std::cin.clear();
         std::cout << "error, not a valid input. please try again.\n";
         std::cin >> difficultyOption;
     }

     // have to cast input to the enumeration type
     difficulty = static_cast<game::Level>(difficultyOption);

     std::cout << "You have selected " << difficulty << ".\n";


     //starts game here
     game::gameOperation(difficulty);
}


/*
Hangman:
Features:
    - Different difficulty levels (counted using enum) (pulls list of characters from different files)
    - Lives counter
    - Visual display of characters
    - Edge cases that cover the right/wrong/same inputs
*/