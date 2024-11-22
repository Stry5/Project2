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
        short size;            // size of the array lettersGuessed
        char* lettersGuessed; // pointer to a dynamically allocated  array of the letters guessed
        short lives;         // number of lives user has
        short numGuesses;   // number of guesses user has made
    };
    void displayMenu();
    void gameOperation(game::Level& difficulty);
    void difficultySelector(game::Level& difficulty, std::ifstream& fileOp);
    void printWord(const std::string& word, char* lettersGuessed[], const short& size);
    bool verifyInput(const char& guess, Game& gameVars);
    void updateArray(Game& gameVars);
    void updateGame(const char& guess, Game& gameVars);

}

namespace game {

    // prints out word
    void printWord(const std::string& word, char* lettersGuessed[], const short& size) {
        for (int i = 0; i < word.length(); i++) { // loops through the entire word

            if (size > 0) { // prints letter if there are guesses present

                bool letterFound = false; // flag that shows if the letter was found in the vector or not
                
                // checks the guessed letter array first
                for (int x = 0; x < size;x++) { 
                    if (isalpha(word[i]) && (toupper(word[i]) == toupper(*lettersGuessed[x]))) { // compares upper case version of letters
                        letterFound = true;
                        std::cout << *lettersGuessed[x];
                        break;
                    }
                }
                // if the letter is not found
                if (letterFound == false) {
                    // and isnt a space, prints a blank
                    if (!isspace(word[i])) {
                        std::cout << "_";
                    }

                    else { // prints space otherwise
                        std::cout << " ";
                    }
                }
            }

            else { // prints blanks if there are no guesses
                if (isalpha(word[i])){
                    std::cout << "-";
                }
                else {
                    std::cout << "|";
                }
            }

        }
        std::cout << std::endl;
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

    // updates the game statuses
    void updateGame(const char& guess, Game& gameVars) {
         // adds letter to the letter as upper case to storage
        updateArray(gameVars);

        for (int i = 0; i < gameVars.word.length(); i++) {

            //debug line, saved for later debugging if necessary
            std::cout << "guess: " << toupper(guess) << " word: " << toupper(gameVars.word[i]) << " array: " << gameVars.lettersGuessed[0] << std::endl;
            
            if (toupper(guess) == toupper(gameVars.word[i])) {
                return; // checks if the letter is present in the word or not. If it is present, the function returns
            }
        }

        gameVars.lives--; // if it isn't, the function subtracts from the lives the player has
    }

    // increases the size of said array by 1
    void updateArray(Game& gameVars) {
        // new array
        char* newArray = new char[gameVars.size + 1];
        gameVars.size++;

        // traverse array and copy the info to the new array
        for (int i = 0; i < gameVars.size; i++) {
            newArray[i] = gameVars.lettersGuessed[i];
        }
        delete gameVars.lettersGuessed; // delete old array
        gameVars.lettersGuessed = newArray; // return the new array
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
        gameVars.size = 0; // sets size of the array to 0
        gameVars.lettersGuessed = new char[gameVars.size]; // creates pointer array to an array

        short wordIndex = rand() % 6; // random number between 0 and 5
        short currentIndex = 0; // current index of counter

        gameVars.lives = 5; // number of lives the user has
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

        // while the player still has lives
        while (gameVars.lives > 0) {

            std::cout << "Lives remaning: " << gameVars.lives << std::endl;

            // debug line, saved for later
            // std::cout << word << std::endl;

            printWord(gameVars.word, &gameVars.lettersGuessed, gameVars.size);
            std::cout << "What's your guess?\n";
            std::cin >> guess;
            updateGame(guess, gameVars);


            while (verifyInput(guess, gameVars) == false) {
                std::cout << "please input a valid guess.\n";
                std::cin >> guess;

            }




            fileOp.close();  // closes file
        }
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
     difficulty = static_cast<game::Level>(difficultyOption);

     std::cout << "You have selected " << difficulty << ".\n";

     game::gameOperation(difficulty);




    std::cout << "You died :(\n";
}


/*
Hangman:
Features:
    - Different difficulty levels (counted using enum) (pulls list of characters from different files)
    - Lives counter
    - Visual display of characters
    - Edge cases that cover the right/wrong/same inputs
*/