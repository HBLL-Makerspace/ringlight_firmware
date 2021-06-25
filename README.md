# Ring Light Firmware

This is the firmware for the ringlights that go in the BYU libraries 3D scanning rig. The  ring lights will control the internal lighting environment of the whole rig. There is both RGB and white LEDs that are used in unision to produce a colored environement. The ring lights are dasiy chained and individually addressable.

## Background

This is an experimental portion of the firmware that will utilize the command line and make build system. The biggest draw backs are the lack of simualtors available in the linux environment that cover the attiny1606. The only one I could find was the Atmel Studio AVR simualtor. Part of this project will be to ensure that the build system here will work and can also run in that simulator if needed.

## Getting Started

This project requires several components to work as expected. The following guidelines will get your setup working on a linux based environement. This repository can also be run in a Windows environment, however, will take more work.

The following are a list of dependencies
1. build-essential; make, python3 etc
2. gcc-avr
3. avrdude; version 6.3 with additional modifications to get jtag2updi working.
4. doxygen
5. attiny1606 device pack

### Installing

The following command should install most of what is needed.

```
sudo apt install build-essesntial python3 gcc-avr avrdude dyoxygen
```

You can also compile and install most of these libraries from scratch from there respective websites. A simple google search will get you started on any one of those. The build essesntial package contains many programs that are useful to building code in a linux environemnt. It contains programs like make and gcc, we are only interested in make, since this build environment uses make to build the firmware code. For make to work it needs a Makefile which contains instructions for how to build and install the firmware. Some adjustments might need to be made to the Makefile to get everything working as expected. The Makefile also contians targets for building the documentation using dyoxygen. Dyoxygen will create two directories in the doc folder, an html and latex folder. The html folder contains a static website that has all the documentation inside, while the latex folder contains all the latex files to generate a latex docuement. See the optional section for more information on latex.

### Configure

There are a couple of additional configurations to do to avrdude and adding a device pack before the project will compile. The programmer that I use is an Arduino nano programmed as a jtag2updi interface. Check out these projects to get something similar set up https://github.com/ElTangas/jtag2updi, https://github.com/SpenceKonde/AVR-Guidance/blob/master/UPDI/jtag2updi.md

To configure avrdude to use the jtag2updi programmer the configuration file must be updated. This project contians a modified config file that continas all the needed adjustments. To put the file where it needs to be run the following command in this repos root directory.

```
sudo cp avrdude.conf /usr/local/etc/
```

The next step is adding a device pack from microchip. It involves a few more steps and is easy to not get right. The steps are below with a description and a piece of code that will most likely do the trick.

Create a new directory in your home folder called .avr and another one inside called packs
```
mkdir -p ~/.avr/packs/
```

Go into the directory and download the device pack.
```
cd ~/.avr/packs
wget https://packs.download.microchip.com/Microchip.ATtiny_DFP.2.6.122.atpack
```

Next unzip the downloaded content. You may need to install unzip before running the commnad. It is prety easy just run sudo apt install unzip.
```
unzip Microchip.ATtiny_DFP.2.6.122.atpack
```

Finally remove the zipped file and rename the folder to be 
```
rm Microchip.ATtiny_DFP.2.6.122.atpack
mv Microchip.ATtiny_DFP.2.6.122.atpack Atmel.Attiny_DFP
```

You should end up with a folder in your home directory that looks like this
~/.avr/packs/Atmel.ATtiny_DFP/ which contians all the additional build files for the attiny1606 and other chips. If this does not happen there will be errors while building the firmware.


### Optional (LaTeX)

An optional installation is the latex packages. LaTeX is a documentation generation tool for pdfs and other formats. It is used extensivly in industry to create user manuals and the likes. Each LaTeX project consists of a bunch of tex files, which contain a text based lanaguge to describe the docuement. Take a look at one to get a feel for it. The tex files are used to compile a pdf, and is very configurable. The following command will install latex onto a Ubuntu system, it is a very large install so be prepared for it to take longer than ussual.

```
sudo apt install texlive-full
```

## Building

The build system relies on make which needs a Makefile. The Makefile contians all the instructions needed for make to do its magic. Makefile contain what are called targets. Basically a target is something you tell make you want and it will go and figure out what needs to happen for the target to be completed. There are 4 primary targets in the Makefile in this directory. the targets are all, install, doc and clean. To run the targets simply run make followed by the target. The targets are desribed in detail in the following sections.

Target: all

This will compile the whole project, it will create a directory called build and every source file will be compiled into an object file with its directory struture mirrored in the build directory. 

make will run the first target it sees in the file if no target is specified with the command. So to build just run
```
make
```

Target: install

To install the firmware run the following
```
make install
```

This target depends on the all target, so if you have not run the make all target, it will do it for you before installing. It uses avrdude to install the firmware and will install it to /dev/ttyUSB0. If your programmer is on a different port modify the make file to point to that port.

Target: doc

This target will build the documentation files using doxygen. They will be located in the doc folder.

```
make doc
```

When finsihed there will be two folders in the doc doc folder, html and latex. The html is a static website and the latex folder contains all the tex files. To build a pdf run make in the latex folder.
