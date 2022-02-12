#ifndef __MAIN_APP_CONTROLLER_H__
#define __MAIN_APP_CONTROLLER_H__

/**
 * This file is the main app controller: the first entry in our app.
 */

class MainAppController {
 public:
  MainAppController(const int argc, const char* const* const argv);
  // Get rid of constructor we don't plan to use (leaning purpose)
  // Copy constructor
  MainAppController(MainAppController const& other) = delete;

  int run();

 private:
  const int _argc;
  const char* const* const _argv;
};

#endif
