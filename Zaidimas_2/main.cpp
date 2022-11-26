#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <random>

#define ENEMY_COUNT	5

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
	std::uniform_int_distribution<> distr(3, 6);

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
	window.setFramerateLimit(300);

	// Veikejas
	sf::Vector2f playerSize(50.f, 50.f);
	sf::RectangleShape player(playerSize);
	player.setFillColor(sf::Color::Red);
	player.setPosition(0.f, 850.f);

	// Priesai
	std::pair<float, float> enemyProperties[ENEMY_COUNT];
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemyProperties[i].first = randomizerX();
		enemyProperties[i].second = randomizerSpeed();
	}

	sf::Vector2f enemySize(50.f, 50.f);
	sf::RectangleShape enemy(enemySize);
	enemy.setFillColor(sf::Color::Cyan);

	std::vector <sf::RectangleShape> enemies(ENEMY_COUNT);
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies[i] = enemy;
		enemies[i].setPosition(randomizerX(), 0.f);
	}

	// TEKSTAS
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		std::cerr << "ERROR\n";
		return -1;
	}

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
			}
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

		// PIESIMAS
		window.clear();
		window.draw(player);
		
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			window.draw(enemies[i]);
		}

		window.display();
	}

	return 0;
}