#include <catch2/catch_test_macros.hpp>
#include <random>
#include "regex"
#include "AVL.h"

using namespace std;

//Test at least five incorrect commands. Ex: insert "A11y" 45679999
TEST_CASE("Test 1 - Invalid Command Inputs", "[flag]"){

    std::regex insert("^insert\\s+\"([A-Za-z\\s]+)\"\\s+([0-9]{8})$");

    std::string command = "insert \"A11y\" 45679999"; //name includes non-letters
    bool attempt1 = regex_match(command,insert);
	REQUIRE(!attempt1); //Incorrect name should result in false

    command = "insert A11y 45679999"; //name isn't enclosed by quotes
    bool attempt2 = regex_match(command,insert);
    REQUIRE(!attempt2); //Incorrect name should result in false

    command = "inserttttt \"Abby\" 45679999"; //insert command misspelled
    bool attempt3 = regex_match(command,insert);
    REQUIRE(!attempt3); //Incorrect command should result in false

    command = "INseRT \"abby\" 45679999"; //mix of upper and lowercase in command name
    bool attempt4 = regex_match(command,insert);
    REQUIRE(!attempt4); //capitalization affects it

    command = "insert \"    a   b  c   d    e\" 45679999"; //lots of dif whitespace in name
    bool attempt5 = regex_match(command,insert);
    REQUIRE(attempt5); //no limit to whitespace

    command = "insert \"ab\\nby\" 45679999"; //newline in name
    bool attempt6 = regex_match(command,insert);
    REQUIRE(!attempt6); //newline is not allowed as whitespace in name

    command = "insert \"Abby\" 123456789"; //ufid doesn't have exactly 8 digits
    bool attempt7 = regex_match(command,insert);
    REQUIRE(!attempt7); //Incorrect ufid should result in false

    command = "insert \"Abby\" 1234 5678"; //ufid should be exactly 8 digits altogether
    bool attempt8 = regex_match(command,insert);
    REQUIRE(!attempt8); //Incorrect ufid should result in false

    command = "insert \"Abby\" 1234#678"; //ufid can only have digits
    bool attempt9 = regex_match(command,insert);
    REQUIRE(!attempt9); //Incorrect ufid should result in false

    command = "insert \\n \"Abby\" 12345678"; //command contains newline before the end
    bool attempt10 = regex_match(command,insert);
    REQUIRE(!attempt10); //command can't contain newline characters

}

//Test at least three edge cases for various functions. Ex: removing a node that does not exist
TEST_CASE("Test 2 - Edge cases", "[flag]"){
	// you can also use "sections" to share setup code between tests
    // each section runs the setup code independently to ensure that they don't affect each other
	AVLTree testcase2;
    std::string existingName = "Angela Luca";
    std::string nonExistingName = "Perry the Platypus";
    std::string existingID= "12345678";
    std::string nonExistingID = "87654321";

	SECTION("Remove node from an empty tree") {
		bool attempt1 = testcase2.remove(nonExistingID);
		REQUIRE(!attempt1);
	};

    SECTION("Remove node that doesn't exist in the tree") {
        testcase2.insert(existingName, existingID);
        bool attempt1 = testcase2.remove(nonExistingID); //should be false
        bool otherAttempt1 = testcase2.remove(existingID); //should be true
        REQUIRE(!attempt1);
        REQUIRE(otherAttempt1);
    };

    SECTION("Remove Nth Node that doesn't exist"){
        testcase2.insert(existingName, existingID);
        testcase2.insert(nonExistingName, nonExistingID);
        bool attempt = testcase2.removeInOrder(2); //max index is 1
        REQUIRE(!attempt);
    };

    SECTION("Insert duplicate IDs, duplicate names, or duplicate name & ID pair"){
        bool attempt1 = testcase2.insert(existingName, existingID); //should be true
        bool otherAttempt1 = testcase2.insert(nonExistingName, existingID); //duplicate ID so should be false
        bool attempt2 = testcase2.insert(existingName, nonExistingID); //duplicate names are allowed with dif ids
        bool otherAttempt2 = testcase2.insert(existingName, existingID); //tries to insert name & id pair that already exists
        REQUIRE(attempt1);
        REQUIRE(!otherAttempt1);
        REQUIRE(attempt2);
        REQUIRE(!otherAttempt2);
    };

}

