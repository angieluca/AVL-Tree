#include <iostream>
#include <regex>
#include "AVL.h"
using namespace std;

int main(){

    AVLTree tree;
    std::string numCommands;
    getline(std::cin, numCommands);
    int commandsNum = 0;

    //First line must be integer within bounds
    try {
        commandsNum = stoi(numCommands);
        if (commandsNum < 1 || commandsNum > 1000) {
            std::cout << "unsuccessful\n";
            return 0;
        }
    } catch (std::invalid_argument& e) {
        std::cout << "unsuccessful\n";
        return 0;
    }

    std::smatch matches; //smatch object (acts like array) where all matching strings are stored

    //Regex symbols explanation:
    /*
     * Example:
     * std::regex insert("^insert\\s+\"([A-Za-z ]+\"\\s+([0-9]{8})$");
     * "^insert means command must start with word insert
     * \\s+ means one or more spaces after insert (remember \\s cuz escape characters}
     * [A-Za-z\s]+ means allows one or more whitespace except new line
     * \" means opening quote @ start of name
     * [A-Za-z ]+ means all upper and lowercase letters and spaces
     * \" means closing quotes after name
     * \\s+ one or more spaces after the name
     * ([0-9]{8}) means exactly eight-digit number
     * $ command ends here
     * \\d means matches any digit 0-9
     */

    //Create regex patterns to represent type of info you want to find in string:
    std::regex newLine("\n");
    std::regex insert("^insert\\s+\"([A-Za-z\\s]+)\"\\s+([0-9]{8})$");
    std::regex remove("^remove\\s+([0-9]{8})$");
    std::regex searchID("^search\\s+([0-9]{8})$");
    std::regex searchName("^search\\s+\"([A-Za-z ]+)\"$");
    std::regex printInorder("^printInorder$");
    std::regex printPreorder("^printPreorder$");
    std::regex printPostorder("^printPostorder$");
    std::regex printLevelCount("^printLevelCount$");
    std::regex removeInorder("^removeInorder\\s+([0-9]+)$");

    std::string command;
    std::string name;
    std::string ufid;

    //Iterate through the number of commands
    for (int i = 0; i < commandsNum; i++)
    {
        getline(std::cin, command);
        if (regex_match(command, matches,insert))
        {
            name = matches[1]; //matches can be used like indexing system
            ufid = matches[2];
            tree.insert(name, ufid);
        } else if (regex_match(command, matches,remove))
        {
            ufid = matches[1];
            tree.remove(ufid);
        } else if (regex_match(command, matches,searchID))
        {
            ufid = matches[1];
            tree.searchID(ufid);
        } else if (regex_match(command, matches,searchName))
        {
            name = matches[1];
            tree.searchName(name);
        } else if (regex_match(command, matches,printInorder))
        {
            std::string fullList;
            tree.printInOrder(tree.getRoot(), fullList);
            std::cout << fullList << "\n";
        } else if (regex_match(command, matches,printPreorder))
        {
            std::string fullList;
            tree.printPreOrder(tree.getRoot(), fullList);
            std::cout << fullList << "\n";
        } else if (regex_match(command, matches,printPostorder))
        {
            std::string fullList;
            tree.printPostOrder(tree.getRoot(), fullList);
            std::cout << fullList << "\n";
        } else if (regex_match(command, matches,printLevelCount))
        {
            tree.printLevelCount();
        } else if (regex_match(command, matches,removeInorder))
        {
            int NthIndex = stoi(matches[1]);
            tree.removeInOrder(NthIndex);
        } else {
            std::cout << "unsuccessful\n";
        }
    }

	return 0;
}
