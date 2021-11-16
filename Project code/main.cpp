#include <stdio.h>
#include<stdlib.h>
#include "fibonacci.hpp"
#define no_testcases 15


class TestFibonacciHeap : public FibonacciHeap<int> {
public:
	void dump() {
		printf("digraph G {\n");
		if(heap==NULL) {
			printf("empty;\n}\n");
			return;
		}
		printf("minimum -> \"%d\"\n",heap->getValue());
		Node<int>* c=heap;
		do {
			_dumpChildren(c);
			c=c->getNext();
		} while(c!=heap);
		printf("}\n");
	}

private:

	void _dumpChildren(Node<int>* n) {
		printf("\"%d\" -> \"%d\" [next];\n",n->getValue(),n->getNext()->getValue());
		printf("\"%d\" -> \"%d\" [prev];\n",n->getValue(),n->getPrev()->getValue());
		if(n->getLoser()>0)printf("\"%d\" [marked];\n",n->getValue());
		if(n->getParent()) {
			printf("\"%d\" -> \"%d\" [parent];\n",n->getValue(),n->getParent()->getValue());
		}
		if(n->getChild()) {
			Node<int>* c=n->getChild();
			do {
				printf("\"%d\" -> \"%d\" [child];\n",n->getValue(),c->getValue());
				_dumpChildren(c);
				c=c->getNext();
			} while(c!=n->getChild());
		}
	}
};


void test() {
	TestFibonacciHeap h;
	for(int i=0; i<no_testcases/3; i++){
		h.insert(rand()%90);
	}
	Node<int>* nine=h.insert(90);
	for(int i=0; i<no_testcases/9; i++){
		printf("Popped \"%d\"\n",h.popMinimum());
	}
	for(int i=0; i<no_testcases/3; i++){
		h.insert(rand()%90);
	}
	for(int i=0; i<no_testcases/9; i++){
		printf("Popped \"%d\"\n",h.popMinimum());
	}
	for(int i=0; i<no_testcases/3; i++){
		h.insert(rand()%90);
	}
	printf("------------------------------------------------------------------------\n");
	h.decreaseKey(nine,1);
	h.dump();
}

int main() {
	test();
}