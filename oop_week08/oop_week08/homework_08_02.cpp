#include <iostream>

class Strategy {
public:
	virtual int doOperation(int num1, int num2) = 0;
};

class OperationAdd : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubstract : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 - num2;
	}
};

class OperationMultiply : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};

class Context {
private:
	Strategy* strategy;

public:
	Context(Strategy* strategy) {
		this->strategy = strategy;
	}
	int executeStrategy(int num1, int num2) {
		return strategy->doOperation(num1, num2);
	}
};

void main() {
	OperationAdd* a = new OperationAdd();
	Context* context = new Context(a);
	std::cout << "10 + 5 = " << context->executeStrategy(10, 5) << std::endl;
	OperationSubstract* s = new OperationSubstract();
	context = new Context(s);
	std::cout << "10 - 5 = " << context->executeStrategy(10, 5) << std::endl;
	OperationMultiply* m = new OperationMultiply();
	context = new Context(m);
	std::cout << "10 * 5 = " << context->executeStrategy(10, 5) << std::endl;
}
