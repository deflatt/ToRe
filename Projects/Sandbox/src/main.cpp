#include <iostream>

import TR.Essentials.Array;
import TR.Essentials.Procedure;
using namespace TR;

struct Base {
	int b = 3;
	virtual ~Base() {}
};

struct Test : public Base {
	int t = 5;
};

int main() {

	ProcedureSet<Base*> set = {};
	
	Procedure<Test*> proc = [](Test* t) {
		std::cout << t->t << std::endl;
	};
	set.Insert(&proc);

	Test t;
	t.t = 7;
	Base* b = &t;
	set(b);

	return 0;
}