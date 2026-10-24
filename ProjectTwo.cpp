//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Daniel Quintana
// Version     : 1.0
// Copyright   : Copyright ? 2023 SNHU COCE
// Description : ABCU Advising Assistance Program using a Binary Search Tree
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// Store course information and any required prerequisites
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Each node holds one course in the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Start with no connected nodes
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Create a new node and store its course information
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    // Helper functions for managing courses in the tree
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Clear();
    void Swap(BinarySearchTree& other);
    // Display courses in alphanumeric order
    void InOrder();

    // Add or search for a course
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Create an empty Binary Search Tree
 */
BinarySearchTree::BinarySearchTree() {
    // Start with an empty tree
    root = nullptr;
}

/**
 * Remove all courses when the tree is destroyed
 */
BinarySearchTree::~BinarySearchTree() {
    Clear();
}

/**
 * Empty the tree so new course data can be loaded
 */
void BinarySearchTree::Clear() {
    // Remove each course node until the tree is empty
    while (root != nullptr) {
        root = removeNode(root, root->course.courseNumber);
    }
}
/**
 * Swap course data between two Binary Search Trees
 */
void BinarySearchTree::Swap(BinarySearchTree& other) {
    // Exchange the root nodes of both trees
    Node* temp = root;
    root = other.root;
    other.root = temp;
}
/**
 * Display courses in alphanumeric order
 */
void BinarySearchTree::InOrder() {
    // Start at the root and display courses in order
    inOrder(root);
}

/**
 * Insert a new course into the Binary Search Tree
 */
void BinarySearchTree::Insert(Course course) {
    // If the tree is empty, create the first course node
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        // Otherwise, find where the course belongs
        addNode(root, course);
    }
}

/**
 * Search the Binary Search Tree for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // Start searching at the root
    Node* current = root;

    // Continue until the course is found or no nodes remain
    while (current != nullptr) {

        // Return the course if the number matches
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }

        // Search the left side if the course number is smaller
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            // Otherwise, search the right side
            current = current->right;
        }
    }

    // Return an empty course if no match is found
    Course course;
    return course;
}

/**
 * Add a course to the correct location in the tree
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Check if the new course number is smaller
    if (course.courseNumber < node->course.courseNumber) {

        // If the left side is empty, add the course here
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            // Otherwise, keep searching on the left
            addNode(node->left, course);
        }
    }
    else {
        // If the right side is empty, add the course here
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            // Otherwise, keep searching on the right
            addNode(node->right, course);
        }
    }
}
/**
 * Display all courses in alphanumeric order
 */
void BinarySearchTree::inOrder(Node* node) {
    // Continue only if the node exists
    if (node != nullptr) {

        // Visit courses with smaller numbers first
        inOrder(node->left);

        // Display the course number and name
        cout << node->course.courseNumber << ", "
            << node->course.courseName << endl;

        // Visit courses with larger numbers last
        inOrder(node->right);
    }
}
/**
 * Remove a course node from the Binary Search Tree
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    // If the node is empty, there is nothing to remove
    if (node == nullptr) {
        return nullptr;
    }

    // Search the left side if the course number is smaller
    if (courseNumber < node->course.courseNumber) {
        node->left = removeNode(node->left, courseNumber);
    }

    // Search the right side if the course number is larger
    else if (courseNumber > node->course.courseNumber) {
        node->right = removeNode(node->right, courseNumber);
    }

    // The course was found
    else {
        // If there is no left child, use the right child
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }

        // If there is no right child, use the left child
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // If there are two children, find the smallest on the right
        Node* temp = node->right;

        while (temp->left != nullptr) {
            temp = temp->left;
        }

        // Replace the course with the next course in order
        node->course = temp->course;

        // Remove the duplicate node
        node->right = removeNode(node->right, temp->course.courseNumber);
    }

    return node;
}



//============================================================================
// Functions for loading and displaying course information
//============================================================================


/**
 * Display a course and its prerequisites
 */
void displayCourse(Course course, BinarySearchTree* bst) {
    // Display the course number and name
    cout << course.courseNumber << ", "
        << course.courseName << endl;

    // Check if the course has any prerequisites
    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";

        // Look up each prerequisite to find its course name
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            Course prerequisite = bst->Search(course.prerequisites[i]);

            cout << course.prerequisites[i];

            if (!prerequisite.courseNumber.empty()) {
                cout << " (" << prerequisite.courseName << ")";
            }

            // Separate multiple prerequisites with commas
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}
/**
 * Read course information from a CSV file
 * and store valid courses in the Binary Search Tree
 */
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    // Open the course information file
    ifstream file(csvPath);

    // Check if the file opened correctly
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    vector<Course> courses;
    string line;

    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        Course course;

        // Read the course number and name
        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseName, ',');

        // Make sure the course has a number and name
        if (course.courseNumber.empty() || course.courseName.empty()) {
            cout << "Error: Invalid course information." << endl;
            return false;
        }

        // Read any remaining values as prerequisites
        while (getline(ss, value, ',')) {
            if (!value.empty()) {
                course.prerequisites.push_back(value);
            }
        }
        // Check whether this course number was already added
        for (const Course& existing : courses) {
            if (existing.courseNumber == course.courseNumber) {
                cout << "Error: Duplicate course number "
                    << course.courseNumber << endl;
                return false;
            }
        }
        courses.push_back(course);
    }

    file.close();
    // Make sure the file contains course information
    if (courses.empty()) {
        cout << "Error: The course file is empty." << endl;
        return false;
    }
    // Check that each prerequisite exists in the file
    for (const Course& course : courses) {
        for (const string& prerequisite : course.prerequisites) {
            bool found = false;

            for (const Course& other : courses) {
                if (prerequisite == other.courseNumber) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Error: Invalid prerequisite "
                    << prerequisite << endl;
                return false;
            }
        }
    }

    // Add validated courses to the Binary Search Tree
    for (const Course& course : courses) {
        bst->Insert(course);
    }

    cout << "Course information loaded successfully." << endl;
    return true;
}
/**
 * Main menu for the ABCU Advising Assistance Program
 */
int main() {
    BinarySearchTree bst;
    string csvPath;
    string courseNumber;
    string input;
    bool dataLoaded = false;

    // Welcome the user
    cout << "Welcome to the ABCU Advising Assistance Program." << endl;

    // Keep showing the menu until the user exits
    while (true) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        getline(cin, input);

        // Load course information from the selected file
        if (input == "1") {
            cout << "Enter the file name: ";
            getline(cin, csvPath);

            // Load the courses into a temporary tree
            BinarySearchTree newTree;

            if (loadCourses(csvPath, &newTree)) {
                // Replace the old courses with the new ones
                bst.Swap(newTree);
                dataLoaded = true;
            }
        }

        // Display all courses in alphanumeric order
        else if (input == "2") {
            if (!dataLoaded) {
                cout << "Please load the course file first." << endl;
            }
            else {
                cout << "\nHere is a sample schedule:" << endl;
                bst.InOrder();
            }
        }

        // Search for and display an individual course
        else if (input == "3") {
            if (!dataLoaded) {
                cout << "Please load the course file first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                Course course = bst.Search(courseNumber);

                if (!course.courseNumber.empty()) {
                    displayCourse(course, &bst);
                }
                else {
                    cout << "Course not found." << endl;
                }
            }
        }

        // Exit the program
        else if (input == "9") {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        // Handle invalid menu selections
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
