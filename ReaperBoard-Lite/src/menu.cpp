#include "menu.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

bool MenuDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        buttonPressed = true;
        index[1] = (index[1] + 1) % MENU_COLS;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        char buffer[9];
        strcpy_P(buffer, menuLayout[index[0]][index[1]]);
        selectedItem = String(buffer);
        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        buttonPressed = true;
        index[0] = (index[0] + 1) % MENU_ROWS;
    }

    return buttonPressed;
}

void MenuDisplay::displayScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0,0);

    char buffer[9];

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 2; col++) {
            display.setTextColor(SSD1306_WHITE);

            strcpy_P(buffer, menuLayout[row][col]);

            int x = col == 0 ? 7 : 63;
            int y = 7 + row * (6 + 8);

            if (row == index[0] && col == index[1]) {
                display.setTextColor(SSD1306_BLACK);
                display.fillRect(x - 3, y - 3, 54, 14, SSD1306_WHITE);
            }
            display.setCursor(x, y);
            display.print(buffer);
        }
    }

    renderSidebar(Icon::LOGO, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}