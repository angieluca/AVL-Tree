#include "AVL.h"
#include <sstream>

//Rotation Functions:
Node* AVLTree::rotateLeft(Node* rootNode)
{
    //parent balance factor = -2; child balance factor = -1
    Node* rightChild = rootNode->right;
    rootNode->right  = rightChild->left;
    rightChild->left = rootNode;
    return rightChild; //new root
}
Node* AVLTree::rotateRight(Node* rootNode)
{
    //parent balance factor = +2; child balance factor = +1
    Node* leftChild = rootNode->left;
    rootNode->left = leftChild->right;
    leftChild->right = rootNode;
    return leftChild; //new root
}
Node* AVLTree::rotateLeftRight(Node* rootNode)
{
    //parent balance factor = +2; child balance factor = -1
    rootNode->left = rotateLeft(rootNode->left);
    return rotateRight(rootNode);
}
Node* AVLTree::rotateRightLeft(Node* rootNode)
{
    //parent balance factor = -2; child balance factor = +1
    rootNode->right = rotateRight(rootNode->right);
    return rotateLeft(rootNode);
}

int AVLTree::calculateHeight(Node* rootNode)
{
    if (rootNode == nullptr) return 0;
    return 1 + std::max(calculateHeight(rootNode->left), calculateHeight(rootNode->right));
}
int AVLTree::calculateBF(Node* rootNode)
{
    int leftHeight = calculateHeight(rootNode->left);
    int rightHeight = calculateHeight(rootNode->right);
    return leftHeight - rightHeight;
}

bool AVLTree::insert(std::string &name, std::string &ufid)
{
    //My searchID prints something, but I just want bool value so Ima suppress cout
    std::streambuf* OGcoutBuffer = std::cout.rdbuf(); // original cout buffer
    std::ostringstream tempStream; // cout to null (suppress output)
    std::cout.rdbuf(tempStream.rdbuf());  // Output now goes to temp stream and not cout

    bool foundDuplicateID = searchID(ufid); //I can call searchID without "successful/unsuccessful" printing
    std::cout.rdbuf(OGcoutBuffer); // Now cout back to normal

    if (foundDuplicateID){
        std::cout << "unsuccessful\n";
        return false;
    }

    root = insertHelper(root, name, ufid);
    if (root != nullptr)
    {
        std::cout << "successful\n";
        return true;
    }
    else
    {
        std::cout << "unsuccessful\n";
        return false;
    }
}
Node* AVLTree::insertHelper(Node* rootNode, std::string &name, std::string &ufid)
{
    if (rootNode == nullptr) return new Node(name, ufid); //empty tree so inserted node becomes root
    if (std::stoi(ufid) < std::stoi(rootNode->ufid))
    {
        //add to left subtree
        rootNode->left = insertHelper(rootNode->left, name, ufid);
    }
    else if (std::stoi(ufid) > std::stoi(rootNode->ufid))
    {
        //add to right subtree
        rootNode->right = insertHelper(rootNode->right, name, ufid);
    }
    else
    {
        return nullptr; //duplicate: unsuccessful
    }

    return balanceTree(rootNode); //return the balanced tree's root
}

Node* AVLTree::balanceTree(Node* rootNode) {
    if (rootNode == nullptr) return nullptr;
    int parentBF = calculateBF(rootNode);

    //Left Rotation when: parentBF = -2 and rightChildBF = -1
    if (parentBF == -2 && calculateBF(rootNode->right) == -1)
    {
        return rotateLeft(rootNode);
    }
    //Right Rotation when: parentBF = 2 and rightChildBF = 1
    if (parentBF == 2 && calculateBF(rootNode->left) == 1)
    {
        return rotateRight(rootNode);
    }
    //LeftRight Rotation when: parentBF = 2 and leftChildBF = -1
    if (parentBF == 2 && calculateBF(rootNode->left) == -1) {
        return rotateLeftRight(rootNode);
    }
    //RightLeft Rotation when: parentBF = -2 and rightChildBF = 1
    if (parentBF == -2 && calculateBF(rootNode->right) == 1){
        return rotateRightLeft(rootNode);
    }
    return rootNode;
}

