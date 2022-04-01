#ifndef KEYBOARD_LISTENER_H_
#define KEYBOARD_LISTENER_H_

/**
 * Abstract class to subscribe for keyboard actions
 */
class KeyboardListener {
 public:
  KeyboardListener() = default;
  virtual ~KeyboardListener() = default;

  virtual void onMoveForward(){};
  virtual void onMoveBackward(){};
  virtual void onMoveLeft(){};
  virtual void onMoveRight(){};
  virtual void onZoomOut(){};
  virtual void onZoomIn(){};

  KeyboardListener(const KeyboardListener& other) = default;
  KeyboardListener(KeyboardListener&& other) = delete;
  auto operator=(const KeyboardListener& other) -> KeyboardListener& = delete;
  auto operator=(KeyboardListener&& other) -> KeyboardListener& = delete;
};

#endif