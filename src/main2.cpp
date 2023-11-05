#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Input");

    // Установите локаль на уровне программы
    std::locale::global(std::locale("C.utf8"));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                // Теперь должно быть правильное значение для символа "z"
                std::wcout << L"Input: " << static_cast<wchar_t>(event.text.unicode) << std::endl;
            }

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    return 0;
}
