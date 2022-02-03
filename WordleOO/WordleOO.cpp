#include <iostream>
#include <vector>
#include <windows.h>                   // you will need this for the Windows console routines
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
const int WORD_LENGTH = 5; //the length of the word that the player will be guessing
const int TRIES = 6; //the number of tried
const string WORD_LIST_FILE = "word_list.txt"; // the name of the file with all the words int
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //get a handle on the console
/**
* Status enum for the 3 status is char could be, 
* NOT_FOUND, means the char is not in the word
* FOUND_WRONG_PLACE means the char is in the word, but not in this location
* FOUND means the char is in the word and in the correct place
*/
enum class Status 
{
	NOT_FOUND,
	FOUND_WRONG_PLACE,
	FOUND
};
/**
* GuessCharacter is the class that represent a single character in each guess word.
* Has two attributes, letter which is the letter in the word, and status which is the
* status of the characted in terms of the game.
*/
class GuessCharacter
{
private:
	char letter;
	Status status;
public:
	GuessCharacter() { letter = '1'; status = Status::NOT_FOUND; }
	/**
	* displayChar function display the characted by its status
	*/
	void displayChar() 
	{
		if (letter != '1')
		{
			if (status == Status::FOUND_WRONG_PLACE)
			{
				SetConsoleTextAttribute(h, 6); //display yellow
			}
			else if (status == Status::FOUND)
			{
				SetConsoleTextAttribute(h, 2); //display green
			}
			cout << letter;
			SetConsoleTextAttribute(h, 7); //dispay white
		}
	}		
	//setters for letter and status, getters not needed
	void setLetter(char letter)
	{
		this->letter = letter;
	}
	void setStats(Status status)
	{
		this->status = status;
	}
};
/**
* GuessWord is each word that the player can guess, it is made up of an array of 5 GuessCharacter 's
*/
class GuessWord
{
private:
	GuessCharacter guessCharacters[WORD_LENGTH]; //array of guess charactes

public:
	/**
	* setGuessCharacters sets the guess characted on the guess word, compares to the word to find to get status
	*/
	void setGuessCharacters(string guess, string wordToFind)
	{
		for (int i = 0; i < WORD_LENGTH; i++)
		{
			guessCharacters[i].setLetter(guess[i]);
			if (guess[i] == wordToFind[i])
			{
				guessCharacters[i].setStats(Status::FOUND);
			}//check if the char is in the word
			else if (wordToFind.find(guess[i])<wordToFind.length())  //.find() return the position of the letter in the word, so if it returns less than the word length, we know its there
			{
				guessCharacters[i].setStats(Status::FOUND_WRONG_PLACE);
			}
			else
			{
				guessCharacters[i].setStats(Status::NOT_FOUND);
			}
			
		}
	}
	/**
	* display prints the word with colours to the console, for the player to see how they are doing
	*/
	void display()
	{
		for (int i = 0; i < WORD_LENGTH; i++)
		{
			guessCharacters[i].displayChar();
		}
		cout << endl;
	}
};
/**
* Game class contains all the function to play the game and the data needed.
* It holds and array of guessWord that are the players attempts;
* It also holds the complete wordLIst
* and it also holds the word that needs to be found
* it also holds a turn counter
*/
class Game
{
private:
	GuessWord guessWords[TRIES];//the array of all the guesses so far
	vector<string> wordList;//all possible words
	string wordToFind; //the word the player is looking for.
	int turn = 0;//this i the turn the player is on
	/**
	* load in all possible words, called from the play function, private as not needed outside the class
	*/
	void populateWordList() 
	{
		ifstream inputfile(WORD_LIST_FILE);
		while (!inputfile.eof())
		{
			string line;
			getline(inputfile, line);
			wordList.push_back(line);
		}
	}
	/**
	* binarySearchWordList performs a binary searh on the word list
	*/
	bool binarySearchWordList(string value)
	{
		int  first = 0,						// First array element
			last = wordList.size()-1  ,		// Last array element
			middle,							// Midpoint of search
			position = -1;					// Position of search value
		bool found = false;					// Flag to indicate if the value was found 

		while (!found && first <= last)
		{
			middle = (first + last) / 2;    // Calculate midpoint
			if (wordList[middle] == value)  // If value is found there
			{
				found = true;
				position = middle;
			}
			else if (wordList[middle] > value) // If value is in lower half
				last = middle - 1;
			else
				first = middle + 1;          // If value is in upper half
		}
		return found;
	}

