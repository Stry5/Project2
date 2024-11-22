// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <memory>
#include <cctype>


// prototypes
namespace game {
    enum Level { // enumeration containing diffiuclty levels
        QUIT,
        EASY,
        MED,
        HARD

    };
    // overloading operator when you use the << operator on a level enumeration
    std::ostream& operator<<(std::ostream& stream, Level& level) {
        switch (level) {
        case 0:
            stream << "[QUIT]";
            break;
        case 1:
            stream << "[EASY]";
            break;
        case 2:
            stream << "[MEDIUM]";
            break;
        case 3: 
            stream << "[HARD]";
            break;
        default:
            stream << "[DIFFICULTY NOT FOUND]";
            break;
        }

        return stream;
    };

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
    void printWord(const Game& gameVars);
    bool verifyInput(const char& guess, Game& gameVars);
    void updateArray(Game& gameVars);
    void updateGame(const char& guess, Game& gameVars);
    void checkWin(Game& gameVars);
}

namespace game {

    // checks win conditions
    void checkWin(Game& gameVars) {
        if (gameVars.lives > 0) {
            for (int i = 0; i < gameVars.word.length(); i++) {

                if ((toupper(gameVars.word[i]) == toupper(gameVars.printedWord[i])) && gameVars.word[i] != ' ') {
                    gameVars.playerHasWon = true;
                }
                else {
                    gameVars.playerHasWon = false;
                    break;
                }
            }
        }
        else {
            gameVars.playerIsAlive = false;
            gameVars.playerHasWon = false;
        }


        if (gameVars.playerHasWon == true) {
            std::cout << "You win!! the word was: " << gameVars.word << std::endl;
            return;
        }
        if ((gameVars.playerHasWon == false) && (gameVars.playerIsAlive == false)) {
            std::cout << "You lose :( the word was: " << gameVars.word << std::endl;
            return;
        }
    }

    // prints out word
    void printWord(Game& gameVars) {

        // prints out the letters the user has guessed so far
        std::cout << "Letters Guesssed:" << std::endl;
        for (int i = 0; i < gameVars.size; i++) {
            std::cout << gameVars.lettersGuessed[i] << ",";
        }
        std::cout << std::endl;

        for (int i = 0; i < gameVars.word.length(); i++) { // loops through the entire word

            if (gameVars.size > 0) { // if there are guesss present

                bool letterFound = false; // flag that shows if the letter was found in the array or not
                
                // checks the guessed letter array
                for (int x = 0; x < gameVars.size;x++) { 

                    // if the character in the word is alphabetical and it is in the letters guessed array
                    if (isalpha(gameVars.word[i]) && (toupper(gameVars.word[i]) == toupper(gameVars.lettersGuessed[x]))) { // compares upper case version of letters
                        letterFound = true;
                        gameVars.printedWord[i] = gameVars.lettersGuessed[x];
                        break;
                    }
                }
                // if the letter is not found
                if (letterFound == false) {
                    // and isnt a space, prints a blank
                    if (!isspace(gameVars.word[i])) {
                        gameVars.printedWord[i] = '_';
                    }

                    else { // prints space otherwise
                        gameVars.printedWord[i] = ' ';
                    }
                }
            }

            else { // prints blanks if there are no guesses
                if (isalpha(gameVars.word[i])){
                    gameVars.printedWord[i] = '_';
                }
                else {
                    gameVars.printedWord[i] = ' ';;
                }
            }

        }
        std::cout << gameVars.printedWord << std::endl;
    }

    // verifys the input the user gave
    bool verifyInput(const char& guess, Game& gameVars) { // checks if input is valid

        //checks if its alphanumeric or not
        if (!isalpha(guess)) {
            std::cout << "guess is not alphanumeric\n";
            return false;
        }

        //checks if the letter has been guessed or not
        for (int i = 0; i < gameVars.size; i++) {
            if (toupper(guess) == std::toupper(gameVars.lettersGuessed[i])) {
                std::cout << "letter already guessed\n";
                return false;
            }
        }
        return true;
    }

