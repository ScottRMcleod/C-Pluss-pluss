#ifndef NODE_H
#define NODE_H

#include <list>
using std::list;
class Node
{
public:
	Node();

	virtual ~Node();

	void setParent(Node *parent);
	Node *getParent(void);
	
	list<Node *> &getChildren(void);
	
	static list<Node *> &getNodes(void);
private:
	static list<Node *> nodes;
protected:
	list<Node *> children;
	Node *parent;

};
#endif