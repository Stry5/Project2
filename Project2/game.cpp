#include "game.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>

namespace game {
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

    // checks win conditions
    bool checkWin(Game& gameVars) {

        // if the player still has lives
        if (gameVars.lives > 0) {
            // loop thru the word and check if the two strings are equal
            for (int i = 0; i < gameVars.word.length(); i++) {

                if ((toupper(gameVars.word[i]) == toupper(gameVars.printedWord[i]))) {
                    gameVars.playerHasWon = true; // returns true if they are
                }
                else {
                    gameVars.playerHasWon = false; // returns false if they aren't
                    break;
                }
            }
        }
        // if the player doesn't have lives, set these two variables to false
        else {
            gameVars.playerIsAlive = false;
            gameVars.playerHasWon = false;
        }

        // return depending on if the player has won or not
        if (gameVars.playerHasWon == true) {
            std::cout << "You win!! the word was: " << gameVars.word << std::endl;
            return gameVars.playerHasWon;
        }
        if ((gameVars.playerHasWon == false) && (gameVars.playerIsAlive == false)) {
            std::cout << "You lose :( the word was: " << gameVars.word << std::endl;
            return gameVars.playerHasWon;
        }

        // defaults to returning if the player has won or not
        return gameVars.playerHasWon;
    }

    // prints out word
    void printWord(Game& gameVars) {

        // prints out the letters the user has guessed so far
        std::cout << "Letters Guesssed:" << std::endl;
        for (int i = 0; i < gameVars.size; i++) {
            std::cout << gameVars.lettersGuessed[i] << ",";
        }
        std::cout << std::endl;

        // updates the word
        initializeWord(gameVars);

        // checks win conditions
        checkWin(gameVars);
        std::cout << gameVars.printedWord << std::endl;
    }

    // intializes the word 
    void initializeWord(Game& gameVars) {
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
                if (isalpha(gameVars.word[i])) {
                    gameVars.printedWord[i] = '_';
                }
                else {
                    gameVars.printedWord[i] = ' ';;
                }
            }

        }
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

        initializeWord(gameVars);

        // while the player still has lives
        while ((gameVars.playerHasWon == false) && (gameVars.playerIsAlive == true)) {
            checkWin(gameVars);
            std::cout << "Lives remaning: " << gameVars.lives << std::endl;
            printWord(gameVars);

            // only does this while the player hasn't won yet
            if (gameVars.playerHasWon == false) {
                // debug line, saved for later
                // std::cout << word << std::endl;
                std::cout << "What's your guess?\n";
                std::cin >> guess;
                while (verifyInput(guess, gameVars) == false) {
                    std::cout << "please input a valid guess.\n";
                    std::cin >> guess;
                }
            }
            updateGame(guess, gameVars);

        }
        fileOp.close();  // closes file

        // dealocate memory
        delete[] gameVars.lettersGuessed;
        gameVars.lettersGuessed = nullptr;



    }
}