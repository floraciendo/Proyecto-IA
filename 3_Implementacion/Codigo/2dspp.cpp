#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <chrono>

#include "2dspp.hpp"

bool Strip::canBePlaced(Rectangle rectangle, int posX, int posY, int orientation, vector<Rectangle> elements) {
	for (const Rectangle &r : elements) {
		// Revisa las 4 ubicaciones posibles
		bool left = posX + rectangle.width + orientation * (rectangle.height - rectangle.width) <= r.position.x;
		bool right = r.position.x + r.width + r.position.r * (r.height - r.width) <= posX;
		bool up = posY + rectangle.height + orientation * (rectangle.width -  rectangle.height) <= r.position.y;
		bool down = r.position.y + r.height + r.position.r * (r.width - r.height) <= posY;

		// Si ninguna se cumple es porque se solapa 
		if (!(left || right || up || down)) {
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
			if (y < bestY && canBePlaced(rectangle, x, y, 1, rectangles)) {
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
			if (y < bestY && canBePlaced(rectangle, x, y, 0, rectangles)) {
				bestY = y;
				bestX = x;
			}
		}
		// Agrega el rectangulo
		rectangle.position = {bestX, bestY, 0};
		rectangles.push_back(rectangle);
	}
}

// Revisa todos los espacios vacíos, debería ser igual al área inutilizada
void Strip::calculateEmptySpaces() {
	emptySpaces.reserve(evaluationFunction(rectangles));

	// Se usara un rectángulo de 1*1 para verificar que posiciones estan vacías
	Rectangle space = {0, 1, 1};
	int currentHeight = getHeight(rectangles);

	for (int x = 0; x < fixedWidth; x++) {
		for (int y = 0; y < currentHeight; y++) {
			// Si se puede colocar el rectángulo significa que es un espacio vacío
			if (canBePlaced(space, x, y, 1, rectangles)) {
				emptySpaces.push_back({x, y});
			}
		}
	}
}

// Implementa HC-FI para mejorar la solución
void Strip::hillClimbingFirstImprovement() {
	// Obtiene los posibles espacios en los que se puede agregar el rectángulo
	calculateEmptySpaces();

	vector<Rectangle> improvedRectangles = rectangles;
	Rectangle currentRectangle;
	int currentSolution = evaluationFunction(rectangles);
	int improvedSolution;
	
	// Para revisar si la solución es un óptimo 
	bool local = false;
	bool improvement = false;

	// Revisa con cada rectángulo, partiendo de los últimos posicionados
	int counter = rectangles.size() - 1;

	// Mientras no se encuentre en un optimo local, o global
	while(!(local) && (counter >= 0)) {	
		improvement = false;

		// Reviso cada una de las posiciones posibles
		for (int i = 0; i < emptySpaces.size(); i++) {
			// El rectángulo que se va a reposicionar
			improvedRectangles.erase(improvedRectangles.begin() + counter);

			// Revisa si se puede posicionar y que sea una posición factible
			if (canBePlaced(rectangles[counter], emptySpaces[i].x, emptySpaces[i].y, 0, improvedRectangles) &&
				(emptySpaces[i].x + rectangles[counter].width) <= fixedWidth) {
				// Agrega el rectángulo en esa posición
				currentRectangle = rectangles[counter];
				currentRectangle.position = {emptySpaces[i].x, emptySpaces[i].y, 0};
				improvedRectangles.push_back(currentRectangle);
				
				// Revisa si la solución encontrada es mejor
				improvedSolution = evaluationFunction(improvedRectangles);
				if (improvedSolution < currentSolution) {
					// Reemplaza la solución
					rectangles[counter] = currentRectangle;
					currentSolution = improvedSolution;
					improvement = true;
				}
			} else if (canBePlaced(rectangles[counter], emptySpaces[i].x, emptySpaces[i].y, 1, improvedRectangles) &&
				(emptySpaces[i].x + rectangles[counter].height) <= fixedWidth) {
				// Agrega el rectángulo en esa posición
				currentRectangle = rectangles[counter];
				currentRectangle.position = {emptySpaces[i].x, emptySpaces[i].y, 1};
				improvedRectangles.push_back(currentRectangle);

				// Revisa si la solución encontrada es mejor
				improvedSolution = evaluationFunction(improvedRectangles);
				if (improvedSolution < currentSolution) {
					cout << "Rectángulo n°" << rectangles[counter].id << " altura encontrada "<< getHeight(improvedRectangles) << " altura anterior " << getHeight(rectangles) << endl;
					cout << "Función de evaluación " << improvedSolution << endl;
					// Reemplaza la solución
					rectangles[counter] = currentRectangle;
					currentSolution = improvedSolution;
					improvement = true;
				}
			}
			// Actualiza los rectángulos
			improvedRectangles = rectangles;

			// Si encuentra una mejor solución deja de buscar en esta iteración
			if (improvement) {
				cout << "Se mejoró la posición del rectángulo n°" << rectangles[counter].id << endl;
				break;
			}
		}
		// Si no hay mejoras significa que es un óptimo
		if (!(improvement)) {
			cout << "Se encontró un óptimo local revisando el rectángulo n°" << rectangles[counter].id << endl;
			local = true;
		}
		// Sigue usando el rectángulo anterior y actualiza los espacios vacíos
		counter--;
		emptySpaces.clear();
		calculateEmptySpaces();
	}
}

int Strip::getHeight(vector<Rectangle> elements) {
	int maxHeight = 0;

	// Revisa las posiciones de todos los rectángulos
	for (const Rectangle &element : elements) {
		maxHeight = max(maxHeight, element.position.y + element.height + element.position.r * (element.width - element.height));
	}

	return maxHeight;
}

int Strip::evaluationFunction(vector<Rectangle> elements) {
	// Para luego poder calcular el área sin usar
	int totalArea = getHeight(elements) * fixedWidth;
	int totalRectangleArea = 0;

	// Suma el área total de los rectángulos
	for (const Rectangle &element : elements) {
		totalRectangleArea += element.width * element.height;
	}

	return totalArea - totalRectangleArea;
}

vector<Rectangle> Strip::sortRectangles() {
	// Crea un vector nuevo para no modificar el original
	vector<Rectangle> sortedRectangles = rectangles;

	// Utiliza una función lambda para oordenarlos de acuerdo a su id
	sort(sortedRectangles.begin(), sortedRectangles.end(), [](const Rectangle& a, const Rectangle& b) {
		return a.id < b.id;
	});

	return sortedRectangles;
}

void Strip::printOutput() {
	// Calcula e imprime la altura total y el área sin usar
	int currentHeight = getHeight(rectangles);
	int totalUnsedArea = evaluationFunction(rectangles);
	cout << currentHeight << "\n" << totalUnsedArea << endl;

	// Obtiene los rectangulos ordenados por su id
	vector<Rectangle> sortedRectangles = sortRectangles();

	int minimo = 0;

	// Imprime las posiciones e índice de rotación de cada uno de los rectángulos
	for (const Rectangle &rectangle : sortedRectangles) {
		cout << rectangle.position.x << " " << rectangle.position.y << " " << rectangle.position.r << endl;
		minimo += rectangle.width * rectangle.height;
	}
	float alt = minimo / fixedWidth;
	cout << "altura minima " << alt << endl;
}

int main(int argc, char* argv[]) {
	// Tiempo cuando parte la ejecución del programa
	auto startTime = chrono::high_resolution_clock::now();

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
	rectangles.reserve(n);

	for (int i = 0; i < n; i++) {
		Rectangle rectangle;
		input >> rectangle.id >> rectangle.width >> rectangle.height;
		rectangles.push_back(rectangle);
	}

	// Cierra el archivo
	input.close();

	// Se genera el orden aleatorio en base a la semilla
	shuffle(rectangles.begin(), rectangles.end(), default_random_engine(seed));

	// Obtiene la solución inicial
	for (const Rectangle &r : rectangles) {
		cout << "Agregando el rectángulo n°" << r.id << endl;
		strip.addRectangleGreedy(r);
	}

	strip.printOutput();

	// Mejora la solución, si se puede, con HC-FI
	strip.hillClimbingFirstImprovement();

	// Muestra la solución encontrada
	strip.printOutput();

	auto stopTime = chrono::high_resolution_clock::now();
	// Calcula el tiempo final de ejecución
	chrono::duration<double> executionTime = stopTime - startTime;
	cout << "Tiempo de ejecución: " << executionTime.count() << endl;

	return 0;
}
