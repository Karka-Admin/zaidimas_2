#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <random>

// ZAIDIMO NUSTATYMAI
struct Options
{
	uint8_t		minSpeed	= 1;	// Maziausias laselio greitis
	uint8_t		maxSpeed	= 4;	// Didziausias laselio greitis

	uint8_t		enemyCount	= 7;	// Laseliu skaicius

	uint8_t		minAlpha	= 100;	// Maziausias laselio permatomumas
	uint8_t		maxAlpha	= 255;	// Didziausias laselio permatomumas

	uint16_t	minX		= 0;	// Kairiausia laselio pozicija
	uint16_t	maxX		= 950;	// Desiniausia laselio pozicija
};

// ZAIDEJO JUDEJIMO VALDYMAS
void playerMovement(sf::RectangleShape &player)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x < 950)
	{
		player.move(4.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > -1)
	{
		player.move(-4.f, 0.f);
	}
}

// ATSITIKTINIU SKAICIU GENERATORIUS (su overloadais)
float randomizer(uint16_t &min, uint16_t &max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(min, max);

	return float(distr(gen));
}

float randomizer(uint8_t &min, uint8_t &max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(min, max);

	return float(distr(gen));
}

// LANGO IVYKIU TIKRINIMAS
void checkWindowEvents(sf::RenderWindow &window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		window.close();
	}
}

int main()
{
	// LANGAS
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Avoid the rain!", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 10));
	window.setFramerateLimit(300);

	// NUSTATYMAI
	Options options;

	// MUZIKA
	sf::Music music;
	if (!music.openFromFile("music.ogg"))
	{
		return -1;
	}

	// VEIKEJAS
	sf::Vector2f playerSize(50.f, 50.f);
	sf::RectangleShape player(playerSize);
	sf::Texture	playerTexture;

	if (!playerTexture.loadFromFile("sugar.jpg"))
	{
		return -1;
	}

	player.setTexture(&playerTexture);
	player.setPosition(0.f, 850.f);

	// LIETUS
	std::vector<std::pair<float, float>> enemyProperties(options.enemyCount);
	sf::Vector2f enemySize(20.f, 65.f);
	sf::RectangleShape enemy(enemySize);
	std::vector <sf::RectangleShape> enemies(options.enemyCount);

	enemy.setFillColor(sf::Color(0, 255, 255, 255));

	for (uint8_t i = 0; i < options.enemyCount; i++)
	{
		enemyProperties[i].first = randomizer(options.minX, options.maxX);
		enemyProperties[i].second = randomizer(options.minSpeed, options.maxSpeed);
	}

	for (uint8_t i = 0; i < options.enemyCount; i++)
	{
		enemies[i] = enemy;
		enemies[i].setPosition(randomizer(options.minX, options.maxX), 0.f);
		enemies[i].setFillColor(sf::Color(0, 255, 255, randomizer(options.minAlpha, options.maxAlpha)));
	}

	// TEKSTAS
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
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
	gameOverText.setPosition(500.f, 250.f);

	// "PRESS ESC TO QUIT!"
	sf::Text exitText;

	exitText.setFont(font);
	exitText.setString("PRESS ESC TO QUIT");
	exitText.setCharacterSize(75);
	exitText.setFillColor(sf::Color::Red);
	exitText.setOrigin
	(
		exitText.getLocalBounds().left + exitText.getGlobalBounds().width / 2.0f,
		exitText.getLocalBounds().top + exitText.getGlobalBounds().height / 2.0f
	);
	exitText.setPosition(500.f, 500.f);

	// "PRESS ENTER TO START"
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
	window.clear();
	window.draw(gameStartText);
	window.display();

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && window.isOpen())
	{
		checkWindowEvents(window);
	}

	while (window.isOpen())
	{
		// GROTI MUZIKA
		if (music.getStatus() == sf::Sound::Stopped)
		{
			music.play();
		}

		// LANGO VALDYMAS
		checkWindowEvents(window);

		// LIETAUS JUDEJIMAS/VALDYMAS
		for (uint8_t i = 0; i < options.enemyCount; i++)
		{
			if (enemies[i].getPosition().y > 950.f)
			{
				enemyProperties[i].first = randomizer(options.minX, options.maxX);
				enemyProperties[i].second = randomizer(options.minSpeed, options.maxSpeed);

				enemies[i].setPosition(enemyProperties[i].first, 0.f);
				enemies[i].setFillColor(sf::Color(0, 255, 255, randomizer(options.minAlpha, options.maxAlpha)));

				score++;
			}
		}

		switch (score)
		{
		case 50:
			options.minSpeed = 3;
			options.maxSpeed = 6;
			break;

		case 100:
			options.minSpeed = 4;
			options.maxSpeed = 7;
			break;

		case 150:
			options.minSpeed = 5;
			options.maxSpeed = 8;
			break;

		case 200:
			options.minSpeed = 6;
			options.maxSpeed = 9;
			break;

		case 250:
			options.minSpeed = 7;
			options.maxSpeed = 10;
			break;

		case 300:
			options.minSpeed = 8;
			options.maxSpeed = 11;
			break;

		default:
			break;
		}

		// LASU KRITIMAS
		for (uint8_t i = 0; i < options.enemyCount; i++)
		{
			enemies[i].setPosition(enemyProperties[i].first, enemies[i].getPosition().y + enemyProperties[i].second);
		}

		// AR ZAIDEJAS PALIETE LASA
		for (uint8_t i = 0; i < options.enemyCount; i++)
		{
			if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
			{
				window.draw(gameOverText);
				window.draw(exitText);

				window.display();

				music.stop();

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

		for (uint8_t i = 0; i < options.enemyCount; i++)
		{
			window.draw(enemies[i]);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}