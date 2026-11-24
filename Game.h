// Game.h - click logic, right/wrong checking and explanation display.
// Functions are written in Game.cpp.
#include "raylib.h"
#include "Question.h"
#include <vector>
#include <string>
using namespace std;

// All of our screens.
enum GameScreen {
    START_SCREEN,
    GAMEPLAY_SCREEN,
    GAME_OVER_SCREEN,
    WIN_SCREEN
};

// Draws long text on several lines.
void DrawWrapped(string text, int x, int y, int maxChars, int fontSize, Color color);

// The game variables. They live in Game.cpp and main.cpp can use them too
// ("extern" just means: this variable is created in another file).
// https://www.geeksforgeeks.org/cpp/how-do-i-use-extern-to-share-variables-between-c-source-files/
extern GameScreen currentScreen;   
extern int current;                
extern int money;                  
extern int picked;                

void UpdateGame(vector<Question> questions);
void DrawFeedback(Question q, vector<string> labels,int picked, bool isLastQuestion);