#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cstdlib>
// #include <chrono>

#include "2dspp.hpp"

bool Strip::canBePlaced(Rectangle rectangle, int posX, int posY, int orientation) {
	for (const Rectangle &r : rectangles) {
		// Revisa por el eje x
		bool overlapX = (posX + rectangle.width + orientation * (rectangle.height - rectangle.width) <= r.position.x) || 
						(r.position.x + r.width + r.position.r * (r.height - r.width) > posX);
		// Revisa por el eje y
		bool overlapY = (posY + rectangle.height + orientation * (rectangle.width - rectangle.height) <= r.position.y) || 
						(r.position.y + r.height + r.position.r * (r.width - r.height) > posY);
		
		if (!(overlapX || overlapY)) {
			return false;
		}
	}
	return true;
}

// Siguiendo la lógica de Bottom Left
void Strip::addRectangleGreedy(Rectangle rectangle) {
	int bestY = INT32_MAX;
	int bestX = 0;

	// Intenta agregar la menor altura posible
	if (rectangle.width < rectangle.height && rectangle.height <= fixedWidth) {
		// Lo puede colocar rotado
		for (int x = 0; x <= fixedWidth - rectangle.height; x++) {
			int y = 0;

			for (const Rectangle &r : rectangles) {
				if (x < r.position.x + r.width + r.position.r * (r.height - r.width) && x + rectangle.height > r.position.x && rectangle.height <= fixedWidth - x) {
					y = max(y, r.position.y + r.height + r.position.r * (r.width - r.height));
				}
			}

			// Revisa si encontró alguna posición mejor
			if (y < bestY && canBePlaced(rectangle, x, y, 1)) {
				bestY = y;
				bestX = x;
			}
		}

		// Agrega el rectangulo
		rectangle.position = {bestX, bestY, 1};
		rectangles.push_back(rectangle);
	} else {
		// Asumiendo que todas las instancias van a ser factibles, lo coloca sin rotar
		for (int x = 0; x <= fixedWidth - rectangle.width; x++) {
			int y = 0;

			for (const Rectangle &r : rectangles) {
				if (x < r.position.x + r.width + r.position.r * (r.height - r.width) && x + rectangle.width > r.position.x && rectangle.width <= fixedWidth - x) {
					y = max(y, r.position.y + r.height + r.position.r * (r.width - r.height));
				}
			}

			// Revisa si encontró alguna posición mejor
			if (y < bestY && canBePlaced(rectangle, x, y, 0)) {
				bestY = y;
				bestX = x;
			}
		}

		// Agrega el rectangulo
		rectangle.position = {bestX, bestY, 0};
		rectangles.push_back(rectangle);
	}
}

int Strip::getHeight() {
	int maxHeight = 0;

	// Revisa las posiciones de todos los rectángulos
	for (const Rectangle &rectangle : rectangles) {
		maxHeight = max(maxHeight, rectangle.position.y + rectangle.height + rectangle.position.r * (rectangle.width - rectangle.height));
	}

	return maxHeight;
}

int Strip::evaluationFunction() {
	// Para luego poder calcular el área sin usar
	int totalArea = getHeight() * fixedWidth;
	int totalRectangleArea = 0;

	// Suma el área total de los rectángulos
	for (const Rectangle &rectangle : rectangles) {
		totalRectangleArea += rectangle.width * rectangle.height;
	}

	return totalArea - totalRectangleArea;
}

void Strip::printOutput() {
	// Calcula e imprime la altura total y el área sin usar
	int currentHeight = getHeight();
	int totalUnsedArea = evaluationFunction();
	cout << currentHeight << "\n" << totalUnsedArea << endl;

	// Imprime las posiciones e índice de rotación de cada uno de los rectángulos
	for (const Rectangle &rectangle : rectangles) {
		cout << rectangle.position.x << " " << rectangle.position.y << " " << rectangle.position.r << endl;
	}
}

int main(int argc, char* argv[]) {
	// Tiempo cuando parte la ejecución del programa
	// auto startTime = chrono::high_resolution_clock::now();

	// Nombre de la instancia y semilla
	string instance = argv[1];
	int seed = atoi(argv[2]);

	// Lectura del archivo
	ifstream input(instance);
	
	// En caso que ocurra algún error
	if (!input.is_open()) {
		cerr << "Ocurrió un error intentando leer la instancia." << endl;
		return 1;
	}
	
	// Abrió la instancia correctamente
	int n, w;
	input >> n >> w;
	
	// Crea la región con el ancho dado
	Strip strip(w);

	// Crea un vector y agregar todos los rectángulos
	vector<Rectangle> rectangles;

	for (int i = 0; i < n; i++) {
		Rectangle rectangle;
		input >> rectangle.id >> rectangle.width >> rectangle.height;
		rectangles.push_back(rectangle);
	}

	// Cierra el archivo
	input.close();

	// Agrega los rectángulos en un orden aleatorio generado por la semilla
	shuffle(rectangles.begin(), rectangles.end(), default_random_engine(seed));

	for (const Rectangle &r : rectangles) {
		strip.addRectangleGreedy(r);
	}

	// Muestra la solución encontrada
	strip.printOutput();

	// auto stopTime = chrono::high_resolution_clock::now();
	// // Calcula el tiempo final de ejecución
	// chrono::duration<double> executionTime = stopTime - startTime;
	// cout << "Tiempo de ejecución: " << executionTime.count() << endl;

	return 0;
}
