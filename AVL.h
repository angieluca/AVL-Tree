#ifndef PROJECT1_AVL_H
#define PROJECT1_AVL_H
#include <iostream>
#include <vector>

struct Node{
    std::string name;
    std::string ufid;
    Node* left;
    Node* right;

    //Parameterized constructor w/o children
    Node(std::string &name, std::string &ufid) : name(name), ufid(ufid), left(nullptr), right(nullptr) {}
    //Parameterized constructor with left and right children
    Node(std::string &name, std::string &ufid, Node *left, Node *right) : name(name), ufid(ufid), left(left), right(right) {}

};

class AVLTree{
private:

    Node* root = nullptr;
    Node* insertHelper(Node* node, std::string &name, std::string &ufid);
    Node* balanceTree(Node* rootNode); //balance tree after every insertion
    int calculateHeight(Node* rootNode);
    int calculateBF(Node* rootNode);

    //Rotation Functions
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* rotateLeftRight(Node* root);
    Node* rotateRightLeft(Node* root);

    Node* deletePostOrder(Node* rootNode);
    Node* vectorsPreOrder(Node* rootNode, std::vector<std::string> &namesVec, std::vector<std::string> &idVec);
    Node* vectorsInOrder(Node *rootNode, std::vector<std::string> &namesVec, std::vector<std::string> &idVec);

    Node* findNode(Node* rootNode, std::string& id);
    Node* findParent(Node* rootNode, Node* childNode);
    Node* findSuccessor(Node* rootNode);

public:

    AVLTree() = default; //Default constructor
    ~AVLTree() {deletePostOrder(root);} //Destructor (postOrder since each node gets visited once)

    Node* getRoot(){ return root; }

    bool insert(std::string &name, std::string &ufid);
    bool remove(std::string& id);
    bool searchID(std::string& id);
    bool searchName(std::string& name);

    std::string printPreOrder(Node* rootNode, std::string &fullList);
    std::string printInOrder(Node* rootNode, std::string &fullList);
    std::string printInOrderIDS(Node* rootNode, std::string &fullList);
    std::string printPostOrder(Node* rootNode, std::string &fullList);
    bool printLevelCount();
    bool removeInOrder(int N);

};

#endif //PROJECT1_AVL_H