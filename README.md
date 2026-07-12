# Image Compression with Quadtrees

## Description
This project implements an image compression algorithm using **Quadtrees**. A quadtree is a tree data structure where each internal node has exactly four children. In this context, it recursively subdivides an image into four quadrants to approximate regions of similar colors, thereby compressing the image. The project features a graphical user interface (GUI) built with the [MLV library](http://www-igm.univ-mlv.fr/~boussica/mlv/index.html).

It allows you to:
- Load an image and perform a quadtree approximation.
- Minimize the generated quadtree by merging identical branches.
- Save and load the compressed quadtrees in custom binary formats (`.qtn` for Black & White, `.qtc` for RGBA).

Lien vers le repository du projet :
https://github.com/ghassenhnid/Image-Compression-with-Quadtrees

## Requirements
To compile and run this project, you need:
- A C compiler (e.g., `gcc`)
- `make` utility
- **MLV Library**: The *Bibliothèque MLV* must be installed on your system.

### Installing the MLV Library
The MLV (Multimédia LaBRI) library is an educational C library. Depending on your OS, you can install it as follows:

**Linux (Debian/Ubuntu):**
You can install it directly via the package manager:
```bash
sudo apt-get update
sudo apt-get install libmlv3-dev
```

**macOS:**
You can install it using Homebrew. Since it relies on SDL, install the SDL dependencies first:
```bash
brew install sdl2 sdl2_gfx sdl2_image sdl2_ttf sdl2_mixer
```
Then follow the source compilation instructions from the [official MLV website](http://www-igm.univ-mlv.fr/~boussica/mlv/index.html).

**Windows:**
1. **Using MSYS2 / MinGW-w64 (Recommended):**
   - Install MSYS2.
   - Install the GCC toolchain and `make` (`pacman -S mingw-w64-x86_64-gcc make`).
   - Download the precompiled MLV MinGW bundle from the [official MLV website](http://www-igm.univ-mlv.fr/~boussica/mlv/index.html).
   - Extract the `.h` files into your MinGW `include` directory and the `.a` library files into the `lib` directory. Place any `.dll` files next to your `main.exe`.
2. **Using WSL (Windows Subsystem for Linux):**
   - Install WSL with Ubuntu.
   - Run the Linux instructions above (`sudo apt-get install libmlv3-dev`). You will also need an X11 server like VcXsrv to run graphical applications from WSL.

## How to Build and Launch
1. Open a terminal and navigate to the `src` directory of the project:
   ```bash
   cd src
   ```
2. Build the project using `make`:
   ```bash
   make
   ```
3. Run the generated executable:
   ```bash
   ./main
   ```
   *(On Windows, you may run `main.exe` instead, provided the MLV library is properly configured for your environment).*

## Usage
Once launched, the GUI will present several buttons to interact with:
- **Load Image**: Enter the path to an image (e.g., `../imgs/earth.jpeg`).
- **Quadtree Approximation**: Starts the process of approximating the loaded image into a quadtree.
- **Minimize Quadtree**: Optimizes the tree structure by combining identical branches.
- **Save Binary BW / RGBA**: Saves the quadtree structure as a `.qtn` or `.qtc` file in a `compressed/` folder.
- **Load minimized Image**: Load a previously saved `.qtn` or `.qtc` file to view it.

## Directory Structure
- `src/`: Contains the C source code and the `makefile`.
- `imgs/`: Contains sample images to test the compression.
- `doc/`: Contains project documentation and reports.