	/**
	* isValidWord called from playerGuess checks if a word is of the correct length, and exists in the word list
	**/
	bool isValidWord(string guess)
	{
		if (guess.length() != 5)
		{
			cout << "word is not correct length, it must be exactly 5 chars" << endl;
			return false;
		}
		if (binarySearchWordList(guess))
		{
			return true;
		}
		else
		{
			cout << "Not in word list, try again" << endl;
			return false;
		}
	}
	/**
	* selectWord generates a random number, and selects a word to play with for the game
	*/
	void selectWord()
	{
		srand(time(0));
		wordToFind = wordList[(rand() % wordList.size())];
	}
	/**
	* welcomeMessage display the player instructions
	*/
	void welcomeMessage()
	{
		cout << "Welcome to World OO, you have " << TRIES << " tried to guess the word " << endl;
		cout << "The letter will be ";
		SetConsoleTextAttribute(h, 6);
		cout << "Yellow";
		SetConsoleTextAttribute(h, 7);
		cout << " if the character is in the word, but in a different place" << endl;
		cout << "The letter will be ";

		SetConsoleTextAttribute(h, 2);
		cout << "Green";
		SetConsoleTextAttribute(h, 7);
		cout << " if the character is in the word and in the correct place" << endl;
	}
	
	/**
	* checkAndDisplayGuess displays the guess with colour coding, and then tell the player if they have won, or if it is game over
	*/
	bool checkAndDisplayGuess(string guess)
	{
		guessWords[turn].setGuessCharacters(guess, wordToFind); // the the guess characters, the word to find is needed to it can be compared to
		system("cls"); //clear the screen
		welcomeMessage(); //redisplay welcome message
		for (int i = 0; i <= turn; i++) //then display all guesses so far
		{
			guessWords[i].display();
		}		
		if (guess == wordToFind)
		{
			return true;
		}
		turn++;
		return false;
	}

	/**
	* gets the player to enter their guess, and then checks if it is valid
	*/
	bool playerGuess()
	{
		int wrongCount = 0;
		string guess;
		cout << "Enter your guess" << endl;
		do {
			getline(cin, guess); //gets guess from player
		} while (!isValidWord(guess)); //checks if a valid word

		if (checkAndDisplayGuess(guess)) //checks the guess cor correctness, then displays with colour
		{
			return true;
		}
		else
		{
			return false;
		}
	}
public:
	

	
	/**
	* play function sets up and starts the game. The player is shown the game rules.
	* has a bool flag called cheat which if is true, displays the word to be found at the start of the game
	*/
	void play(bool cheat)
	{
		populateWordList(); // the word list is populated
		selectWord();
		if (cheat)
		{
			cout << "cheat mode on!!" << endl;
			cout << "word to find is........" << endl;
			Sleep(1000);
			cout << wordToFind << endl << endl;
		}

		welcomeMessage();

		bool playerWon = false; //flag to check if the player has won;
		while (!playerWon)
		{
			if (turn == TRIES)
			{
				SetConsoleTextAttribute(h, 4); //set text red
				cout << "Game Over" << endl;
				SetConsoleTextAttribute(h, 7); //set text white
				cout << "The word to find was : " << wordToFind;
				break;
			}
			if (playerGuess())
			{
				playerWon = true;

				SetConsoleTextAttribute(h, 2); //set set green
				cout << "Congratualtions! you win" << endl;
				SetConsoleTextAttribute(h, 7); //set text white
			}
		}
	}
};

int main()
{                       
	Game game;
	game.play(true);	//starts the game
}