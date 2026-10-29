# Steganography
A robust, command-line Steganography Application written in C. This program allows you to securely hide secret text files (such as confidential notes or passwords inside standard BMP image files using Least Significant Bit (LSB) encoding without visibly altering the cover image. 

# Features

* Data Hiding (Encoding): Embed any secret text file (`.txt`) inside a carrier BMP image (`.bmp`) using LSB substitution.
* Data Retrieval (Decoding): Extract hidden messages and files from the stego-image back into an output text file.
* Magic String Verification: Uses a unique signature (`#*`)[cite: 13] to verify whether an image actually contains hidden steganographic data before decoding[cite: 14].
* Dynamic Capacity Check: Automatically calculates the image capacity to ensure the source image is large enough to hold the secret data.
* Header Preservation: Copies the original 54-byte BMP header intact so the carrier image remains fully viewable and uncorrupted.

 📂 Project Structure

├── main.c       # Program entry point and operation routing (Encode/Decode)
├── encode.c     # Core logic for hiding data, file validation, and LSB encoding[cite: 16]
├── encode.h     # Header file defining encoding structures and function prototypes[cite: 17]
├── decode.c     # Core logic for extracting hidden data and validation[cite: 14]
├── decode.h     # Header file defining decoding structures and function prototypes[cite: 15]
├── common.h     # Shared macros (e.g., Magic String definition `#*`)[cite: 13]
└── types.h      # User-defined status types (`e_success`, `e_failure`, etc.)

# Getting Started Prerequisites:
You need a C compiler installed on your system (such as gcc or MinGW). 
Note: The input carrier image must be in uncompressed BMP format.CompilationClone the repository and compile all source files together using your terminal:Bashgcc main.c encode.c decode.c -o stego

# Usage Instructions: 
The program takes command-line arguments to dictate whether it should run in Encoding or Decoding mode.

1. Encoding (Hide a secret file inside an image): Run the executable with the -e flag, followed by the source BMP image, the secret text file, and optionally an output stego image name:   Bash./stego -e beautiful.bmp secret.txt stego.bmp. If you omit stego.bmp, the program defaults to naming the output stego.bmp.

2. Decoding (Extract the secret file from the stego image): Run the executable with the -d flag, followed by the stego image and optionally the destination output file name:   Bash./stego -d stego.bmp decoded.txt. If you omit decoded.txt, the program defaults to saving the output as decoded.txt.


# Technical Workflow Validation: 
Checks command-line arguments, validates file extensions (.bmp for images, .txt for secrets), and opens file streams safely.Capacity Calculation: Computes image pixel capacity to ensure it exceeds the size of the magic string, file extension size, file extension data, file size integer, and actual payload.Header Copying: Copies the 54-byte header directly from the source image to the destination image.Bit Manipulation (LSB): Modifies the least significant bit of the image's RGB bytes to sequentially store data bits without changing the color profile perceivably.
