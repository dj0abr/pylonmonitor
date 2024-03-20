#include <iostream>

// Function declaration
void unusedFunction();

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

// Define a function that is never called
void unusedFunction() {
    std::cout << "This function is unused." << std::endl;
}
