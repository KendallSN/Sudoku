#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>  // Para srand() y rand()
#include <ctime>    // Para time()
#include "celda.cpp"

// Método para verificar si un número ya está en la fila, columna o subcuadro 3x3
bool canPlaceNumber(int num, int row, int col, Celda grid[3][3]) {
    int subGridRow = row / 3;
    int subGridCol = col / 3;

    // Verificar fila y columna
    for (int i = 0; i < 9; ++i) {
        if (grid[row / 3][i / 3].obtenerValor(row % 3, i % 3) == num || grid[i / 3][col / 3].obtenerValor(i % 3, col % 3) == num) {
            return false;
        }
    }

    // Verificar subcuadro 3x3
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grid[subGridRow][subGridCol].obtenerValor(i, j) == num) {
                return false;
            }
        }
    }
    return true;
}

// Método para llenar la matriz 3x3 de objetos Celda
bool fillGrid(Celda grid[3][3]) {
    std::srand(static_cast<unsigned>(std::time(0))); // Inicializar la semilla del generador de números aleatorios

    // Para cada celda en la matriz 9x9, intentar colocar números del 1 al 9
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            bool placed = false;
            std::vector<int> availableNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};

            while (!availableNumbers.empty()) {
                int index = std::rand() % availableNumbers.size();
                int num = availableNumbers[index];

                if (canPlaceNumber(num, row, col, grid)) {
                    grid[row / 3][col / 3].obtenerValor(row % 3, col % 3) = num;
                    placed = true;
                    break;
                } else {
                    // Eliminar el número de los disponibles si no se puede colocar
                    availableNumbers.erase(availableNumbers.begin() + index);
                }
            }

            if (!placed) {
                // Reiniciar la cuadrícula si no se pudo colocar un número válido
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        for (int k = 0; k < 3; ++k) {
                            for (int l = 0; l < 3; ++l) {
                                grid[i][j].valores[k][l] = 0;
                            }
                        }
                    }
                }
                row = -1; // Reiniciar la fila
                break;
            }
        }
    }
    return true;
}

bool solveGrid(Celda grid[3][3],Celda gridComplete[3][3], Celda gridToComplete[3][3], int gameMode) {
    std::srand(static_cast<unsigned>(std::time(0))); // Inicializar la semilla del generador de números aleatorios
    if(gameMode == 1){
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    for (int l = 0; l < 3; ++l) {
                        if (gridToComplete[i][j].valores[k][l] == 0){
                            grid[i][j].valores[k][l] = gridComplete[i][j].valores[k][l] ; 
                        }
                    }
                }
            }
        }
        return true;
    }

    // Para cada celda en la matriz 9x9, intentar colocar números del 1 al 9
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            bool placed = false;
            std::vector<int> availableNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};

            while (!availableNumbers.empty()) {
                int index = std::rand() % availableNumbers.size();
                int num = availableNumbers[index];
                if(grid[row / 3][col / 3].obtenerValor(row % 3, col % 3) == 0){
                    if (canPlaceNumber(num, row, col, gridComplete)) {
                        gridComplete[row / 3][col / 3].obtenerValor(row % 3, col % 3) = num;
                        placed = true;
                        break;
                    } else {
                        // Eliminar el número de los disponibles si no se puede colocar
                        availableNumbers.erase(availableNumbers.begin() + index);
                    }
                }else{
                    if (canPlaceNumber(grid[row / 3][col / 3].obtenerValor(row % 3, col % 3), row, col, gridComplete)) {
                        gridComplete[row / 3][col / 3].obtenerValor(row % 3, col % 3) = grid[row / 3][col / 3].obtenerValor(row % 3, col % 3);
                        placed = true;
                        break;
                    }else{
                        break;
                    }
                }
            }

            if (!placed) {
                // Reiniciar la cuadrícula si no se pudo colocar un número válido
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        for (int k = 0; k < 3; ++k) {
                            for (int l = 0; l < 3; ++l) {
                                gridComplete[i][j].valores[k][l] = 0;
                            }
                        }
                    }
                }
                row = -1; // Reiniciar la fila
                break;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    grid[i][j].valores[k][l] = gridComplete[i][j].valores[k][l];
                }
            }
        }
    }
    return true;
}

