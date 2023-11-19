#include <vector>

using namespace std;

struct Position {
	int x;
	int y;
	int r;
};

struct Rectangle {
	int id;
	int width;
	int height;
	Position position;
};

class Strip {
private:
	int fixedWidth;
	vector<Rectangle> rectangles;
	vector<Position> emptySpaces;

public:
	explicit Strip(int w) : fixedWidth(w) {}
	~Strip() = default;
	int getHeight(vector<Rectangle> elements);
	int evaluationFunction(vector<Rectangle> elements);
	void addRectangleGreedy(Rectangle rectangle);
	bool canBePlaced(Rectangle rectangle, int posX, int posY, int orientation, vector<Rectangle> elements);
	void hillClimbingFirstImprovement();
	void calculateEmptySpaces();
	vector<Rectangle> sortRectangles();
	void printOutput();
};