//Test all four rotation cases
TEST_CASE("Test 3 - Rotation cases", "[flag]"){
    AVLTree testcase3;
    std::string name1 = "Phineas";
    std::string name2 = "Perry the Platypus";
    std::string name3 = "Ferb";
    std::string id1 = "12345678";
    std::string id2 = "23456789";
    std::string id3 = "34567890";

    SECTION("Test left rotation") {
        //root should go from name1 to name2
        testcase3.insert(name1, id1);
        REQUIRE(testcase3.getRoot()->name == name1);
        testcase3.insert(name2, id2);
        REQUIRE(testcase3.getRoot()->name == name1);
        testcase3.insert(name3, id3); //Now left rotation should occur
        REQUIRE(testcase3.getRoot()->name == name2);
    };

    SECTION("Test right rotation") {
        //root should go from name3 to name2
        testcase3.insert(name3, id3);
        REQUIRE(testcase3.getRoot()->name == name3);
        testcase3.insert(name2, id2);
        REQUIRE(testcase3.getRoot()->name == name3);
        testcase3.insert(name1, id1); //Now right rotation should occur
        REQUIRE(testcase3.getRoot()->name == name2);
    };

    SECTION("Test right-left rotation") {
        //root should go from name1 to name2
        testcase3.insert(name1, id1);
        REQUIRE(testcase3.getRoot()->name == name1);
        testcase3.insert(name3, id3);
        REQUIRE(testcase3.getRoot()->name == name1);
        testcase3.insert(name2, id2); //Now right-left rotation should occur
        REQUIRE(testcase3.getRoot()->name == name2);
    };

    SECTION("Test left-right rotation") {
        //root should go from name3 to name2
        testcase3.insert(name3, id3);
        REQUIRE(testcase3.getRoot()->name == name3);
        testcase3.insert(name1, id1);
        REQUIRE(testcase3.getRoot()->name == name3);
        testcase3.insert(name2, id2); //Now left-right rotation should occur
        REQUIRE(testcase3.getRoot()->name == name2);
    };

}

//Test all three deletion cases (two children, one child (left and right), no children)
TEST_CASE("Test 4 - Deletion cases", "[flag]"){
    AVLTree testcase4;
    std::string name1 = "Phineas";
    std::string name2 = "Perry the Platypus";
    std::string name3 = "Ferb";
    std::string id1 = "12345678";
    std::string id2 = "23456789";
    std::string id3 = "34567890";

    SECTION("Test delete node with 2 children") {
        //root should go from name2 to inorder successor name3
        testcase4.insert(name1, id1);
        testcase4.insert(name2, id2);
        testcase4.insert(name3, id3);
        REQUIRE(testcase4.getRoot()->name == name2);
        testcase4.remove(id2);
        REQUIRE(testcase4.getRoot()->name == name3); //root is now inorder successor
    };

    SECTION("Test delete node with 1 left child") {
        std::string name4 = "Isabella";
        std::string id4 = "10000000";
        //root->left should go from name1 to name4
        testcase4.insert(name1, id1);
        testcase4.insert(name2, id2);
        testcase4.insert(name3, id3);
        testcase4.insert(name4, id4);
        REQUIRE(testcase4.getRoot()->left->name == name1);
        testcase4.remove(id1); //remove node that has only one left child
        REQUIRE(testcase4.getRoot()->left->name == name4);
    };

    SECTION("Test delete node with 1 right child") {
        std::string name4 = "Isabella";
        std::string id4 = "15000000";
        //root->left should go from name1 to name4
        testcase4.insert(name1, id1);
        testcase4.insert(name2, id2);
        testcase4.insert(name3, id3);
        testcase4.insert(name4, id4);
        REQUIRE(testcase4.getRoot()->left->name == name1);
        testcase4.remove(id1); //remove node that has only one right child
        REQUIRE(testcase4.getRoot()->left->name == name4);
    };

    SECTION("Test delete node with 0 children") {
        std::string name4 = "Isabella";
        std::string id4 = "10000000";
        //root should go from name2 to name1
        testcase4.insert(name1, id1);
        testcase4.insert(name2, id2);
        testcase4.insert(name3, id3);
        testcase4.insert(name4, id4);
        REQUIRE(testcase4.getRoot()->name == name2);
        testcase4.remove(id3); //remove node that has only one right child
        REQUIRE(testcase4.getRoot()->name == name1);
    };

}

//Insert >= 100 nodes, remove ten random nodes using any delete operation. Check in order
TEST_CASE("Test 5 - Deletion cases", "[flag]") {
    AVLTree testcase5;
    std::string expectedOutput = "";
    std::string actualOutput = "";
    //each node will have same name, but dif ID
    int idExample = 00000001;
    std::string id;
    std::string name = "Angie";

    //create 100 nodes all named Angie with IDs from 00000001 to 00000101
    for(int i = 0; i < 111; i++)
    {
        id = std::to_string(idExample);
        testcase5.insert(name, id);
        idExample++;
    }

    //remove 10 random nodes
    std::random_device generator;
    std::mt19937 gen(generator());
    std::uniform_int_distribution<> dist(1, 10);
    int randomNode = dist(gen);

    for (int i = 0; i < 10; i++){
        testcase5.removeInOrder(i * randomNode);
    }

    actualOutput = testcase5.printInOrder(testcase5.getRoot(), actualOutput);
    REQUIRE(testcase5.printInOrder(testcase5.getRoot(), actualOutput) == actualOutput);
}
