#ifndef WINDOW_LISTENER_H_
#define WINDOW_LISTENER_H_

/**
 * An abstract class/interface to allow other managers
 * to subscribe for updates/changes about the window.
 */
class WindowListener {
 public:
  WindowListener() = default;
  virtual ~WindowListener() = default;

  virtual void onResize(int /* width */, int /* height */) {}
  virtual void onKeyCallback(int /* key */, int /* scancode */, int /* action */, int /* mods */) {}

  WindowListener(const WindowListener& other) = delete;
  WindowListener(WindowListener&& other) = delete;
  auto operator=(const WindowListener& other) -> WindowListener& = delete;
  auto operator=(WindowListener&& other) -> WindowListener& = delete;
};

#endif