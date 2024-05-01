#pragma once
#include <SFML/Graphics.hpp>
#include <string>

typedef void(*ButtonClick)();

class Button {
public:
	ButtonClick onClick = []() {};
	Button(){}
	Button(std::string btnTxt, sf::Vector2f position, sf::Vector2f size, sf::Color btnClr,sf::Font& font, ButtonClick clickFunc) {
		text.setString(btnTxt);
		text.setFont(font);
		btn.setFillColor(btnClr);
		btn.setPosition(position);
		btn.setSize(size);
		onClick = clickFunc;
		
		// Center the text inside the button
		sf::FloatRect textSize = text.getLocalBounds();
		sf::Vector2f textPos= text.getPosition();
		sf::FloatRect btnSize = btn.getLocalBounds();
		sf::Vector2f btnPos = btn.getPosition();
		text.setOrigin(sf::Vector2f(textPos.x + textSize.width / 2, textPos.y + textSize.height / 2));
		text.setPosition(sf::Vector2f(btnPos.x + btnSize.width/2, btnPos.y + btnSize.height / 2 - 10));
	}
	
	bool isHovering(sf::RenderWindow &window) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

		return (mousePosition.x < (btn.getPosition().x + btn.getLocalBounds().width) &&
			mousePosition.x > btn.getPosition().x &&
			mousePosition.y < (btn.getPosition().y + btn.getLocalBounds().height) &&
			mousePosition.y > btn.getPosition().y
			);
	}
	void draw(sf::RenderWindow &window) const{
		window.draw(btn);
		window.draw(text);
	}

private:
	sf::RectangleShape btn;
	sf::Text text;
};