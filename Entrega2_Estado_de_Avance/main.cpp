#include <fstream>
#include <iostream>
#include <string>

#include "main.hpp"

// Revisar bien la sobreposición
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
			if (y < bestY) {
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
			if (y < bestY) {
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
	for (const Rectangle &rectangle : rectangles) {
		maxHeight = max(maxHeight, rectangle.position.y + rectangle.height + rectangle.position.r * (rectangle.width - rectangle.height));
	}
	return maxHeight;
}

int Strip::evaluationFunction() {
	int totalArea = getHeight() * fixedWidth;
	int totalRectangleArea = 0;
	for (const Rectangle &rectangle : rectangles) {
		totalRectangleArea += rectangle.width * rectangle.height;
	}
	return totalArea - totalRectangleArea;
}

void Strip::printOutput() {
	int currentHeight = getHeight();
	int totalUnsedArea = evaluationFunction();
	cout << currentHeight << "\n" << totalUnsedArea << endl;
	for (const Rectangle &rectangle : rectangles) {
		cout << rectangle.position.x << " " << rectangle.position.y << " " << rectangle.position.r << endl;
	}
}

int main() {
	// Nombre del archivo de la instancia
	string instance = "beng01.txt";
	ifstream input(instance);
	
	// En caso que ocurra algún error
	if (!input.is_open()) {
		cerr << "Ocurrió un error." << endl;
		return 1;
	}
	
	// Abrió la instancia correctamente
	int n, w;
	// Obtiene la cantidad de bloques y el ancho fijo
	input >> n >> w;
	Strip strip(w);
	// Agrega todos los bloques a la región
	for (int i = 0; i < n; i++) {
		Rectangle rectangle;
		input >> rectangle.id >> rectangle.width >> rectangle.height;
		strip.addRectangleGreedy(rectangle);
	}
	// Cierra el archivo
	input.close();
	strip.printOutput();
	return 0;
}
