-------- Description ---------

A Fibonacci Heap is a collection of heap-ordered trees. It is a data structure used to implement priority queue operations, as it has a much better amortized running time than all other heaps, including binary and binomial heaps.

-------- Run Software ---------

The implementation file consisting of all the implementation code and functions with respect to Fibonacci heaps has been provided as a header file that is included in the client code as follows:
#include “fibb_heap.h”

Once this is included, clients can create Fibonacci heaps and call functions to perform different operations as explained above or in the readme file associated with this document using the provided interface.

The client file is then compiled using the g++ utility.
g++ main.cpp -o exec

The generated executable is then loaded and executed.
./exec

-------- Design ---------

Fibonacci Heap Class:

Protected Types: Pointer to the root of the heap.

Public Functions: 
	Default Constructor: Default NULL Heap
	Copy Constructor: Copies the entire Heap
	Default Destructor: Deletes all Nodes, entire Heap
	insert()
	merge()
	peekMinimum()
	popMinimum()
	decreaseKey()

Private Functions:
	_copyAll() (used by copy constructor)
	_merge()
	_deleteAll()
	_addChild()
	_disown()
	_popMinimum()
	_cut()
	_decreaseKey()
	

Node Class:

Private Data Types:
	generic value
	degree (depth)
	loser (marked node)
	pointer to prev node
	pointer to next node
	pointer to parent node
	pointer to child node
	declaration of friend class FibonaciHeap

Public Data:
	parameterised constructor
	copy constructor
	getPrev()
	getNext()
	getChild()
	getParent()
	getValue()
	getLoser()
	getDegree()
	setPrev()
	setNext()
	setParent()
	setChild()
	setValue()
	setLoser()
	setDegree()

-------------------------
