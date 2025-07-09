#include "dataDisplay.h"

extern Adafruit_SSD1306 display;
extern bool awaitingExit;

bool DataDisplayDisplay::scanInputs() {
    bool buttonPressed = false;

    if (digitalRead(FIRST_BUTTON) == LOW) {
        awaitingExit = true;
        onExit();
        buttonPressed = true;
    }
    if (digitalRead(SECOND_BUTTON) == LOW) {
        if (currentPage == CurrentPage::DATA_LIST_PAGE) {
            if (SDManager::loadData(dataList[index], pageData)) {
                currentPage = CurrentPage::DATA_SHOW_PAGE;
            }
        }

        buttonPressed = true;
    }
    if (digitalRead(THIRD_BUTTON) == LOW) {
        if (dataLoaded != 0 && currentPage == CurrentPage::DATA_LIST_PAGE) { 
            index = (index + 1) % dataLoaded;
        }
        buttonPressed = true;
    }

    return buttonPressed;
}

void DataDisplayDisplay::displayScreen() {
    display.clearDisplay();
    display.setTextSize(1);

    if (currentPage == CurrentPage::DATA_LIST_PAGE) {
        if (dataLoaded != 0) {
            int start = (index >= 6 ? index - 5 : 0);
            for (int i = start; i < dataLoaded; i++) {
                int row = i - start;

                if (index == i) {
                    display.fillRect(0, (row * 10), 118, 10, SSD1306_WHITE);
                    display.setTextColor(SSD1306_BLACK);
                } else {
                    display.setTextColor(SSD1306_WHITE);
                }

                display.setCursor(1, (row * 10) + 1);
                display.print(dataList[i]);
            }
        } else {
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(1,1);
            display.print("No data to display");
            display.setCursor(1,10);
            display.print("Turn on your AP");
        }
    } else if (currentPage == CurrentPage::DATA_SHOW_PAGE) {
        display.setTextColor(SSD1306_WHITE);
    
        int y = 0;
        int lineHeight = 10; // Pixels per line
        int start = 0;

        while (true) {
            int end = pageData.indexOf('\n', start);
            if (end == -1) end = pageData.length();

            String line = pageData.substring(start, end);
            display.setCursor(0, y);
            display.print(line);

            y += lineHeight;
            if (end == pageData.length() || y >= display.height()) break;

            start = end + 1;
        }
    }

    renderSidebar(Icon::WIFI, Icon::RIGHT_ARROW, Icon::TARGET, Icon::DOWN_ARROW, -99);

    display.display();
}

void DataDisplayDisplay::onEnter() {
    dataLoaded = SDManager::listData(dataList, MAX_DATA);
    currentPage = CurrentPage::DATA_LIST_PAGE;
}

bool DataDisplayDisplay::timeoutEnabled() {
    return true;
}