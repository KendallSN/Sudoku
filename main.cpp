#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>  // Para srand() y rand()
#include <ctime>    // Para time()
#include "celda.cpp"

bool isNumberInRowOrColumn(int numero, int fila, int columna, Celda grid[3][3]) {
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                for (int l = 0; l < 3; l++){
                    if((i*3)+k ==fila && grid[i][j].valores[k][l]==numero){
                        return true;
                    }
                    if((j*3)+l == columna && grid[i][j].valores[k][l]==numero){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
void fillGrid(Celda grid[3][3]) {
    std::srand(static_cast<unsigned>(std::time(0))); // Inicializar la semilla del generador de números aleatorios

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                for (int l = 0; l < 3; l++){
                    std::vector<int> availableNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                    bool placed = false;

                    while (!availableNumbers.empty()) {
                        int index = std::rand() % availableNumbers.size();
                        int num = availableNumbers[index];

                        if (!isNumberInRowOrColumn(num, i*3+k, j*3+l, grid)) {
                            grid[i][j].valores[k][l] = num;
                            placed = true;
                            break;
                        } else {
                            availableNumbers.erase(availableNumbers.begin() + index);
                        }
                    }
                    if (!placed) {
                        for (int m = 0; m < 3; m++){
                            for (int n = 0; n < 3; n++){
                                for (int o = 0; o < 3; o++){
                                    for (int p = 0; p < 3; p++){
                                       grid[m][n]. valores[o][p]=0;
                                    }
                                }
                            }
                        }
                        i=j=k=l=-1;
                        break;
                    }
                }
            }
        }
    }
}
int main() {
    const int gridSize = 9;      // Tamaño total de la cuadrícula 9x9
    const int cellSize = 50;     // Tamaño de cada celda en la pantalla
    const int menuWidth = 200;   // Ancho del área del menú
    const int windowWidth = gridSize * cellSize + menuWidth; // Ancho de la ventana (cuadrícula + menú)
    const int windowHeight = gridSize * cellSize; // Alto de la ventana

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SUDOKU");
    Celda grid[3][3];  // Matriz de 3x3 de objetos Celda
    Celda gridToSolve[3][3]; // Matriz de 3x3 de objetos Celda para resolver
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> numbersAux;

    // Cargar fuente para mostrar texto en las celdas y el menú
    sf::Font font;
    if (!font.loadFromFile("resources/Arial.ttf")) {
        std::cerr << "No se pudo cargar la fuente Arial.ttf" << std::endl;
        return -1;
    }

    // Configurar botones del menú
    sf::Text newGameText("Nuevo Juego", font, 20);
    newGameText.setFillColor(sf::Color::Black);
    sf::Text loadGameText("Cargar", font, 20);
    loadGameText.setFillColor(sf::Color::Black);
    sf::Text solveText("Solucionar", font, 20);
    solveText.setFillColor(sf::Color::Black);

    // Configurar variable para controlar el modo de juego
    int gameMode = 0;
    // Inicializar la semilla del generador de números aleatorios
    std::srand(static_cast<unsigned>(std::time(0)));
    int random = 0;

    // Posicionar los textos del menú a la derecha de la cuadrícula
    newGameText.setPosition(gridSize * cellSize + 10, 50);
    loadGameText.setPosition(gridSize * cellSize + 10, 100);
    solveText.setPosition(gridSize * cellSize + 10, 150);

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
                            if (gameMode == 1) {
                                selectedRow = row;
                                selectedCol = col;
                            }
                        }
                    }

                    // Verificar si el clic es en los botones del menú
                    else if (x >= gridSize * cellSize) {
                        if (newGameText.getGlobalBounds().contains(x, y)) {
                            gameMode = 1;  // Habilitar edición para "Nuevo Juego"
                            selectedRow = selectedCol = -1; // Des-seleccionar cualquier celda
                            // Lógica para "Nuevo Juego": resetear la cuadrícula
                            for (int i = 0; i < 3; ++i) {
                                for (int j = 0; j < 3; ++j) {
                                    for (int k = 0; k < 3; ++k) {
                                        for (int l = 0; l < 3; ++l) {
                                            // grid[i][j].valores[k][l] = 0;
                                            // random = std::rand() % 100;
                                            // if(random > 0){
                                            //     gridToSolve[i][j].valores[k][l] = 1;
                                            //     numbersAux = numbers;
                                                
                                            //     while(grid[i][j].valores[k][l] == 0){
                                            //         int index = std::rand() % numbersAux.size();
                                            //         int num = numbersAux[index];
                                            //         if(!isNumberInRowOrColumn(num,(i*3)+k,(j*3)+l,grid)){
                                            //             grid[i][j].valores[k][l] = num;
                                            //         }else{
                                            //             numbersAux.erase(std::remove(numbersAux.begin(), numbersAux.end(), num), numbersAux.end());
                                            //         }
                                            //     }
                                            // }else{
                                            //     gridToSolve[i][j].valores[k][l] = 0;
                                            // }
                                        }
                                    }
                                }
                            }
                            fillGrid(grid);
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
                if (gameMode == 1 && selectedRow != -1 && selectedCol != -1) {
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
        window.setFramerateLimit(20);
        window.display();
    }

    return 0;
}
