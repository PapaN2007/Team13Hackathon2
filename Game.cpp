// Game.cpp - click logic, right/wrong checking and explanation display.
// Written with help from Claude (Anthropic AI), edited by our team.
#include "Game.h"
#include <vector>
#include <string>
using namespace std;

// DrawWrapped: draws long text on several lines.
// Written with help from Claude (Anthropic AI), edited by me (Rafiu). on Friday 20th at 12:00 a.m.
// raylib DrawText (raylib 5.5 cheatsheet).
void DrawWrapped(string text, int x, int y, int maxChars, int fontSize, Color color) {
    while ((int)text.length() > maxChars) {
        size_t cut = text.rfind(' ', maxChars);
        if (cut == string::npos) cut = maxChars;
        DrawText(text.substr(0, cut).c_str(), x, y, fontSize, color);
        text = text.substr(cut + 1);
        y = y + fontSize + 4;
    }
    DrawText(text.c_str(), x, y, fontSize, color);
}

// The game variables (declared with "extern" in GameLogic.h)
GameScreen currentScreen = START_SCREEN;
int current = 0;
int money = 0;
int picked = -1;

// Runs every frame: handles the keys (the mouse clicks are in main.cpp).
void UpdateGame(vector<Question> questions) {

    // START SCREEN: ENTER starts a new game
    if (currentScreen == START_SCREEN) {
        if (IsKeyPressed(KEY_ENTER)) {
            current = 0;
            money = 0;
            picked = -1;
            currentScreen = GAMEPLAY_SCREEN;
        }
    }
    // GAMEPLAY SCREEN
    else if (currentScreen == GAMEPLAY_SCREEN) {
        // press 1 to go back to the start currentScreen
        if (IsKeyPressed(KEY_ONE)) {
            currentScreen = START_SCREEN;
        }
        // answer already picked: SPACE moves on
        else if (picked != -1 && IsKeyPressed(KEY_SPACE)) {
            if (picked == questions[current].correct) {
                current++;
                picked = -1;
                if (current >= (int)questions.size()) {
                    currentScreen = WIN_SCREEN;
                }
            } else {
                currentScreen = GAME_OVER_SCREEN;
            }
        }
    }
    // GAME OVER or WIN SCREEN: ENTER goes back to the start currentScreen
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            currentScreen = START_SCREEN;
        }
    }
}

// Runs every frame after an answer is picked: right/wrong + explanation.
void DrawFeedback(Question q, vector<string> labels, int picked, bool isLastQuestion) {
    int y = 250;

    if (picked == q.correct) {
        DrawText("Correct!", 100, 200, 40, GREEN);
    } else {
        DrawText("Wrong!", 100, 200, 40, RED);
        DrawWrapped("Correct answer: " + labels[q.correct], 100, y, 80, 24, WHITE);
        y = 310;   // push the explanation down so it doesn't overlap
    }

    DrawWrapped(q.explanation, 100, y, 80, 26, YELLOW);
    DrawWrapped("Source: " + q.source, 100, y + 70, 120, 16, LIGHTGRAY);

    // what to do next
    if (picked != q.correct) {
        DrawText("GAME OVER - Press SPACE to continue", 100, 750, 28, RED);
    } else if (isLastQuestion) {
        DrawText("You answered them all! Press SPACE to finish", 100, 750, 28, WHITE);
    } else {
        DrawText("Correct! Press SPACE to move on to the next question", 100, 750, 28, WHITE);
    }
}