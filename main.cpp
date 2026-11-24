#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "raylib.h"
#include "Game.h"
#define NUM_FRAMES  3
using namespace std;
/* Used Gemini To help figure out why screen wouldn't stay after clicking enter
and switching back to the start screen
Question Vectors with all of our questions,
each question answers and the correct answers. - Gained from Both Raifu and Chrislin.
*/
vector<Question> questions = {
    Question(
        "1895 - Cinematographe",
        "What made the Lumiere brothers' Cinematographe different from Edison's Kinetoscope?",
        0,
        "National Science and Media Museum (2009), The Lumiere Brothers: Pioneers of Cinema and Colour Photography",
        "The Cinematographe could project films for an audience, while the Kinetoscope was a peephole viewer."
    ),
    Question(
        "1891 - Kinetograph",
        "What was the main purpose of the Kinetograph developed in Edison's laboratory?",
        1,
        "Alice Askins (2018), Mechanical Eye: Early History Motion Pictures",
        "The Kinetograph was the camera that recorded moving images on film."
    ),
    Question(
        "1891 - Kinetoscope",
        "How did people originally watch films using the Kinetoscope?",
        2,
        "Richard Brown (2012), A New Look at Old History - The Kinetoscope: Fraud and Market Development in Britain in 1895",
        "The Kinetoscope was a cabinet viewed through a peephole, one person at a time."
    ),
    Question(
        "1895 - Kinetophone",
        "What was the Kinetophone designed to combine?",
        1,
        "Jane M. Gaines (2023), Antonia Dickson: The Kineto-Phonograph and the Telephony of the Future",
        "The Kinetophone tried to join moving pictures with recorded sound."
    ),
    Question(
        "1895 - Lumiere Brothers",
        "What happened during the Lumiere brothers' public screening in Paris on December 28, 1895?",
        1,
        "HISTORY.com Editors (2009), First Commercial Movie Screened",
        "They showed short motion pictures to a paying audience."
    ),
    Question(
        "1895 - Early Film Shorts",
        "What do early Lumiere films showing workers leaving factories tell us about cinema?",
        1,
        "Pamela Hutchinson (2016), A Window on Infinity: Rediscovering the Short Films of the Lumiere Brothers",
        "Early films could document real people, work, and everyday life."
    ),
    Question(
        "1891-1895 - Development of Cinema",
        "What was one important difference between Edison's early film system and the Lumiere brothers' Cinematographe?",
        0,
        "Peter Domankiewicz (2021), In the Beginning: Cinema's Murky Origin Story",
        "Edison used individual peephole viewers, while the Cinematographe projected films to a group."
    )
};

// choices[0] belongs to questions[0], choices[1] to questions[1], and so on - Raifu
vector<vector<string>> choices = {
    {   "It allowed films to be projected for an audience.",
        "It only recorded sound.",
        "It allowed one person to watch through a peephole.",
        "It was used only for animation." },
    {   "To project movies onto large screens.",
        "To record moving images on film.",
        "To record voices and music.",
        "To create computer animations." },
    {   "On a large cinema screen.",
        "On a television.",
        "Through a peephole, usually one person at a time.",
        "Through a smartphone." },
    {   "Film and photography.",
        "Moving pictures and recorded sound.",
        "Animation and painting.",
        "Film and radio broadcasting." },
    {   "They introduced the first talking movie.",
        "They presented short motion pictures to a paying audience.",
        "They introduced color television.",
        "They created the first animated feature film." },
    {   "Films only showed fictional stories.",
        "Films could document real people, work, and everyday life.",
        "Films were only scientific experiments.",
        "Films were created entirely by computers." },
    {   "Edison's system used individual peephole viewers, while the Cinematographe could project films to an audience.",
        "Both systems were designed only for recording sound.",
        "Edison's system was used only for animation.",
        "The Cinematographe could not record moving images." }
};