bool ableToWriteon(int row, int col, Celda grid[3][3]){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    if(i*3+k == row && j*3+l == col){
                        if(grid[i][j].valores[k][l] == 0){
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
            }
        }
    }
}

bool checkWin(Celda grid[3][3],Celda gridComplete[3][3]){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    if(grid[i][j].valores[k][l] != gridComplete[i][j].valores[k][l]){
                        return false;                            
                    }
                }
            }
        }
    }
    return true;
}
int main() {
    const int gridSize = 9;      // Tamaño total de la cuadrícula 9x9
    const int cellSize = 50;     // Tamaño de cada celda en la pantalla
    const int menuWidth = 200;   // Ancho del área del menú
    const int windowWidth = gridSize * cellSize + menuWidth; // Ancho de la ventana (cuadrícula + menú)
    const int windowHeight = gridSize * cellSize; // Alto de la ventana

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "sudoku");
    Celda grid[3][3];  // Matriz de 3x3 de objetos Celda
    Celda gridComplete[3][3];
    Celda gridToSolve[3][3]; // Matriz de 3x3 de objetos Celda para resolver
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> numbersAux;
    bool solved = false;
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
    // Configurar mensaje de resuelto
    sf::Text solvedText("Resuelto", font, 30);
    solvedText.setFillColor(sf::Color::Green);
    sf::Text unsolvedText("Sin Resolver", font, 30);
    unsolvedText.setFillColor(sf::Color::Red);

    // Configurar variable para controlar el modo de juego
    int gameMode = 0;
    // Inicializar la semilla del generador de números aleatorios
    std::srand(static_cast<unsigned>(std::time(0)));
    int random = 0;

    // Posicionar los textos del menú a la derecha de la cuadrícula
    newGameText.setPosition(gridSize * cellSize + 10, 50);
    loadGameText.setPosition(gridSize * cellSize + 10, 100);
    solveText.setPosition(gridSize * cellSize + 10, 150);
    solvedText.setPosition(gridSize * cellSize + 10, 250);
    unsolvedText.setPosition(gridSize * cellSize + 10, 250);

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
    sf::RectangleShape cellsGroups[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cellsGroups[i][j].setSize(sf::Vector2f((cellSize - 1)*3, (cellSize - 1)*3)); // Tamaño de la celda
            cellsGroups[i][j].setPosition(j * cellSize * 3, i * cellSize * 3);           // Posición en la cuadrícula
            cellsGroups[i][j].setFillColor(sf::Color::Transparent);
            cellsGroups[i][j].setOutlineThickness(2);
            cellsGroups[i][j].setOutlineColor(sf::Color::Magenta);
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
                            if (gameMode == 1 && ableToWriteon( row, col, gridToSolve)) {
                                selectedRow = row;
                                selectedCol = col;
                            }else if(gameMode == 2 && ableToWriteon( row, col, gridToSolve)){
                                selectedRow = row;
                                selectedCol = col;
                            }
                        }
                    }

                    // Verificar si el clic es en los botones del menú
                    else if (x >= gridSize * cellSize) {
                        if (newGameText.getGlobalBounds().contains(x, y)) {
                            solved = false;
                            gameMode = 1;  // Habilitar edición para "Nuevo Juego"
                            selectedRow = selectedCol = -1; // Des-seleccionar cualquier celda
                            // Lógica para "Nuevo Juego": resetear la cuadrícula
                            fillGrid(gridComplete);
                            for (int i = 0; i < 3; ++i) {
                                for (int j = 0; j < 3; ++j) {
                                    for (int k = 0; k < 3; ++k) {
                                        for (int l = 0; l < 3; ++l) {
                                           grid[i][j].valores[k][l] = 0; 
                                           gridToSolve[i][j].valores[k][l] = 0;
                                        }
                                    }
                                }
                            }
                            bool failToCreate = true;
                            for (int i = 0; i < 3; ++i) {
                                for (int j = 0; j < 3; ++j) {
                                    for (int k = 0; k < 3; ++k) {
                                        for (int l = 0; l < 3; ++l) {
                                            random = std::rand() % 100;
                                            if(random > 10){
                                                gridToSolve[i][j].valores[k][l] = 1;
                                                grid[i][j].valores[k][l] = gridComplete[i][j].valores[k][l];
                                            }else{
                                                failToCreate = false;
                                                gridToSolve[i][j].valores[k][l] = 0;
                                            }
                                        }
                                    }
                                }
                                if(i == 2 && failToCreate){
                                    i=-1;
                                    for (int m = 0; m < 3; ++m) {
                                        for (int j = 0; j < 3; ++j) {
                                            for (int k = 0; k < 3; ++k) {
                                                for (int l = 0; l < 3; ++l) {
                                                grid[m][j].valores[k][l] = 0; 
                                                gridToSolve[m][j].valores[k][l] = 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (loadGameText.getGlobalBounds().contains(x, y)) {
                            // Lógica para "Cargar"
                            solved = false;
                            gameMode = 2;  // 
                            selectedRow = selectedCol = -1;
                            for (int i = 0; i < 3; ++i) {
                                for (int j = 0; j < 3; ++j) {
                                    for (int k = 0; k < 3; ++k) {
                                        for (int l = 0; l < 3; ++l) {
                                           grid[i][j].valores[k][l] = 0; 
                                           gridToSolve[i][j].valores[k][l] = 0;
                                        }
                                    }
                                }
                            }
                        } else if (solveText.getGlobalBounds().contains(x, y)) {
                            solved = false;
                            if (gameMode != 3){
                                solveGrid(grid, gridComplete, gridToSolve, gameMode);
                            }
                            gameMode = 3;
                            selectedRow = selectedCol = -1;
                            checkWin(grid, gridComplete) ? solved=true : solved =false;
                            std::cout << "Solucionar presionado" << std::endl;
                        }
                    }
                }
            }

            // Detectar entrada de texto en la celda seleccionada
            if (event.type == sf::Event::TextEntered) {
                if ((gameMode == 1 || gameMode == 2) && selectedRow != -1 && selectedCol != -1) {
                    char inputChar = static_cast<char>(event.text.unicode);
                    if (inputChar >= '1' && inputChar <= '9') {
                        int subGridRow = selectedRow / 3;
                        int subGridCol = selectedCol / 3;
                        int cellRow = selectedRow % 3;
                        int cellCol = selectedCol % 3;
                        if(gameMode ==1 || (gameMode==2 && canPlaceNumber(inputChar - '0', selectedRow, selectedCol, grid))){
                            grid[subGridRow][subGridCol].obtenerValor(cellRow, cellCol) = inputChar - '0'; // Guardar valor
                        }
                        checkWin(grid, gridComplete) ? solved=true : solved =false;
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
                } else if(ableToWriteon(i,j,gridToSolve)){
                    cells[i][j].setFillColor(sf::Color::Yellow); 
                }else {
                    cells[i][j].setFillColor(sf::Color::White); // Blanco para celdas no seleccionadas
                }
                int xGroup = i/3;
                int yGroup = j/3;
                window.draw(cells[i][j]);
                window.draw(cellsGroups[xGroup][yGroup]);

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
        if(solved){
            window.draw(solvedText);
        }else{
            window.draw(unsolvedText);
        }
        window.setFramerateLimit(20);
        window.display();
    }

    return 0;
}
