#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "celda.cpp"

int main() {
    const int gridSize = 9;      // Tamaño total de la cuadrícula 9x9
    const int cellSize = 50;     // Tamaño de cada celda en la pantalla
    const int windowWidth = 700; // Ancho de la ventana (incluye menú)
    const int windowHeight = gridSize * cellSize; // Alto de la ventana

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SUDOKU");
    Celda grid[3][3];  // Matriz de 3x3 de objetos Celda
    bool isEditingEnabled = false; // Controla si se permite modificar la cuadrícula

    // Cargar fuente para mostrar texto en las celdas y el menú
    sf::Font font;
    if (!font.loadFromFile("resources/Arial.ttf")) {
        std::cerr << "No se pudo cargar la fuente Arial.ttf" << std::endl;
        return -1;
    }

    // Configurar botones del menú
    sf::Text newGameText("Nuevo Juego", font, 20);
    sf::Text loadGameText("Cargar", font, 20);
    sf::Text solveText("Solucionar", font, 20);
    
    // Asegúrate de que estén dentro del ancho visible de la ventana
    newGameText.setPosition(windowWidth - 180, 50);
    loadGameText.setPosition(windowWidth - 180, 100);
    solveText.setPosition(windowWidth - 180, 150);

    // Configurar la cuadrícula de celdas
    sf::RectangleShape cells[gridSize][gridSize];
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            cells[i][j].setSize(sf::Vector2f(cellSize - 1, cellSize - 1)); // Tamaño de la celda
            cells[i][j].setPosition(j * cellSize, i * cellSize);           // Posición en la cuadrícula
            cells[i][j].setFillColor(sf::Color::White);
            cells[i][j].setOutlineThickness(1);
            cells[i][j].setOutlineColor(sf::Color::Red);
        }
    }

    int selectedRow = -1, selectedCol = -1; // Almacena la celda seleccionada

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Detectar clic en una celda de la cuadrícula
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;

                    // Verificar si el clic es en la cuadrícula
                    if (x < gridSize * cellSize) {
                        int row = y / cellSize;
                        int col = x / cellSize;

                        if (row >= 0 && row < gridSize && col >= 0 && col < gridSize) {
                            if (isEditingEnabled) {
                                selectedRow = row;
                                selectedCol = col;
                            }
                        }
                    }

                    // Verificar si el clic es en los botones del menú
                    else if (x >= windowWidth - 200) {
                        if (newGameText.getGlobalBounds().contains(x, y)) {
                            isEditingEnabled = true;  // Habilitar edición para "Nuevo Juego"
                            selectedRow = selectedCol = -1; // Des-seleccionar cualquier celda
                            // Lógica para "Nuevo Juego": resetear la cuadrícula
                            for (int i = 0; i < 3; ++i) {
                                for (int j = 0; j < 3; ++j) {
                                    for (int k = 0; k < 3; ++k) {
                                        for (int l = 0; l < 3; ++l) {
                                            grid[i][j].valores[k][l] = 0;
                                        }
                                    }
                                }
                            }
                        } else if (loadGameText.getGlobalBounds().contains(x, y)) {
                            // Lógica para "Cargar"
                            std::cout << "Cargar juego presionado" << std::endl;
                        } else if (solveText.getGlobalBounds().contains(x, y)) {
                            // Lógica para "Solucionar"
                            std::cout << "Solucionar presionado" << std::endl;
                        }
                    }
                }
            }

            // Detectar entrada de texto en la celda seleccionada
            if (event.type == sf::Event::TextEntered) {
                if (isEditingEnabled && selectedRow != -1 && selectedCol != -1) {
                    char inputChar = static_cast<char>(event.text.unicode);
                    if (inputChar >= '1' && inputChar <= '9') {
                        int subGridRow = selectedRow / 3;
                        int subGridCol = selectedCol / 3;
                        int cellRow = selectedRow % 3;
                        int cellCol = selectedCol % 3;

                        grid[subGridRow][subGridCol].obtenerValor(cellRow, cellCol) = inputChar - '0'; // Guardar valor
                    } else if (inputChar == '0') {
                        int subGridRow = selectedRow / 3;
                        int subGridCol = selectedCol / 3;
                        int cellRow = selectedRow % 3;
                        int cellCol = selectedCol % 3;

                        grid[subGridRow][subGridCol].obtenerValor(cellRow, cellCol) = 0;
                    }
                }
            }
        }

        // Renderizar la cuadrícula
        window.clear(sf::Color::White);

        // Dibujar celdas de la cuadrícula
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                if (i == selectedRow && j == selectedCol) {
                    cells[i][j].setFillColor(sf::Color(173, 216, 230)); // Azul claro para celda seleccionada
                } else {
                    cells[i][j].setFillColor(sf::Color::White); // Blanco para celdas no seleccionadas
                }
                window.draw(cells[i][j]);

                int subGridRow = i / 3;
                int subGridCol = j / 3;
                int cellRow = i % 3;
                int cellCol = j % 3;

                int value = grid[subGridRow][subGridCol].obtenerValor(cellRow, cellCol);
                if (value != 0) {
                    sf::Text cellText;
                    cellText.setFont(font);
                    cellText.setString(std::to_string(value));
                    cellText.setCharacterSize(24);
                    cellText.setFillColor(sf::Color::Black);
                    cellText.setPosition(j * cellSize + 15, i * cellSize + 10);
                    window.draw(cellText);
                }
            }
        }

        // Dibujar opciones del menú
        window.draw(newGameText);
        window.draw(loadGameText);
        window.draw(solveText);

        window.display();
    }

    return 0;
}