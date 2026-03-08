# Smart File Organizer (C++)

A command-line tool built in C++ that automatically organizes files inside a directory.

## Features
- Organize files by type (Images, Documents, Videos, Others)
- Organize files by modification date
- Detect duplicate files
- Recursive folder scanning
- Preview mode before moving files
- Logging system to track file operations
- Error handling using try-catch

## Technologies Used
- C++
- STL (unordered_set)
- std::filesystem

## How to Compile

g++ main.cpp -o organizer.exe

## How to Run

organizer.exe

Enter the folder path you want to organize when prompted.

## Example

Input folder:
Downloads/

After running:
Downloads/
Images/
Documents/
Videos/
Duplicates/