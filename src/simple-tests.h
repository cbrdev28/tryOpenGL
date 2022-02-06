/**
 * This file is meant to do a simple parsing of the args
 * to check if we have:
 * - "--test": to print the self test "Hello tests"
 * - "--test version": to print the version number
 */

#include <iostream>

class SimpleTests {
    public:
    static void SelfTest() {
        std::cout << "Hello tests" << std::endl;
    }
};
