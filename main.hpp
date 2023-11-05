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

public:
	explicit Strip(int w) : fixedWidth(w) {}
	~Strip() = default;
	int getHeight();
	int evaluationFunction();
	void addRectangleGreedy(Rectangle rectangle);
	void printOutput();
};
