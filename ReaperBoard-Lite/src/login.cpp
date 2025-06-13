#include "login.h"

extern Adafruit_SSD1306 display;
bool loggedIn = false;

bool LoginDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        buttonPressed = true;
        index[1] = (index[1] + 1) % MENU_COLS;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        char buffer[2];
        strcpy_P(buffer, loginLayout[index[0]][index[1]]);
        char selectedChar = buffer[0];

        if (selectedChar == '<' && charIndex > 0) {
            passcode[charIndex] = ' ';
            charIndex--;
        } else if (selectedChar == '*') {
            if (memcmp(passcode, hardCodedPassword, 6) == 0) {
                loggedIn = true;
            }
            charIndex = 0;
            memset(passcode, 0, sizeof(passcode));
        } else if (charIndex < 6 && selectedChar != '<') {
            passcode[charIndex] = selectedChar;
            charIndex++;
        }

        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        buttonPressed = true;
        index[0] = (index[0] + 1) % MENU_ROWS;
    }

    return buttonPressed;
}

void LoginDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0,0);

    char buffer[2];

    for (int row = 0; row < MENU_ROWS; row++) {
        for (int col = 0; col < MENU_COLS; col++) {
            display.setTextColor(SSD1306_WHITE);

            strcpy_P(buffer, loginLayout[row][col]);

            int x = 48 + col * (6 + 2);
            int y = 25 + row * (2 + 8);

            if (row == index[0] && col == index[1]) {
                display.setTextColor(SSD1306_BLACK);
                display.fillRect(x - 1, y - 1, 8, 10, SSD1306_WHITE);
            }
            display.setCursor(x, y);
            display.print(buffer);
        }
    }

    for (int i = 0; i < 6; i++) {
        int x = 41 + i * 7;
        int y = 8;

        display.setCursor(x, y);
        if (charIndex > i) {
            display.print("*");
        }

        display.setCursor(x, y + 2);
        display.print("_");
    }

    renderSidebar(Icon::LOGO, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}