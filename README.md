# Imagine-Image
Object oriented C++ image editor application

## Info
Author: Brian Veber, contact: bveber@uoregon.edu

## About:
Imagine Image is a object oriented C++ image editor that implements logging and exception handling, virtual functions, inheritance, file I/O and more. It comes 
with the following features (which can be stacked):
```
* LRCombine - takes two input images that have same height and makes a single image where they're combined left-to-right
* TBCombine - takes two input images that have same width and makes a single image where they're combined left-to-right
* HalfSize - splits the length and width of the image in half
* Blend - takes two images of same w and h and blends them according to the degree of user-inputted blend factor
* Blur - blurs an image
* Subtract - finds the difference between two input images
* Rotate - performs a 90-degree clockwise rotation on an image
* Mirror - performs a horizontal left/right on an image
* Grayscale - removes color out of an image
* ConstantColor - takes a color and a size, and produces an image of that size with that color
* Shrinker - shrinks an image
```

## Getting Started
To run Imagine Image, go to your command terminal and cd to your imagine image directory. Head to the file `main3G.C` and edit the main as necessary for what operations listed above you want to perform on your input image. Then run `make` on the command line. Once the process has completed, run `./<name of your executable> <your input image.pnm> <your desired output image file name>`. The application will then create your output image in the folder you're in to which you can do with as you wish!

## Examples

* Running the default main3G.C filters on this image:

![alt text](https://github.com/bcveber/CIS330/blob/master/3G/exampleinput.png)

Produces an image with the following result:

![alt text](https://github.com/bcveber/CIS330/blob/master/3G/exampleoutput.png)

## App Quality / Testing

* This program was tested against 85 stress tests (found in CIS330/3H on my github) created by colleagues and professors and passed 85/85 tests:

![alt text](https://github.com/bcveber/CIS330/blob/master/3G/stress_tests_passes.png)

* This program was also tested with valgrind. No memory leaks were found:

![alt text](https://github.com/bcveber/CIS330/blob/master/3G/valgrind_check.png)
