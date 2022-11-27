#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <random>

#define ENEMY_COUNT	8
int minSpeed = 2;
int maxSpeed = 5;


void playerMovement(sf::RectangleShape &player)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x < 950)
	{
		player.move(5.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > -1)
	{
		player.move(-5.f, 0.f);
	}
	
	/*else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.getPosition().y > -1)
	{
		player.move(0.f, -5.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y < 950)
	{
		player.move(0.f, 5.f);
	}*/
}

float randomizerX()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 950);

	return float(distr(gen));
}

float randomizerSpeed()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(minSpeed, maxSpeed);

	return float(distr(gen));
}

float randomizerColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(100, 255);

	return float(distr(gen));
}

void checkWindowEvents(sf::RenderWindow &window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		window.close();
	}
}

int main()
{
	// Langas
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Avoid the rain!", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 10));
	window.setFramerateLimit(500);

	// Muzika
	sf::Music music;
	if (!music.openFromFile("music.ogg"))
	{
		return -1;
	}
	music.play();

	// Veikejas
	sf::Vector2f playerSize(50.f, 50.f);
	sf::RectangleShape player(playerSize);
	//player.setFillColor(sf::Color::Red);
	player.setPosition(0.f, 850.f);

	sf::Texture texture;
	if (!texture.loadFromFile("sugar.jpg"))
	{
		return -1;
	}

	player.setTexture(&texture);

	// Priesai
	std::pair<float, float> enemyProperties[ENEMY_COUNT];
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemyProperties[i].first = randomizerX();
		enemyProperties[i].second = randomizerSpeed();
	}

	sf::Vector2f enemySize(15.f, 70.f);
	sf::RectangleShape enemy(enemySize);
	enemy.setFillColor(sf::Color(0, 255, 255, 200));

	std::vector <sf::RectangleShape> enemies(ENEMY_COUNT);
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies[i] = enemy;
		enemies[i].setPosition(randomizerX(), 0.f);
		enemies[i].setFillColor(sf::Color(0, 255, 255, randomizerColor()));
	}

	// TEKSTAS
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		std::cerr << "ERROR\n";
		return -1;
	}

	// TASKAI
	uint64_t score = 0;
	std::stringstream ss;
	std::string scoreString;
	sf::Text scoreText;

	scoreText.setFont(font);
	scoreText.setString(scoreString);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::Green);
	scoreText.setPosition(0, 0);

	// "GAME OVER!"
	sf::Text gameOverText;

	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER!");
	gameOverText.setCharacterSize(100);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setOrigin
	(
		gameOverText.getLocalBounds().left + gameOverText.getGlobalBounds().width / 2.0f,
		gameOverText.getLocalBounds().top + gameOverText.getGlobalBounds().height / 2.0f
	);
	gameOverText.setPosition(500.f, 500.f);

	// "PRADETI ZAIDIMA?"
	sf::Text gameStartText;

	gameStartText.setFont(font);
	gameStartText.setString("PRESS ENTER TO START");
	gameStartText.setCharacterSize(72);
	gameStartText.setFillColor(sf::Color::Red);
	gameStartText.setOrigin
	(
		gameStartText.getLocalBounds().left + gameStartText.getGlobalBounds().width / 2.0f,
		gameStartText.getLocalBounds().top + gameStartText.getGlobalBounds().height / 2.0f
	);
	gameStartText.setPosition(500.f, 500.f);

	// PRADZIOS LANGAS
	window.draw(gameStartText);
	window.display();

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && window.isOpen())
	{
		checkWindowEvents(window);
	}

	while (window.isOpen())
	{
		// LANGO VALDYMAS
		checkWindowEvents(window);

		// LIETAUS JUDEJIMAS/VALDYMAS
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (enemies[i].getPosition().y > 950.f)
			{
				enemyProperties[i].first = randomizerX();
				enemyProperties[i].second = randomizerSpeed();

				enemies[i].setPosition(enemyProperties[i].first, 0.f);
				enemies[i].setFillColor(sf::Color(0, 255, 255, randomizerColor()));

				score++;
			}
		}

		switch (score)
		{
		case 50:
			minSpeed = 3;
			maxSpeed = 6;
			break;

		case 100:
			minSpeed = 4;
			maxSpeed = 7;
			break;

		case 150:
			minSpeed = 5;
			maxSpeed = 8;
			break;

		default:
			break;
		}

		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			enemies[i].setPosition(enemyProperties[i].first, enemies[i].getPosition().y + enemyProperties[i].second);
		}

		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
			{
				window.draw(gameOverText);
				window.display();

				while (window.isOpen())
				{
					checkWindowEvents(window);
				}
			}
		}

		// ZAIDEJO JUDEJIMAS/VALDYMAS
		playerMovement(player);

		// TASKU KEITIMAS
		ss.clear();
		ss << score;
		ss >> scoreString;
		scoreText.setString("Raindrops:" + scoreString);

		// PIESIMAS
		window.clear();

		window.draw(player);
		window.draw(scoreText);
		
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			window.draw(enemies[i]);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}