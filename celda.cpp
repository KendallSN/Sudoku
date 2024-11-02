// Clase Celda que contiene una matriz de 3x3 enteros
class Celda {
public:
    int valores[3][3]; // Matriz de enteros de 3x3

    Celda() {
        // Inicializamos todos los valores en 0
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                valores[i][j] = 0;
            }
        }
    }

    int& obtenerValor(int fila, int columna) {
        return valores[fila][columna];
    }
};