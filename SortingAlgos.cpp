#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "SortingAlgos.h"
#include <math.h>

sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Sorting visualiser - DevBoiAgru");
sf::Font font;

int LIST_SIZE = 250;

int main(int argc, char* argv[])
{
    if (argc > 1) {
        char* sortDelayStr = argv[1];

        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
            std::cout << "Command line usage: sortingalgo <delay between operations in nanoseconds>\n"
                      << "For example:\n"
                      << "	sortingalgos.exe 10\n"
                      << "	This will make it so that the hold between each comparision is 10 nanoseconds.\n";
            return 0;
        }

        // Validate if string is a valid number
        if (is_number(sortDelayStr)) {
            SORT_DELAY = atoi(sortDelayStr);
        } else {
            std::cout << "Invalid delay input.\n"
                      << "Usage: sortingalgo <delay between operations in nanoseconds>\n"
                      << "For example:\n"
                      << "	sortingalgos.exe 10\n"
                      << "	This will make it so that the hold between each comparision is 10 nanoseconds.\n";
            return 1;
        }
    }

    font.loadFromFile("assets/Roboto-Regular.ttf");
    sf::Text listNum;
    // Adding leading zeroes, modified from https://stackoverflow.com/questions/6143824/add-leading-zeroes-to-string-without-sprintf
    listNum.setString(std::string(3 - std::min(3, static_cast<int>(std::to_string(LIST_SIZE).length())), '0') + std::to_string(LIST_SIZE));
    listNum.setFont(font);
    listNum.setCharacterSize(40);
    sf::FloatRect listTextSize = listNum.getLocalBounds();
    sf::Vector2f listTextPos = listNum.getPosition();
    // Use width/height members of sf::Rect<float> and ensure float arithmetic
    listNum.setOrigin(sf::Vector2f(listTextPos.x + listTextSize.width / 2.f, listTextPos.y + listTextSize.height / 2.f));

    // Create all the buttons
    std::vector<Button> buttons {
        Button(
            std::string("Shuffle"),
            sf::Vector2f(10.f, 10.f),
            sf::Vector2f(160.f, 70.f),
            sf::Color(0, 0, 255), font, []() {if (canClick) std::shuffle(numbers.begin(), numbers.end(), randomGen); }),
        Button(
            std::string("Reverse"),
            sf::Vector2f(180.f, 10.f),
            sf::Vector2f(160.f, 70.f),
            sf::Color(160, 15, 78), font, []() {if (canClick) std::reverse(numbers.begin(), numbers.end()); }),
        Button(
            std::string("Refresh"),
            sf::Vector2f(WINDOW_X - 320.f, 10.f),
            sf::Vector2f(160.f, 70.f),
            sf::Color(127, 200, 101), font,
            []() {
                if (canClick) {
                    canClick = false;
                    numbers.clear();
                    for (int j = 1; j <= LIST_SIZE; j++) {
                        numbers.push_back(j);
                        HighlightRectangles(-1, j - 1, j - 2);
                        resetHighlights();
                    }
                    canClick = true;
                }
            }),
        Button(
            std::string("Quit"),
            sf::Vector2f(WINDOW_X - 150.f, 10.f),
            sf::Vector2f(140.f, 70.f),
            sf::Color(180, 0, 0), font, []() { std::exit(0); }),
        Button(
            std::string("-"),
            sf::Vector2f(WINDOW_X / 2 - 70 - 70, 10.f),
            sf::Vector2f(70.f, 70.f),
            sf::Color(6, 57, 113), font,
            []() {
                if (canClick) {
                    if (LIST_SIZE > 10 && canClick)
                        LIST_SIZE -= 10;
                    else if (LIST_SIZE > 5 && canClick)
                        LIST_SIZE -= 5;
                }
            }),
        Button(
            std::string("+"),
            sf::Vector2f(WINDOW_X / 2 + 70, 10.f),
            sf::Vector2f(70.f, 70.f),
            sf::Color(44, 85, 69), font, []() {if (canClick) { if (LIST_SIZE == 5) LIST_SIZE += 5; else LIST_SIZE += 10; } }),
        Button(
            std::string("Selection sort"),
            sf::Vector2f(10.f, 90.f),
            sf::Vector2f(210.f, 70.f),
            sf::Color(215, 45, 109), font, []() {if (canClick) { canClick = false; SelectionSort(numbers); canClick = true; resetHighlights();} }),
        Button(
            std::string("Bubble sort"),
            sf::Vector2f(230.f, 90.f),
            sf::Vector2f(170.f, 70.f),
            sf::Color(0, 200, 200), font, []() {if (canClick) { canClick = false; BubbleSort(numbers); canClick = true; resetHighlights();} }),
        Button(
            std::string("Insertion sort"),
            sf::Vector2f(410.f, 90.f),
            sf::Vector2f(190.f, 70.f),
            sf::Color(150, 100, 200), font, []() {if (canClick) { canClick = false; InsertionSort(numbers); canClick = true; resetHighlights(); } }),
        Button(
            std::string("Quick sort"),
            sf::Vector2f(610.f, 90.f),
            sf::Vector2f(150.f, 70.f),
            sf::Color(215, 45, 109), font, []() {if (canClick) { canClick = false; if (!numbers.empty()) { QuickSort(numbers, 0, static_cast<int>(numbers.size()) - 1); } canClick = true; resetHighlights(); } }),
        Button(
            std::string("Bogo sort"),
            sf::Vector2f(770.f, 90.f),
            sf::Vector2f(140.f, 70.f),
            sf::Color(137, 172, 118), font, []() {if (canClick) { canClick = false; BogoSort(numbers); canClick = true; resetHighlights();} }),
        Button(
            std::string("Stalin sort"),
            sf::Vector2f(920.f, 90.f),
            sf::Vector2f(150.f, 70.f),
            sf::Color(255, 0, 0), font, []() {if (canClick) { canClick = false;  StalinSort(numbers); canClick = true; resetHighlights(); } }),
    };

    for (int j = 1; j <= LIST_SIZE; j++)
        numbers.push_back(j);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                for (Button btn : buttons) {
                    if (btn.isHovering(window))
                        btn.onClick();
                }
            }
        }
        drawRectangles();

        // Draw all buttons after the rectangles, so that they are always on top.
        for (Button btn : buttons)
            btn.draw(window);

        // Draw the current list size on the top - middle of the screen
        listNum.setString(std::string(3 - std::min(3, static_cast<int>(std::to_string(LIST_SIZE).length())), '0') + std::to_string(LIST_SIZE));
        listNum.setPosition(sf::Vector2f(WINDOW_X / 2, 30));
        window.draw(listNum);

        window.display();
    }
}

