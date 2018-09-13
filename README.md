# Tetsudai

## What is it?
Tetsudai is a Japanese language dictionary program written in C++/Qt. It 
supports character recognition from screengrabs suing Tesseract OCR and displays
kanji stroke diagrams and meanings along with part of speech and definition 
information.

## Compiling
First, make sure you have all the dependencies installed. Tetsudai requires Qt, 
Tesseract OCR, and SQLite. Then, run qmake, followed by make. Currently, 
compiling and running Tetsudai is only supported on Linux.

## Screenshots
### Wildcard search
![Wildcard search](screenshots/screenshot_search.png?raw=true "Wildcard search")

### Term definitions and character stroke diagrams
![Term definitions and character stroke diagrams](screenshots/screenshot_term.png?raw=true "Term definitions and character stroke diagrams")

### Character recognition from screengrab
![Character recognition from screengrab](screenshots/screenshot_ocr.png?raw=true "Character recognition from screengrab")

