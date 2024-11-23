#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>

namespace game {
    enum Level { // enumeration containing diffiuclty levels
        QUIT,
        EASY,
        MED,
        HARD

    };
    // overloading operator when you use the << operator on a level enumeration
    std::ostream& operator<<(std::ostream& stream, Level& level);

    // structure that has all the game variables in it
    struct Game { // structure with game variables stored in it

        std::string word;       // word the user has to guess
        std::string printedWord; // word that is printed
        short size;            // size of the array lettersGuessed
        char* lettersGuessed = nullptr; // pointer to a dynamically allocated array of the letters guessed
        short lives;         // number of lives user has
        short numGuesses;   // number of guesses user has made
        bool playerIsAlive = true;
        bool playerHasWon = false;
    };
    void displayMenu();
    void gameOperation(game::Level& difficulty);
    void difficultySelector(game::Level& difficulty, std::ifstream& fileOp);

    /*
    for these functions, it's easier to have them pass the entire structure by reference
     1. to allow for easy access to any variable
     2. to allow for adding and removing any variable
     3. to make changing pointers work. It doesn't work for some reason if u pass the pointer itself
     vs. accessing the acutal pointer.

     con is that its risky and you can't make specific inputs constant.
                                    vvv
    */
    void printWord(Game& gameVars);
    void initializeWord(Game& gameVars);
    bool verifyInput(const char& guess, Game& gameVars);
    void updateArray(Game& gameVars);
    void updateGame(const char& guess, Game& gameVars);
    bool checkWin(Game& gameVars);
}
