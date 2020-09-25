#include <iostream>
class SetValue {
private:
	int x, y = 0;

public:
	SetValue(int a, int b) {
		x = a;
		y = b;
	}
	int return_x() {
		return x;
	}
	int return_y() {
		return y;
	}
};

int main() {
	SetValue obj(33, 44);

	std::cout << "X = " << obj.return_x() << " ,Y = " << obj.return_y() << std::endl;
	return 0;
}