static void drawRectangles()
{
    window.clear();
    std::vector<sf::RectangleShape> rectList;
    double rectWidth = (static_cast<double>(WINDOW_X) - (2 * SIDEOFFSET)) / numbers.size();

    // 0.5 means not take up more than half of the screen
    double heightFactor = static_cast<double>(WINDOW_Y) / maxNumber(numbers) * 0.5;
    for (int i = 0; i < numbers.size(); i++) {
        int num = numbers[i];
        // Ensure floating arithmetic and explicit casts to float
        float rectHeight = static_cast<float>(num * heightFactor);
        float rectWidthF = static_cast<float>(rectWidth);
        sf::RectangleShape rect(sf::Vector2f(rectWidthF, rectHeight));

        if (i == highlight1)
            rect.setFillColor(sf::Color(255, 0, 0)); // Highlight red
        else if (i == highlight2)
            rect.setFillColor(sf::Color(0, 255, 0)); // Highlight green
        else if (i == highlight3)
            rect.setFillColor(sf::Color(0, 0, 255)); // Highlight blue

        rect.setPosition(sf::Vector2f(static_cast<float>(SIDEOFFSET) + static_cast<float>(i) * rectWidthF, static_cast<float>(WINDOW_Y) - rectHeight));
        rectList.push_back(rect);
        window.draw(rect);
    }
}