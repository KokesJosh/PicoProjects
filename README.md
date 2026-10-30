# WELCOME!
Welcome to my repository for my low-level programming/hardware journey! This repository will contain all of my currently completed projects with the Raspberry Pi Pico 2W microcontroller with full documentation.  Here is the common file structure you will see within each project.

<br>
<br>

Project_Name

|  - README.md (information about the project)

|  - CircutDiagram.jpeg (Diagram of our circut)

\  - src

_ _  | - Project_Name.c (Main program, can have other supporting programs.)

_ _  | - CMakeLists.txt (CMake script creates our makefiles)

_ _  | - pico_sdk_import.cmake (CMake script that imports the sdk for us)

\  - bin

_ _  | - CMakeFiles (For our cmake cache to save our preferences among other things)

_ _  | - Project_Name.uf2 (What will actually run on the pico)

_ _  | - ... (Other misc build files)

<br>
I have made the following nice, albeit janky bash function to automate the process for me:

````
picoinit() {
	echo "initilizing pico project..."
	
	mkdir $1
	mkdir "$1/src"
	mkdir "$1/bin"
	touch "$1/README.md"
	touch "$1/src/$1.c"
	touch "$1/src/CMakeLists.txt"
	cd $1
	cp ~/pico/Projects/default_files/CMakeLists.txt ./src/
	cp ~/pico/Projects/default_files/pico_sdk_import.cmake ./src/
	cmake -S src -B bin -G Ninja -DPICO_BOARD=pico2_w -Dpicotool_DIR="C:/picotool-2.3.0-x64-win/picotool" -DPROJECT_NAME="$1"
	
	echo "pico project built and configured, be sure to check CMakeLists.txt has all necessary dependencies"
}
````

<br>
<br>

# Tools used in project
- Raspberry Pi Pico 2W
- Freenove Ultimate Pico Starter Kit
- CMake
- Ninja
- picotool
- arm-none-eabi-gcc
- Git BASH
- Notepad++