# tryOpenGL

This repository is a playground to try implementing a C++ program using OpenGL (on Windows)

## Runbook

I will use this README as a runbook for now, to describe pretty much all the steps required for this project to run.

### Dependencies

- Install [msys2](https://www.msys2.org/)

  - Download their installer and follow their instructions
  - This is the terminal we will use: **MSYS2 MinGW x64**
  - Complete the setup with the commands they mention:
    - `pacman -Syu`
    - Make sure everything is up to date, close the terminal (if it didn't do it by itself), and re-run it from the Start menu (we use: **MSYS2 MinGW x64**)
      - And run: `pacman -Su`
    - Follow their recommended tools to install (I did not spend time trying to understand all the tools details):
      - `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
  - Add the path to your Mingw-w64 bin folder to the Windows PATH environment variable by using the following steps:
    - In the Windows search bar, type 'settings' to open your Windows Settings
    - Search for Edit environment variables for your account
    - Choose the Path variable in your User variables and then select Edit
    - Select New and add the Mingw-w64 destination folder path to the system path. The exact path depends on which version of Mingw-w64 you have installed and where you installed it. If you used the settings above to install Mingw-w64, then add this to the path: `C:\msys64\mingw64\bin`
    - Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available
  - Re-open the **MSYS2 MinGW x64** terminal and try:
    - `gcc --version`
    - `g++ --version`
    - `gdb --version`

- Install [VS Code](https://code.visualstudio.com/)

  - Install the [C++ VS Code extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  - Enable the **MSYS2 MinGW x64** terminal in VS Code:
    - Open your user config file for VS Code: `settings.json`
      - By default this should work: `Ctrl + Shift + p` to enter commands in VS Code, and start typing: `settings`, which should offer to open the JSON settings file
    - In your `settings.json`, add the following:
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
    - Now by default, the terminal in VS Code should be our favorite **MSYS2 MinGW x64**, where we can run commands

- Git setup

  - Install [git](https://packages.msys2.org/package/git)
    - `pacman -S git`
    - Verify with: `git --version`
  - Make sure to [setup a ssh key for Github](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)
    - Install [openssh](https://packages.msys2.org/package/openssh)
      - `pacman -S openssh`
      - Verify with: `ssh -v`
    - Generate ssh key
      - Run the command: `ssh-keygen -t ed25519 -C "your-email@mail.com"`
      - And follow the instructions from the github tutorial linked above to add your ssh key
  - Make sure VS Code can find git, open your `settings.json` and add:

  ```json
  "git.enabled": true,
  "git.path": "C:\\msys64\\usr\\bin\\git.exe"
  ```

- Install [cmake](https://www.msys2.org/docs/cmake/)

  - `pacman -S mingw-w64-x86_64-cmake`
  - `pacman -S mingw-w64-x86_64-ninja`
  - Verify with: `cmake --version`
  - TODO: explore the [VS Code extension for cmake](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

- Install [clang-format](https://packages.msys2.org/package/mingw-w64-x86_64-clang)
  - `pacman -S mingw-w64-x86_64-clang`
  - Verify with `clang-format --version`
  - TODO: generate a `.clang-format` file based on Google style & explore the [VS Code extension for clang-format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
    - I will try the following content for my first `.clang-format`:
    ```YAML
    BasedOnStyle: Google
    ```
    - To run & format a file: `clang-format -style=file -i <path-to-file>`

For now, this should be all the dependencies we need. I will come back to this section in case we add more in the future.

### Building & Running Hello World C++

At this point, I will try to put together a very basic C++ project with a Hello World as a starting point to run & build. The idea is to use some concept from this [cmake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html).

- Build & run the first Hello world
  - Clone: `git clone git@github.com:cbrdev28/tryOpenGL.git`
  - Checkout branch: `git checkout cbr/first-project`
  - Try to build & run:
    - `mkdir build`
    - `cd build`
    - `cmake ../`
    - `cmake --build .`
    - `./tryOpenGL.exe`

### Adding my first helper class

I would like to do a simple parsing of the arguments passed to the main function, in order to run things like: `./tryOpenGL.exe --test version`, which would run the program to only print the version number.
I will try to achieve that with a single file: `./src/simple-tests.h`

### Experimenting with a first window

The goal here is to add dependencies to be able to open a window and hopefully some OpenGL.
NOTE: I updated our `CMakeLists.txt` and added some scripts under `./scripts/`. [Link](https://github.com/cbrdev28/tryOpenGL/pull/2) to the pull request.

- Add [glfw](https://github.com/glfw/glfw) dependency
  - By trying to use `FetchContent` in `CMakeLists.txt`
  - And then `target_link_libraries(...)`
- Add [glad](https://glad.dav1d.de/) dependency
  - Open their [web-site](https://glad.dav1d.de/)
    - Make sure the language is set to C++
    - In the API section select an OpenGL version of at least 3.3
    - Make sure the profile is set to Core
    - Make sure the Generate a loader option is ticked
    - Click Generate to produce the resulting library files
  - GLAD should have provided a zip file with: two include folders & a single `glad.c` file
    - Copy the `glad.c` file in `./src/`
    - Copy each folder from the `include` to `./src/include/`
  - I need to update our `CMakeLists.txt` to include these dependencies
    - For `glad.c`:
    ```
    add_executable(tryOpenGL main.cc "src/glad.c")
    ```
    - For the 2 includes, trying:
    ```
    target_include_directories(
      tryOpenGL
      PUBLIC
      "src/include"
    )
    ```
- I added `#include <glad/glad.h>` in `simple.tests.h`
  - And was able to run: `./scripts/cbr-test.sh`
- TODO: check to update install steps for `cmake` with new dependencies & includes

### Experimenting with clang-tidy

- Install [extra dependency](https://packages.msys2.org/package/mingw-w64-x86_64-clang-tools-extra):
  - `pacman -S mingw-w64-x86_64-clang-tools-extra`
  - Verify with: `clang-tidy --version`
- Basically, I added:
  - A config/rule file: `.clang-tidy`
  - A script to run clang-tidy: `./scripts/cbr-clang-tidy.sh`

### More environment variable & VS Code extensions

MSYS2 installs some binaries in a different folder than the one set in our environment variables (see beginning of this document).
For example, VS Code (or Visual Studio) are not able to find `git`. I fixed this by:

- Adding the following path `C:\msys64\usr\bin` to my users environment variables

VS Code extensions I ended up using:

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)

Dump of my `settings.json`:

```json
{
  "terminal.integrated.profiles.windows": {
    "MSYS2 MinGW x64": {
      "path": "C:/msys64/usr/bin/bash.exe",
      "args": ["--login"],
      "env": { "MSYSTEM": "MINGW64" }
    }
  },
  "terminal.integrated.defaultProfile.windows": "MSYS2 MinGW x64",
  "git.path": "C:\\msys64\\usr\\bin\\git.exe",
  "git.enabled": true,
  "[cpp]": {
    "editor.formatOnSave": true
  },
  "cSpell.userWords": [
    "Cherno",
    "Glfw",
    "IMGUI",
    "Lclampf",
    "Lsizei",
    "NOLINTNEXTLINE",
    "OPENGL",
    "Programiv",
    "stbi"
  ],
  "cmake.configureOnOpen": false,
  "cmake.configureOnEdit": false,
  "editor.renderWhitespace": "all",
  "editor.fontFamily": "Cascadia mono, Consolas, 'Courier New', monospace",
  "C_Cpp.intelliSenseEngine": "Disabled"
}
```

### Installing ctidy on Mac

References/Links:

- https://gist.github.com/sleepdefic1t/e9bdb1a66b05aa043ab9a2ab6c929509
- https://stackoverflow.com/questions/53111082/how-to-install-clang-tidy-on-macos

Instead we need to use `llvm` which is available from `brew`:

```zsh
brew install llvm
ln -s "$(brew --prefix llvm)/bin/clang-format" "/usr/local/bin/clang-format"
ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
ln -s "$(brew --prefix llvm)/bin/clang-apply-replacements" "/usr/local/bin/clang-apply-replacements"
```
