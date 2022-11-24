#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <random>

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
		player.move(0.f, -1.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y < 1030)
	{
		player.move(0.f, 1.f);
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

int main()
{
	// Langas
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Avoid the rain!", sf::Style::Close);
	window.setFramerateLimit(300);

	// Veikejas
	sf::Vector2f playerSize(50.f, 50.f);
	sf::RectangleShape player(playerSize);
	player.setFillColor(sf::Color::Yellow);
	player.setPosition(0.f, 850.f);

	// Priesai
	sf::Vector2f enemySize(50.f, 50.f);
	sf::RectangleShape enemy(enemySize);
	enemy.setFillColor(sf::Color::Red);

	// Taskai
	uint64_t score = 0;
	

	float randomX = 0, randomSpeed = 2.f;

	while (window.isOpen())
	{
		playerMovement(player);

		if (enemy.getPosition().y > 950.f)
		{
			randomX = randomizerX();
			randomSpeed = randomizerSpeed();

			enemy.setPosition(randomX, 0.f);
		}
		enemy.setPosition(randomX, enemy.getPosition().y + randomSpeed);

		window.clear();

		window.draw(player);
		window.draw(enemy);

		window.display();
		
		score++;
	}

	return 0;
}