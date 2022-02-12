/**
 * This file is the main app controller: the first entry in our app.
 */

#include "MainAppController.h"

#include <iostream>

// Constructor
MainAppController::MainAppController(const int argc,
                                     const char* const* const argv)
    : _argc(argc), _argv(argv) {
  std::cout << "MainAppController(...): argc = " << _argc
            << ", argv = " << _argv << std::endl;
};