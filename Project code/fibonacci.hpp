template <class V> class FibonacciHeap;

//Nodes of heap
template <class V> class Node{
	private:
	V value;
	int degree;
	int loser; //flag - for the maximally damaged tree constraint

	Node<V>* prev; //left sibling
	Node<V>* next; //right sibling
	Node<V>* parent;
	Node<V>* child;
	friend class FibonacciHeap<V>;

	public:
	Node(V _value){ //constructor
		value=_value;
		prev=this;
		next=this;
		degree=0;
		loser=0;
		child=nullptr;
		parent=nullptr;
	}
	Node(const Node<V> &Node){ //copy constructor
		value=Node.value;
		degree=Node.degree;
		loser=Node.loser;
		prev=Node.prev;
		next=Node.next;
		parent=Node.parent;
		child=Node.child;
	}
	Node& operator=(Node Node){ //copy assignment operator
		value=Node.value;
		degree=Node.degree;
		loser=Node.loser;
		prev=Node.prev;
		next=Node.next;
		parent=Node.parent;
		child=Node.child;
	}

	//getter functions
	Node<V>* getPrev(){
		return prev;
	}
	Node<V>* getNext(){
		return next;
	}
	Node<V>* getChild(){
		return child;
	}
	Node<V>* getParent(){
		return parent;
	}
	V getValue(){
		return value;
	}
	int getLoser(){
		return loser;
	}
	int getDegree(){
		return degree;
	}

	//setter functions
	void setPrev(Node<V>* _prev){
		prev=_prev;
	}
	void setNext(Node<V>* _next){
		next=_next;
	}
	void setParent(Node<V>* _parent){
		parent=_parent;
	}
	void setChild(Node<V>* _child){
		child=_child;
	}
	void setValue(V _value){
		value=_value;
	}
	void setLoser(int _loser){
		loser=_loser;
	}
	void setDegree(int _degree){
		degree=_degree;
	}
};

