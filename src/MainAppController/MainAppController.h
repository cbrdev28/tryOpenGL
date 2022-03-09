#ifndef MAIN_APP_CONTROLLER_H_
#define MAIN_APP_CONTROLLER_H_

#include <ImGuiManager.h>
#include <InputManager.h>
#include <WindowManager.h>
#include <WorldManager.h>

/**
 * This file is the main app controller: the first entry in our app.
 */
class MainAppController {
 public:
  /**
   * Constructor
   */
  MainAppController();
  ~MainAppController() = default;

  // Get rid of constructor we don't plan to use (learning purpose)
  // Copy constructor
  MainAppController(const MainAppController& other) = delete;
  MainAppController(MainAppController&& other) = delete;
  auto operator=(const MainAppController& other) -> MainAppController& = delete;
  auto operator=(MainAppController&& other) -> MainAppController& = delete;

  /**
   * Run the main application
   * @return -1 if an error occurred, 0 otherwise
   */
  auto run() -> int;

 private:
  WindowManager windowManager_;
  InputManager inputManager_{windowManager_};
  ImGuiManager imGuiManager_{windowManager_};
  WorldManager worldManager_{windowManager_, inputManager_};

  /**
   * Initialize the app & managers
   * @return -1 if an error occurred, 0 otherwise
   */
  auto init() -> int;

  /**
   * Start a basic render loop
   * @return MainAppController&
   * @throw -1
   */
  auto renderLoop() -> MainAppController&;
};

#endif
