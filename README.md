# Image Steganography using LSB Algorithm in C

## Overview

Image Steganography is a command-line application developed in **C** that hides and extracts secret text files inside **24-bit BMP images** using the Least Significant Bit (LSB) technique.

The project demonstrates low-level file processing, bit manipulation, structures, pointers, and modular C programming.

## Key Features

* Hides text data inside 24-bit BMP images.
* Extracts hidden data from encoded images.
* Uses the Least Significant Bit technique.
* Supports command-line arguments.
* Performs input and argument validation.
* Processes BMP image files.
* Generates output image files automatically.
* Includes error handling for invalid operations.

## How It Works

During encoding, the application reads the BMP image and modifies selected least significant bits of image data to store the secret text information.

During decoding, the application reads the modified image and extracts the embedded information from the corresponding bits.

The image data is processed at the byte and bit level while maintaining the overall image structure.

## Implementation

The project uses a modular architecture with separate encoding and decoding functionality. Structures, pointers, file handling, and standard C libraries are used to organize the implementation.

The application also performs validation to ensure that the required input files and command-line arguments are available before processing.

## Technologies

* C
* File Handling
* Bit Manipulation
* Data Structures
* Pointers
* Structures
* BMP Image Processing
* Standard C Libraries

## Learning Outcomes

This project provided practical experience with binary file processing, bit-level operations, dynamic data handling, command-line applications, and modular C programming.
