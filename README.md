# WELCOME!
Welcome to my repository for my low-level programming/hardware journey! This repository will contain all of my currently completed projects with the Raspberry Pi Pico 2W microcontroller with full documentation. 

Each project is listed in the order that I developed them in, I do not retroactively edit the source code or the README's to preserve authenticity in my growth. Any project can have the following prefixes:
<b>

xx. - Chronological order in which the project was made, represents the n'th project

MP - Major Project, more complex than most of the other projects and is meant to display deep understanding in the subjects that are used

BM - Bare Metal, these projects will use the minimal amount of the hardware SDK in favor of manual register manipulation

Example Project Name: 05.MP_neopixel_led


<br>

Here is the common file structure you will see within each project:
</b>

<br>

xx.Project_Name

|  - README.md (information about the project)

|  - CircutDiagram.jpeg (Diagram of our circut)

\  - src

_ _  | - program_name.c (Main program, can have other supporting programs.)

_ _  | - CMakeLists.txt (CMake script creates our makefiles)

_ _  | - pico_sdk_import.cmake (CMake script that imports the sdk for us)

\  - bin

_ _  | - program_name.uf2 (What will actually run on the pico)

<br>
I have made the following bash function to automate the process of initializing a project for me:

````
export PICOTOOL_DIRECTORY="C:/picotool-2.3.0-x64-win/picotool"
export DEFAULT_FILEPATH="$HOME/pico/Projects/default_files"
picoinit() {
	Project_Name="$1"
	Executable_Name="$2"
	echo "initilizing pico project..."
	
	
	mkdir "$Project_Name"
	mkdir "$Project_Name/src"
	mkdir "$Project_Name/bin"
	touch "$Project_Name/README.md"
	touch "$Project_Name/src/$Executable_Name.c"
	touch "$Project_Name/src/CMakeLists.txt"
	cd "$Project_Name"
	cp "$DEFAULT_FILEPATH/CMakeLists.txt" ./src/
	cp "$DEFAULT_FILEPATH/pico_sdk_import.cmake" ./src/
	cmake -S src -B bin -G Ninja -DPICO_BOARD=pico2_w -Dpicotool_DIR="$PICOTOOL_DIRECTORY" -DPROJECT_NAME="$Project_Name" -DEXE_NAME="$Executable_Name"
	
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