//Preorder recursion: Root, Left, Right
std::string AVLTree::printPreOrder(Node* rootNode, std::string &fullList)
{
    if (rootNode == nullptr) return "";
    if (!fullList.empty()) fullList += ", "; //add comma before if not first element
    fullList += rootNode->name;
    printPreOrder(rootNode->left, fullList);
    printPreOrder(rootNode->right, fullList);
    return fullList;
}
//Inorder recursion: Left, Root, Right
std::string AVLTree::printInOrder(Node* rootNode, std::string &fullList)
{
    if (rootNode == nullptr) return "";
    printInOrder(rootNode->left, fullList);
    if (!fullList.empty()) fullList += ", "; //add comma before if not first element
    fullList += rootNode->name;
    printInOrder(rootNode->right, fullList);
    return fullList;
}
std::string AVLTree::printInOrderIDS(Node* rootNode, std::string &fullList) //for test cases
{
    if (rootNode == nullptr) return "";
    printInOrder(rootNode->left, fullList);
    if (!fullList.empty()) fullList += ", "; //add comma before if not first element
    fullList += rootNode->ufid;
    printInOrder(rootNode->right, fullList);
    return fullList;
}
//Inorder recursion: Left, Root, Right
Node* AVLTree::deletePostOrder(Node *rootNode) {
    if (rootNode == nullptr) return nullptr;
    deletePostOrder(rootNode->left);
    deletePostOrder(rootNode->right);
    delete rootNode;
    rootNode = nullptr;
    return nullptr;
}
//Create vectors of names and id with preorder recursion
Node* AVLTree::vectorsPreOrder(Node *rootNode, std::vector<std::string> &namesVec, std::vector<std::string> &idVec) {
    if (rootNode == nullptr) return nullptr;
    namesVec.push_back(rootNode->name);
    idVec.push_back(rootNode->ufid);
    vectorsPreOrder(rootNode->left, namesVec, idVec);
    vectorsPreOrder(rootNode->right, namesVec, idVec);
    return nullptr;
}
//Create vectors of names and id with inorder recursion
Node* AVLTree::vectorsInOrder(Node *rootNode, std::vector<std::string> &namesVec, std::vector<std::string> &idVec) {
    if (rootNode == nullptr) return nullptr;
    vectorsInOrder(rootNode->left, namesVec, idVec);
    namesVec.push_back(rootNode->name);
    idVec.push_back(rootNode->ufid);
    vectorsInOrder(rootNode->right, namesVec, idVec);
    return nullptr;
}
//Postorder recursion: Left, Right, Root
std::string AVLTree::printPostOrder(Node* rootNode, std::string &fullList)
{
    if (rootNode == nullptr) return "";
    printPostOrder(rootNode->left, fullList);
    printPostOrder(rootNode->right, fullList);
    if (!fullList.empty()) fullList += ", "; //add comma before if not first element
    fullList += rootNode->name;
    return fullList;
}

