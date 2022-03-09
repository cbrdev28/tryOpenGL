# CBR's runbook for C++ project on Windows

This is my runbook to setup my laptop (Macbook) running on Windows to do some coding in C++.
The idea is to make a video game with Jehan.

- Install [VS Code](https://code.visualstudio.com/)
- Follow [tutorial](https://code.visualstudio.com/docs/cpp/config-mingw) to use C++ on Windows with VS Code
  - Make sure to install the dependencies. No need to actually do the "hello world" stuff.
- Once the dependencies installed, check with VS Code:
  - Start VS Code
  - Try to run the following commands withing the Terminal of VS Code:
    - `gcc --version`
    - `g++ --version`
    - `gdb --version`
- Install the [VS Code extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) for C++
- Add support for MSYS2 MinGW x64 terminal in VS Code:
  - Open your `settings.json` (your user config file for VS Code)
  - Add the following:
  ```json
  "terminal.integrated.profiles.windows": {
    "MSYS2 MinGW x64": {
      "path": "C:/msys64/usr/bin/bash.exe",
      "args": ["--login"],
      "env": {"MSYSTEM": "MINGW64"}
    },
  },
  "terminal.integrated.defaultProfile.windows": "MSYS2 MinGW x64"
  ```

Now we can run commands direclty within VS Code.

## Git setup

- Get invited from Jehan to: https://github.com/Nahor/MyGame
- Make a fork from Jehan's repository: https://github.com/Nahor/MyGame
- Make sure to have a [ssh key for github](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)
  - Install [openssh](https://packages.msys2.org/package/openssh)
    - `pacman -S openssh`
    - Verify with: `ssh -v`
  - Generate ssh key
    - Run the command: `ssh-keygen -t ed25519 -C "foo@bar.com"`
    - And follow the instructions from the github tutorial linked above
- Install [git](https://packages.msys2.org/package/git):
  - `pacman -S git`
  - Verify with: `git --version`

## Exploring how to make a C++ project

At this point, I will just try to put together a C++ project, since I'm stil learning.
The goal is to use cmake and create a project with the following dependencies:
- OpengGL (work in progress)
- cmake
- Other libraries required to make an OpenGL C++ application: glfw, glew, ...

For now I stopped on this section and went back to setup git and use Jehan's project to play around.
I will clone his repository and try to make my first project:
- Use cmake and this [tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- Try to make a packager/installer
- Explore how to fetch and link with libraries
  - fmt...
  - raylib...

- Clone: https://github.com/Nahor/MyGame
- I made my branch `cbr/first-project` and added this document in the `docs` folder
- Add cmake [extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) for VS code (or not... it needs configuration, so I removed it)
- Set git path:
  - Open `settings.json`
  - Add the following:
  ```json
  {
    // Is git enabled
    "git.enabled": true,

    // Path to the git executable
    "git.path": "C:\\msys64\\usr\\bin\\git.exe"
  }
  ```
  - I'm actually still not sure if this did anything...

- Explore cmake and this [tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
  - Install cmake inspired from this [tutorial](https://www.msys2.org/docs/cmake/)
    - I moved Jehan's stuff in a folder to start fresh and simple for now
    - `pacman -S mingw-w64-x86_64-cmake`
    - `pacman -S mingw-w64-x86_64-ninja`
  - Example of commands to build (remember to `mkdir build`):
    - `cmake -G Ninja <path-to-source>`
    - `cmake --build .`
  - The idea is to make my project and play around in my branch (see git history)
  - After installing the dependencies above, I was able to build from the terminal
  - And the [cmake extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) for VS Code seems to work fine (it may ask where to locate your `CMakeLists.txt` file)
    - And will most likely create a `.vscode/settings.json` file

- Explore linting/formatting with [clang-format](https://packages.msys2.org/package/mingw-w64-x86_64-clang)
  - `pacman -S mingw-w64-x86_64-clang`
  - Verify with `clang-format --version`
  - Generate config file: `clang-format -style=Google -dump-config > .clang-format`
  - Test it with: `clang-format -style=file ./src/main.cpp`
- I could not find a way to properly make formatting automatic
  - I was not able to integrate it into the cmake file
  - I was not satisfied with the extension from VS Code, to be continued
- For now I will manually format file with the following command
  - `clang-format -style=file -i <path-to-file>`

- Add library dependency with [fmt](https://github.com/fmtlib/fmt.git)
  - I will try to use `FetchContent` from Jehan's example
  - Apparently when using a cmake module, make sure to include it:
    - `include(FetchContent)`
  - It seems common to also add (I will ask Jehan):
    - `find_package(Threads REQUIRED)`
    - `set(CMAKE_CXX_EXTENSIONS OFF)`
    - `set(CMAKE_CXX_STANDARD 11)`
  - Basically, in `CMakeLists.txt`, I added:
  ```
  include(FetchContent)
  FetchContent_Declare(
      fmt
      GIT_REPOSITORY https://github.com/fmtlib/fmt.git
      GIT_TAG 8.1.1
      UPDATE_DISCONNECTED YES
  )
  FetchContent_MakeAvailable(fmt)
  ...
  target_link_libraries(
    CbrProject 
    PRIVATE
    fmt::fmt 
  )
  ```
  - I was able to build & run:
    - `rm -rf build`
    - `mkdir build`
    - `cd build`
    - `cmake -G Ninja ../src/`
    - `cmake --build .`
    - `./CbrProject.exe`

- I will now try add the `install` steps in the cmake
  - `install(TARGETS CbrProject DESTINATION bin)`
  - `install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)`
- And run the command (after the usual build commands):
 - `cmake --install . --prefix "<path-to-folder>"`
- I was able to run the installed executable but I'm still wondering if the `fmt` dependency should be there too?...

- Try to add the `testing` part in cmake:
```
enable_testing()

# Does the application run
add_test(NAME Runs COMMAND CbrProject)
```
- Rebuild and try the following commands:
  - `ctest -N`
  - `ctest -VV`

- Trying to package an installer
  - I copy/paste the [License from fmt](https://github.com/fmtlib/fmt/blob/master/LICENSE.rst) into my own `License.txt` file
  - Add in cmake:
  ```
  include(InstallRequiredSystemLibraries)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
  set(CPACK_PACKAGE_VERSION_MAJOR "${CbrProject_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${CbrProject_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${CbrProject_VERSION_PATCH}")
  include(CPack)
  ```
  - Try to run (after rebuilding): `cpack -G ZIP`

## Adding raylib & the bunny example

At this point, I will continue taking what Jehan did to adapt it into this little project.
The idea is to:
- Add [raylib](https://www.raylib.com/) as a dependency
- Run the [bunny example](https://www.raylib.com/examples/textures/loader.html?name=textures_bunnymark)
