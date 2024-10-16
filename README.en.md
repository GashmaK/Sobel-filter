# Sobel Filter - Laboratory Work №4

This project implements a multithreaded Sobel filter in C++ for edge detection in images. The filter is part of the laboratory work for the "Operating Systems" course at the Moscow Aviation Institute.

## Installing OpenCV

To work with this project, you need to install the OpenCV library. Here's how to do it:

### Installation on Debian

1. Update the package list:
   ```bash
   sudo apt update
   ```

2. Install the necessary packages for OpenCV:
   ```bash
   sudo apt install libopencv-dev
   ```

3. Verify the installation by running:
   ```bash
   pkg-config --modversion opencv4
   ```
   This should display the version of the installed OpenCV library.

## Project Setup

After installing OpenCV, follow these steps to set up the project:

1. Open a terminal and run the following command to get the installation path of OpenCV:
   ```bash
   pkg-config --cflags opencv4
   ```
   The output will look something like this:
   ```
   -I/usr/include/opencv4
   ```
   Copy everything except `-I`. This flag is used to specify the path to header files in Linux, but it is not needed here.

## Setting Up Visual Studio Code

After obtaining the path to OpenCV, follow these steps to set up Visual Studio Code:

1. Open Visual Studio Code.
2. Press `Ctrl + Shift + P` to open the command palette.
3. Type `>edit configurations (JSON)` and select the appropriate option.

This will open the C++ configuration file for compiler settings. You need to add the path to OpenCV that you copied earlier to the `"includePath"` section, for example:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/opencv4"
            ],
            ...
        }
    ],
    "version": 4
}
```

Make sure the path corresponds to your OpenCV installation.

## Compiling the Project

To compile the project using `g++`, use the following command:

```bash
g++ sobel.cpp -o sobel_filter_opencv `pkg-config --cflags --libs opencv4` -pthread
```

## How the Sobel Filter Works

The Sobel filter is used for edge detection in images. It works by comparing each pixel with its neighbors and calculating the gradient, which helps in identifying the edges of the image.

The filter uses two matrices (kernels) to detect changes along the x and y axes. These matrices are convolved with the image to compute the gradient at each point.

## Frequently Asked Questions

### 1. How does the Sobel filter work? What are these matrices?
- The Sobel filter compares pixels with their neighbors and computes values to determine edges.

### 2. What is the purpose of using multithreading?
- Multithreading reduces the execution time of the program, but there is a limit to the effectiveness depending on the number of processor cores.