Node* AVLTree::findNode(Node* rootNode, std::string& id)
{
    //Use preorder traversal
    if (rootNode == nullptr) return nullptr;
    if (rootNode->ufid == id) return rootNode;
    Node* foundNode = findNode(rootNode->left, id);
    if (foundNode != nullptr) return foundNode;
    return findNode(rootNode->right, id);
}
Node* AVLTree::findParent(Node* rootNode, Node* childNode)
{
    //Use preorder traversal
    if (rootNode == nullptr) return nullptr;
    if ((rootNode->left != nullptr && rootNode->left->ufid == childNode->ufid) ||
        (rootNode->right != nullptr && rootNode->right->ufid == childNode->ufid)){
        return rootNode;
    }
    Node* foundParent = findParent(rootNode->left, childNode);
    if (foundParent != nullptr) return foundParent;
    return findParent(rootNode->right, childNode);
}
Node* AVLTree::findSuccessor(Node* rootNode)
{
    if (rootNode->left == nullptr) return rootNode; //no left child
    return findSuccessor(rootNode->left);
}
bool AVLTree::remove(std::string& id)
{
    Node* nodeFound = findNode(root, id);
    if (nodeFound == nullptr)
    {
        std::cout << "unsuccessful\n"; //ID doesn't exist in tree
        return false;
    }

    //Find parent of node you want to delete
    Node* parentNode = findParent(root, nodeFound);

    //Deleted node has two children
    if (nodeFound->left != nullptr && nodeFound->right != nullptr)
    {
        Node* successor = findSuccessor(nodeFound->right);
        nodeFound->ufid = successor->ufid;
        nodeFound->name = successor->name;
        if (successor->right != nullptr) nodeFound->right = successor->right;
        else nodeFound->right = nullptr;
        delete successor;
        successor = nullptr;
    }
    //Deleted node has one left child
    else if (nodeFound->left != nullptr)
    {
        if (parentNode == nullptr) root = nodeFound->left; //cuz deleting root
        else if (stoi(nodeFound->ufid) < stoi(parentNode->ufid)){
            parentNode->left = nodeFound->left;
        }
        else if (stoi(nodeFound->ufid) > stoi(parentNode->ufid)){
            parentNode->right = nodeFound->left;
        }
        delete nodeFound;
        nodeFound = nullptr;
    }
    //Deleted node has one right child
    else if (nodeFound->right != nullptr)
    {
        if (parentNode == nullptr) root = nodeFound->right; //cuz deleting root
        else if (stoi(nodeFound->ufid) < stoi(parentNode->ufid)){
            parentNode->left = nodeFound->right;
        }
        else if (stoi(nodeFound->ufid) > stoi(parentNode->ufid)){
            parentNode->right = nodeFound->right;
        }
        delete nodeFound;
        nodeFound = nullptr;
    }
    //Deleted node has no children
    else
    {
        if (parentNode == nullptr) root = nullptr; //deleting only node in tree (root)
        else if (stoi(nodeFound->ufid) < stoi(parentNode->ufid)){
            parentNode->left = nullptr;
        }
        else if (stoi(nodeFound->ufid) > stoi(parentNode->ufid)){
            parentNode->right = nullptr;
        }
        delete nodeFound;
        nodeFound = nullptr;
    }

    root = balanceTree(root); //Optional balancing post deletion
    std::cout << "successful\n";
    return true;
}

//Search by ID and print NAME if found
bool AVLTree::searchID(std::string& id)
{
    std::vector<std::string> namesVec;
    std::vector<std::string> idVec;
    vectorsPreOrder(root, namesVec, idVec);
    for (int i = 0; i < (int)idVec.size(); i++)
    {
        if (idVec[i] == id)
        {
            std::cout << namesVec[i] << "\n";
            return true;
        }
    }
    std::cout << "unsuccessful\n";
    return false;
}
//Search by NAME in preOrder and print IDs if found
bool AVLTree::searchName(std::string& name)
{
    bool found = false;
    std::vector<std::string> namesVec;
    std::vector<std::string> idVec;
    vectorsPreOrder(root, namesVec, idVec);
    for (int i = 0; i < (int)namesVec.size(); i++)
    {
        if (namesVec[i] == name)
        {
            std::cout << idVec[i] << std::endl; //print out ID
            found = true;
        }
    }
    if (!found) std::cout << "unsuccessful\n";
    return found;
}

bool AVLTree::printLevelCount()
{
    if (root == nullptr){
        std::cout << "0\n";
        return false;
    }
    std::cout << calculateHeight(root) << std::endl; //height is level count cuz 1-based
    return true;
}

bool AVLTree::removeInOrder(int N)
{
    std::vector<std::string> namesVec;
    std::vector<std::string> idVec;
    vectorsInOrder(root, namesVec, idVec);
    if ((int)namesVec.size() <= N){ //index is larger than number of nodes in tree
        std::cout << "unsuccessful\n";
        return false;
    }
    std::string nodeID = idVec[N];
    Node* deleteNode = findNode(root, nodeID);
    if (deleteNode != nullptr){
        remove(deleteNode->ufid); //already prints successful
        return true;
    } else {
        std::cout << "unsuccessful\n";
        return false;
    }
}