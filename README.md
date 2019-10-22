# CISC-310: Lab 2

- Author: Nils Olsson
- Semester: SDSU Fall 2019

## About the program

- Uses [cxxopts], an open-source commandline option parser that facilitates building robust
commandline behavior easily. (I wouldn't include this if I thought it impacted any part of the
project signifcantly, it just makes the end-user experience richer.)
- For help and usage, invoke with the `-h` optional flag.

[cxxopts]: https://github.com/jarro2783/cxxopts

## About the project structure

This project structure was originally addapted from [cmake-project-template][cpt] for a project from
last year, [acm-cpp]. The structure is split into two parts, the `src` directory which contains all
source-code for the project proper, and the `test` directory which includes the [googletest][gt]
library and unit-test source files.
(Although I never really got around to implementing any unit-tests for the decoder.)
There's also a folder, `samples`, of the sample code and sequence files which the professor
provided.

[cpt]: https://github.com/kigster/cmake-project-template
[acm-cpp]: https://github.com/nilsso/acm-cpp
[cmake]: https://cmake.org/
[gt]: https://github.com/google/googletest

## Building

From the command-line, make a directory called `build` in the root of the project, move into it,
invoke `cmake` with the previous directory as its argument, and invoke `make` with the generated
Makefiles.

```bash
mkdir build
cd build
cmake ..
make
```

(Additionally, running `make install` will move the built binaries, built libraries, and header
files into these folders in the project root respectively: `bin`, `lib`, `include`. Resetting the
project structure to a clean state after having "installed" is just a matter of deleting these
directories: `rm -rf {bin,lib,include}`.)

## Running

The built binary will be located at `build/src/lab2`.
Invoke with the `-h` flag for a full list of options to the program.
By default, attempts to read MILES code patterns from a file named `miles.csv` and sequences from a
fale named `seqs.csv` (these files can be changed through optional flags). Additional optional
arguments are `-n` for normalizing input sequences, and `-o <FILE>` for specifying an output file
into which found MILES codes will be written.

## MILES code/sequence generator

Additional to the project, I wrote a quick Python script to help generate MILES code and sequences
files for use in testing the main program. Similar to the main program, invoking
`./miles_generator.py -h` will print a full list of options. But for example:

```bash
> ./miles_generator.py m 0 1 7

0,20,100,140,160,200,0
0,20,60,120,180,200,1
0,20,60,80,120,140,7

> ./miles_generator.py s 0 1 7

0,20,100,140,160,200,220,240,280,340,400,420,440,460,500,520,560,580
```

Note that the values of the output sequence will always be sequential.
