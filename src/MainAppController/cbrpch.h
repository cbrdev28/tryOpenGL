#ifndef CBR_PCH_H_
#define CBR_PCH_H_

#ifdef CBR_WINDOWS
#include <Windows.h>
#endif  // CBR_WINDOWS

#include <memory>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <algorithm>
#include <gsl/gsl>

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <stb_image.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#endif  // CBR_PCH_H_