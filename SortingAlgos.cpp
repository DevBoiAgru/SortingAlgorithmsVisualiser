#include "SortingAlgos.h"


sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Sorting visualiser - DevBoiAgru");
sf::Font font;
int LIST_SIZE = 150;

int main(){
	font.loadFromFile("assets/Roboto-Regular.ttf");
	sf::Text listNum;
	// Adding leading zeroes, modified from https://stackoverflow.com/questions/6143824/add-leading-zeroes-to-string-without-sprintf
	listNum.setString(std::string(3 - std::min(3, static_cast<int>(std::to_string(LIST_SIZE).length())), '0') + std::to_string(LIST_SIZE));
	listNum.setFont(font);
	listNum.setCharacterSize(40);
	sf::FloatRect listTextSize = listNum.getLocalBounds();
	sf::Vector2f listTextPos = listNum.getPosition();
	listNum.setOrigin(sf::Vector2f(listTextPos.x + listTextSize.width / 2, listTextPos.y + listTextSize.height / 2));

	// Create all the buttons
	std::vector<Button> buttons{
		Button(
			std::string("Shuffle"),
			sf::Vector2f(10.f, 10.f),
			sf::Vector2f(160.f, 70.f),
			sf::Color(0, 0, 255), font, []() {std::shuffle(numbers.begin(), numbers.end(), randomGen); }),
		Button(
			std::string("Reverse"),
			sf::Vector2f(180.f, 10.f),
			sf::Vector2f(160.f, 70.f),
			sf::Color(160, 15, 78), font, []() {std::reverse(numbers.begin(), numbers.end()); }),
		Button(
			std::string("Refresh"),
			sf::Vector2f(WINDOW_X - 320.f, 10.f),
			sf::Vector2f(160.f, 70.f),
			sf::Color(127, 200, 101), font,
			[]() {
				numbers.clear();
				for (int j = 1; j <= LIST_SIZE; j++) {
					numbers.push_back(j);
					drawRectangles(-1, j - 1, j - 2, true);
					std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
				}
			}),
		Button(
			std::string("Quit"),
			sf::Vector2f(WINDOW_X - 150.f, 10.f),
			sf::Vector2f(140.f, 70.f),
			sf::Color(180, 0, 0), font, []() {std::exit(0); }),
		Button(
			std::string("-"),
			sf::Vector2f(WINDOW_X / 2 - 70 - 70, 10.f),
			sf::Vector2f(70.f, 70.f),
			sf::Color(6, 57, 113), font,
			[]() {
				if (LIST_SIZE > 10)
					LIST_SIZE -= 10;
				else if (LIST_SIZE > 5) 
					LIST_SIZE -=5;
			}),
		Button(
			std::string("+"),
			sf::Vector2f(WINDOW_X / 2 + 70, 10.f),
			sf::Vector2f(70.f, 70.f),
			sf::Color(44, 85, 69), font, []() {if (LIST_SIZE == 5) LIST_SIZE += 5; else LIST_SIZE += 10; }),
		Button(
			std::string("Selection sort"),
			sf::Vector2f(10.f, 90.f),
			sf::Vector2f(210.f, 70.f),
			sf::Color(215, 45, 109), font, []() {SelectionSort(numbers); }),
		Button(
			std::string("Bubble sort"),
			sf::Vector2f(230.f, 90.f),
			sf::Vector2f(170.f, 70.f),
			sf::Color(0, 200, 200), font, []() {BubbleSort(numbers); }),
		Button(
			std::string("Insertion sort"),
			sf::Vector2f(410.f, 90.f),
			sf::Vector2f(190.f, 70.f),
			sf::Color(150, 100, 200), font, []() {InsertionSort(numbers); }),
		Button(
			std::string("Quick sort"),
			sf::Vector2f(610.f, 90.f),
			sf::Vector2f(150.f, 70.f),
			sf::Color(215, 45, 109), font, []() {QuickSort(numbers, 0, numbers.size() -1); }),
		Button(
			std::string("Bogo sort"),
			sf::Vector2f(770.f, 90.f),
			sf::Vector2f(140.f, 70.f),
			sf::Color(137, 172, 118), font, []() {BogoSort(numbers); }),
		Button(
			std::string("Stalin sort"),
			sf::Vector2f(920.f, 90.f),
			sf::Vector2f(150.f, 70.f),
			sf::Color(255, 0, 0), font, []() {StalinSort(numbers); }),
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
		window.clear();

		drawRectangles(-1, -1, -1, false);
		
		// Draw all buttons after the rectangles, so that they are always on top.
		for (Button btn : buttons)
			btn.draw(window);

		// Draw the current list size on the top - middle of the screen
		listNum.setString(std::string(3 - std::min(3, static_cast<int>(std::to_string(LIST_SIZE).length())), '0') + std::to_string(LIST_SIZE));
		listNum.setPosition(sf::Vector2f(WINDOW_X/2, 30));
		window.draw(listNum);

		window.display();
	}

}

static void drawRectangles(int highlight1, int highlight2, int highlight3 , bool display) {
	window.clear();
	std::vector<sf::RectangleShape> rectList;
	double rectWidth = (static_cast<double>(WINDOW_X) - (2 * SIDEOFFSET)) / numbers.size();

	// 0.5 means not take up more than half of the screen
	double heightFactor = static_cast<double>(WINDOW_Y) / maxNumber(numbers) * 0.5;
	for (int i = 0; i < numbers.size(); i++) {
		int num = numbers[i];
		float rectHeight = num * heightFactor;
		sf::RectangleShape rect(sf::Vector2f(rectWidth, rectHeight));

		if (i == highlight1)
			rect.setFillColor(sf::Color(255, 0, 0));			// Highlight red
		else if (i == highlight2)
			rect.setFillColor(sf::Color(0, 255, 0));			// Highlight green
		else if (i == highlight3)
			rect.setFillColor(sf::Color(0, 0, 255));			// Highlight blue
		
		rect.setPosition(sf::Vector2f(SIDEOFFSET + i * rectWidth, WINDOW_Y - rectHeight));
		rectList.push_back(rect);
		window.draw(rect);
	}
	if (display)
		window.display();
}