template <class V> class FibonacciHeap{ 
protected:
	Node<V>* heap;
public: //interfaces
	FibonacciHeap(){ //constructor
		heap=nullptr;  //initialize
	}
	/*FibonacciHeap(const FibonacciHeap<V> &_heap){ //copy constructor
		//heap=_copyAll(_heap);
		heap=_heap;
	}*/
	virtual ~FibonacciHeap(){ //destructor
		_deleteAll(heap);
	}
	FibonacciHeap& operator=(FibonacciHeap other){ //copy assignment operator
		heap=other.heap;
	}

	Node<V>* insert(V value){
		Node<V> *n=new Node<V>(value); //make Node with value
		heap=_merge(heap,n); //merge Node with heap
		return n;
	}
	void merge(FibonacciHeap& other){
		heap=_merge(heap,other.heap); //merge heaps (put all Nodes into original heap)
		other.heap=nullptr; //yeet the other heap
	}
	bool isEmpty(){
		return heap==nullptr?true:false; //check if heap empty
	}
	V peekMinimum(){
		return heap->getValue(); //getMin
	}

	V popMinimum(){ 
		Node<V>* old=heap;
		heap=_popMinimum(heap); //extractMin
		V n=old->getValue();
		delete old;
		return n;
	}
	void decreaseKey(Node<V>* n,V value){
		heap=_decreaseKey(heap,n,value);
	}

private:
	/*Node<V>* _copyAll(Node<V>* head){ 
		if(head!=nullptr){ //not empty heap

			//copy head
			Node<V> head_copy = *head;
			Node<V> *temp = &head_copy;

			//copy children
			do{
				temp->child=_copyAll(temp->child);
				temp->child->parent=temp;
				Node<V> n=*(temp->next);
				temp->next=&n;
				temp->next->prev=temp;
				temp=temp->next;
			} while(temp!=head); //go to next root 
			return &head_copy;
		}
		return nullptr;
	}*/

	Node<V>* _merge(Node<V>* a,Node<V>* b){
		if(a!=nullptr && b!=nullptr){ //if neither heap empty
			if(b->getValue() < a->getValue()){ //swap a and b if a value is greater than b value - make sure min value is in the right place
				Node<V>* temp=a;
				a=b;
				b=temp;
			}
			//take care of sibling pointers
			Node<V>* a_next=a->next;
			Node<V>* b_prev=b->prev;
			a->next=b;
			b->prev=a;
			a_next->prev=b_prev;
			b_prev->next=a_next;
			return a;
		}
		else{

			return a==nullptr?b:a; //if one heap empty, return other one
		}
	}
	
	void _deleteAll(Node<V>* n){
		if(n!=nullptr){ //traverse
			Node<V>* c=n;
			do{
				Node<V>* d=c;
				c=c->next;
				_deleteAll(d->child); //recursively delete
				delete d;
			} while(c!=n);
		}
	}
	
	void _addChild(Node<V>* parent,Node<V>* child){
		child->prev=child->next=child; //no siblings
		child->parent=parent; //assign parent
		parent->degree++; //increase degree of parent
		parent->child=_merge(parent->child,child); //put into heap?
	}

	void _disown(Node<V>* n){ //helper function - disconnect parent from children, unmark (flags are reset)
		if(n){
			Node<V>* c=n;
			do{
				c->parent=nullptr;
				c->loser=0;
				c=c->next;
			}while(c!=n);
		}
	}

	Node<V>* _popMinimum(Node<V>* n){
		_disown(n->child); //chop off the parent
		if(n->next==n){ //single node
			n=n->child;
		} else{ //add children to root list
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n=_merge(n->next,n->child);
		}
		if(n){
			Node<V>* trees[128]={nullptr}; //buffer
			while(true){ //consolidate tree (using buffer)
				if(trees[n->degree]==nullptr){ //if tree of that degree doesn't already exist
					trees[n->degree]=n; //connect root node to degree-th position of buffer
				}
				else{ //tree of that degree exists
					Node<V>* t=trees[n->degree]; //node at degree-th position of buffer
					if(t==n){ //if back at beginning, break (must be done as rootlist is circular DLL)
						break;
					}
					else{
						//make one root child of the other root
						trees[n->degree]=nullptr;
						if(n->value>t->value){
							t->prev->next=t->next;
							t->next->prev=t->prev;
							if(n->next==n){ //single node
								t->next=t->prev=t;
							}
							else{
								n->prev->next=t;
								n->next->prev=t;
								t->next=n->next;
								t->prev=n->prev;
							}
							_addChild(t,n);
							n=t; //connect node to degree-th position of buffer 
						}
						else{
							t->prev->next=t->next;
							t->next->prev=t->prev;
							_addChild(n,t);
						}
						continue;
					}
				}
				n=n->next; //move to next node
			}
			Node<V>* min=n;
			Node<V>* start=n;
			do{
				if(n->value<min->value)min=n; //make sure we get the right min value after consolidation
				n=n->next;
			} while(n!=start);
			return min;
		}
		else{
			return nullptr;
		}
	}

	Node<V>* _cut(Node<V>* heap,Node<V>* n){
		if(n->next==n){//single Node
			n->parent->child=nullptr; //chop off from parent
		} 
		else{
			//reassign sibling pointers
			n->next->prev=n->prev;
			n->prev->next=n->next;
			//chop off child  
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->loser=0;
		return _merge(heap,n);
	}

	Node<V>* _decreaseKey(Node<V>* heap,Node<V>* n,V value){
		if(n->getValue()<value)return heap; //if trying to increase key, do nothing
		n->value=value; //assign value
		if(n->parent){
			if(n->getValue()<n->parent->getValue()){ //if violating heap invariant
				heap=_cut(heap,n); //chop off node
				Node<V>* parent=n->parent;
				n->parent=nullptr;
				//cascading cuts (maximally damaged trees)
				while(parent!=nullptr && parent->loser){
					heap=_cut(heap,parent);
					n=parent;
					parent=n->parent;
					n->parent=nullptr;
				}
				if(parent!=nullptr && parent->parent!=nullptr)parent->loser++; //mark parent once child is chopped off
			}
		} else{
			if(n->getValue() < heap->getValue()){ //reassign minimum
				heap = n;
			}
		}
		return heap;
	}
};