int main() {
    InitWindow(1500, 800, "Team 13 Hackathon");
    SetTargetFPS(30);

    Texture2D start_photo = LoadTexture("Start_Screen.png"); //Image Generated Using Claude.
    Texture2D game_photo = LoadTexture("game_image.png");  //Image Generated Using Claude.
    Texture2D gameButton = LoadTexture("Answer_Button.png"); // Load button texture

    float frameHeight = (float)gameButton.height/NUM_FRAMES;
    SetTargetFPS(60);

    /* Button sprite setup - same crop (source2) and size (dest2) as before.
    gained from example - sprite button */
    Rectangle source2 = { 0, 0, 340, 340};
    Rectangle dest2 = { 0, 0, gameButton.width * 2.0f, gameButton.height * 2.0f };

    /* Answer_Button.png has a lot of see-through space around the real button, so the
    clickable area and the text have to follow the VISIBLE part of the button.
    GetImageAlphaBorder (raylib) finds the box around the non-transparent pixels. */
    Rectangle art = { 0, 0, source2.width, source2.height };
    {
        Image img = LoadImage("Answer_Button.png");
        if (img.data != NULL) {
            Image cropped = ImageFromImage(img, source2);   // just the 340x340 part we draw
            Rectangle found = GetImageAlphaBorder(cropped, 0.1f);
            if (found.width > 0 && found.height > 0) art = found;
            UnloadImage(cropped);
        }
        UnloadImage(img);
    }
    float scaleX = dest2.width / source2.width;
    float scaleY = dest2.height / source2.height;
    float artW = art.width * scaleX,  artH = art.height * scaleY;     // visible button size
    float artOffX = art.x * scaleX,   artOffY = art.y * scaleY;       // where it starts inside dest2

    // The 4 visible buttons: A, B on top, C, D below (same spots as the 4 DrawTexturePro calls).
    // Used for text placement AND for checking which button was clicked.
    // Used Gemini to help debug button and make it easier to draw and clickable w text over it.
    Rectangle buttons[4] = {
        { 105 + artOffX, 100 + artOffY, artW, artH },
        { 605 + artOffX, 100 + artOffY, artW, artH },
        { 105 + artOffX, 250 + artOffY, artW, artH },
        { 605 + artOffX, 250 + artOffY, artW, artH }
    };

    // prize ladder: prizes[0] is for question 1, and so on (7 questions)
    vector<int> prizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    int total = (int)questions.size();

    /* Used Gemini To help figure out why screen wouldn't stay after clicking enter
    and switching back to the start screen
    */
    while (!WindowShouldClose()) {
        UpdateGame(questions);
        BeginDrawing();
        ClearBackground(WHITE);

        if (currentScreen == START_SCREEN) {
            //Drew the Start Screen
            Rectangle source = { 0, 0, (float)start_photo.width, (float)start_photo.height };
            Rectangle dest   = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(start_photo, source, dest, {0, 0}, 0.0f, WHITE);
        }
        else if (currentScreen == GAMEPLAY_SCREEN) {
            // Drew The GamePlay BackGround.
            Rectangle source = { 0, 0, (float)game_photo.width, (float)game_photo.height };
            Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(game_photo, source, dest, {0, 0}, 0.0f, WHITE);

            Question q = questions[current];          // current question
            vector<string> labels = choices[current]; // its 4 answers

            //which question we are on + prize so far
            DrawText(TextFormat("Question %d of %d", current + 1, total), 30, 20, 24, WHITE);
            DrawText(TextFormat("Prize: $%d", money), GetScreenWidth() - 230, 20, 24, GOLD);
            DrawText(q.title.c_str(), 600, 60, 24, GOLD);

            //Question wrapped onto several lines so long questions fit on the screen
            DrawWrapped(q.text, 100, 100, 60, 36, WHITE);

            /* ADDED all 4 Answer Boxes to the Game Screen - exactly the same 4 calls as before.
            They are always drawn, whether or not an answer was picked. */
            DrawTexturePro(gameButton, source2, dest2, Vector2{-105, -100}, 0.0f, WHITE);
            DrawTexturePro(gameButton, source2, dest2, Vector2{-605, -100}, 0.0f, WHITE);
            DrawTexturePro(gameButton, source2, dest2, Vector2{-105, -250}, 0.0f, WHITE);
            DrawTexturePro(gameButton, source2, dest2, Vector2{-605, -250}, 0.0f, WHITE);

            // The 4 answers for this question
            string A = labels[0];
            string B = labels[1];
            string C = labels[2];
            string D = labels[3];
            /* Text for each answer on each button, centered on the button (same math as before).
            converted string to c_str() gained from geeksforgeeks. (https://www.geeksforgeeks.org/cpp/basic_string-c_str-function-in-c-stl/) */
            DrawText(A.c_str(), 210 + (dest2.width - MeasureText(A.c_str(), 25)) / 2, 125 + (dest2.height - 25) / 2, 18, WHITE);
            DrawText(B.c_str(), 710 + (dest2.width - MeasureText(B.c_str(), 25)) / 2, 125 + (dest2.height - 25) / 2, 18, WHITE);
            DrawText(C.c_str(), 210 + (dest2.width - MeasureText(C.c_str(), 25)) / 2, 275 + (dest2.height - 25) / 2, 18, WHITE);
            DrawText(D.c_str(), 710 + (dest2.width - MeasureText(D.c_str(), 25)) / 2, 275 + (dest2.height - 25) / 2, 18, WHITE);
            /* Gained From example - sprite button used to read the text on the
            Button and we could be able to switch the text on the button for whenever.
            As well as example - input mouse to get the position of the mouse wherever
            to be able to track which button was clicked.
            (buttonA-D are the visible part of each button image, from the buttons[] array above) */
            if (picked == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                Rectangle buttonA = buttons[0];
                Rectangle buttonB = buttons[1];
                Rectangle buttonC = buttons[2];
                Rectangle buttonD = buttons[3];
                cout << "You clicked: ";
                if (CheckCollisionPointRec(mousePos, buttonA)) {
                    cout << A << endl;
                    picked = 0;
                }
                else if (CheckCollisionPointRec(mousePos, buttonB)) {
                    cout << B << endl;
                    picked = 1;
                }
                else if (CheckCollisionPointRec(mousePos, buttonC)) {
                    cout << C << endl;
                    picked = 2;
                }
                else if (CheckCollisionPointRec(mousePos, buttonD)) {
                    cout << D << endl;
                    picked = 3;
                }
                else {
                    cout << "nothing" << endl;
                }
                // right answer: win the prize for this question
                if (picked == q.correct) {
                    money = prizes[current];
                }
            }

            /* After an answer is clicked: tell them right or wrong, show the explanation + source
            in the empty space between the question and the buttons, and what to do next. */
            if (picked != -1) {
                DrawFeedback(q, labels, picked, current == total - 1);
            }
        }
        else if (currentScreen == GAME_OVER_SCREEN) {
            Rectangle source = { 0, 0, (float)game_photo.width, (float)game_photo.height };
            Rectangle dest   = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(game_photo, source, dest, {0, 0}, 0.0f, WHITE);
            DrawText("GAME OVER", 500, 280, 80, RED);
            DrawText(TextFormat("You leave with $%d", money), 500, 400, 40, WHITE);
            DrawText("Press ENTER to return to the start screen", 500, 500, 28, LIGHTGRAY);
        }
        else if (currentScreen == WIN_SCREEN) {
            Rectangle source = { 0, 0, (float)game_photo.width, (float)game_photo.height };
            Rectangle dest   = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(game_photo, source, dest, {0, 0}, 0.0f, WHITE);
            DrawText("YOU WON!", 500, 280, 80, GREEN);
            DrawText(TextFormat("Total: $%d", money), 500, 400, 40, WHITE);
            DrawText("Press ENTER to return to the start screen", 500, 500, 28, LIGHTGRAY);
        }
        EndDrawing();
    }

    UnloadTexture(start_photo);
    UnloadTexture(game_photo);
    UnloadTexture(gameButton);
    CloseWindow();

    return 0;
}