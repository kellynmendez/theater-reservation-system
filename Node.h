/** Node class header file
 */

#ifndef NODE_H
#define NODE_H

template<typename T>
class Node
{
private:
    Node<T> *up;
    Node<T> *down;
    Node<T> *right;
    Node<T> *left;
    T payload;

public:
    // Constructors
    Node() {}
    Node(T value);

    // Accessors
    Node<T>* getUp() {return up;}
    Node<T>* getDown() {return down;}
    Node<T>* getRight() {return right;}
    Node<T>* getLeft() {return left;}
    T getValue() {return payload;}

    // Mutators
    void setUp(Node<T>* u) {up = u;}
    void setDown(Node<T>* d) {down = d;}
    void setRight(Node<T>* r) {right = r;}
    void setLeft(Node<T>* l) {left = l;}
    void setValue(T p) {payload = p;}
};

template<typename T>
Node<T>::Node(T value)
{
    up = nullptr;
    down = nullptr;
    left = nullptr;
    right = nullptr;
    payload = value;
}

#endif //NODE_H
