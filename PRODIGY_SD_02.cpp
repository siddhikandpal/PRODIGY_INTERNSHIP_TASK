#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // Initialize random number generator
    srand(static_cast<unsigned>(time(0)));

    int lowerBound = 1;
    int upperBound = 100;
    int randomNumber = rand() % (upperBound - lowerBound + 1) + lowerBound;

    int attempts = 0;
    int userGuess;

    cout << "Welcome to the Guessing Game!" << endl;
    cout << "I have selected a number between " << lowerBound << " and " << upperBound << ". Try to guess it." << endl;

    do {
        cout << "Enter your guess: ";
        cin >> userGuess;
        attempts++;

        if (userGuess < randomNumber) {
            cout << "Too low! Try again." << endl;
        } else if (userGuess > randomNumber) {
            cout << "Too high! Try again." << endl;
        } else {
            cout << "Congratulations! You guessed the number in " << attempts << " attempts." << endl;
        }
    } while (userGuess != randomNumber);

    return 0;
}
