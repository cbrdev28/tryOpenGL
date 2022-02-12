#ifndef __MAIN_APP_CONTROLLER_H__
#define __MAIN_APP_CONTROLLER_H__

#include "managers/WindowManager.h"
/**
 * This file is the main app controller: the first entry in our app.
 */
class MainAppController {
 public:
  /**
   * Constructor
   * @param argc the argc from main function
   * @param argv the argv from main function
   */
  MainAppController(const int argc, const char* const* const argv);

  // Get rid of constructor we don't plan to use (leaning purpose)
  // Copy constructor
  MainAppController(MainAppController const& other) = delete;

  /**
   * Run the main application
   * @return -1 if an error occurred, 0 otherwise
   */
  int run();

 private:
  const int _argc;
  const char* const* const _argv;
  WindowManager _windowManager;
};

#endif