    //displays menu
    void displayMenu() {
        std::cout << "------\n"
            << std::setw(5) << std::left << "Difficulty:\n"
            << "------\n"
            << std::setw(10) << std::left << "Easy:" << std::setw(3) << std::right << "1\n"
            << std::setw(10) << std::left << "Medium:" << std::setw(3) << std::right << "2\n"
            << std::setw(10) << std::left << "Hard:" << std::setw(3) << std::right << "3\n";

    }

    // updates the game variables (lives, letters guessed)
    void updateGame(const char& guess, Game& gameVars) {
        updateArray(gameVars);


        gameVars.lettersGuessed[gameVars.size - 1] = toupper(guess); // adds guess to the end of the array

        //  updates the lives
        for (int i = 0; i < gameVars.word.length(); i++) {

            //debug line, saved for later debugging if necessary
            //std::cout << "guess: " << toupper(guess) << " word: " << toupper(gameVars.word[i]) << " array: " << gameVars.lettersGuessed[0] << std::endl;
            
            if (toupper(guess) == toupper(gameVars.word[i])) {
                return; // checks if the letter is present in the word or not. If it is present, the function returns
            }
        }

        gameVars.lives--; // if it isn't, the function subtracts from the lives the player has
        checkWin(gameVars);

    }

    // increases the size of lettersGuessed array by 1 per call
    void updateArray(Game& gameVars) {

        // new array
        char* newArray = new char[gameVars.size + 1];

        gameVars.size++;

        // traverse array and copy the info to the new array
        for (int i = 0; i < gameVars.size; i++) {

            newArray[i] = gameVars.lettersGuessed[i];

        }

        delete[] gameVars.lettersGuessed; // delete old array

        gameVars.lettersGuessed = newArray; // set the new array to the pointer


    }

    //selects the difficulty passing by reference
    void difficultySelector(game::Level& difficulty, std::ifstream& fileOp) {

        switch (difficulty) {
        case 1:
            fileOp.open("EASY_WORDS.txt");
            if (fileOp.fail()) {
                std::cout << "File open failed.\n";
                throw std::invalid_argument("!! File open falied !!");
            }
            break;
        case 2:
            fileOp.open("MED_WORDS.txt");

            if (fileOp.fail()) {
                std::cout << "File open failed.\n";
                throw std::invalid_argument("!! File open falied !!");
            }
            break;

        case 3:
            fileOp.open("HARD_WORDS.txt");
            if (fileOp.fail()) {
                std::cout << "File open failed.\n";
                throw std::invalid_argument("!! File open falied !!");
            }
            break;
        default:
            throw std::invalid_argument("!! Difficulty selection falied !!");
            break;
        }
    }

    // game during runtime
    void gameOperation(game::Level& difficulty) {

        Game gameVars; // creates gameVars structure
        gameVars.size = 0;
        gameVars.lettersGuessed = new char[gameVars.size]; // creates pointer array to an array

        short wordIndex = rand() % 6; // random number between 0 and 5
        short currentIndex = 0; // current index of counter

        gameVars.lives = 5;
        char guess; // user inputted guess

        std::ifstream fileOp; // file operator variable

        difficultySelector(difficulty, fileOp);

        while (!fileOp.eof()) { // gets the word from the file
            currentIndex++;
            std::getline(fileOp, gameVars.word);
            if (currentIndex == wordIndex) {
                break;
            }
        }
        gameVars.printedWord = gameVars.word;

        // while the player still has lives
        while ((gameVars.playerHasWon == false) && (gameVars.playerIsAlive == true)) {

            std::cout << "Lives remaning: " << gameVars.lives << std::endl;
            // debug line, saved for later
            // std::cout << word << std::endl;
            printWord(gameVars);
            checkWin(gameVars);
            std::cout << "What's your guess?\n";
            std::cin >> guess;
            while (verifyInput(guess, gameVars) == false) {
                std::cout << "please input a valid guess.\n";
                std::cin >> guess;
            }
            updateGame(guess, gameVars);

        }
        fileOp.close();  // closes file

        // dealocate memory
        delete[] gameVars.lettersGuessed;
        delete gameVars.lettersGuessed;
        gameVars.lettersGuessed = nullptr;



    }
}


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