# CISC-310: Lab 2

- Author: Nils Olsson
- Semester: SDSU Fall 2019

## About the program

The driver program `lab2` is in charge of:

- Streaming data from a MILES CSV file and constructing a list of MILES code objects
- Streaming data from a sequence CSV file into a list of sequences (queues)
- Constructing a decoder from the list of MILES code objects
- Using the decoder to decode the sequences and collecting found IDs
- Outputting the found MILES code IDs in CSV format

## About the project structure

This project structure was originally adapted from [cmake-project-template][cpt] for a project from
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

## Acquiring and building

**(Note: cloning wont work since the repository is currently private)**

To clone this project along with the [googletest][gt] submodule:

```bash
git clone --recursive git://github.com/nilsso/cisc310-lab2
```

Then from the command-line, make a directory called `build` in the root of the project, move into it,
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
file named `seqs.csv` (these files can be changed through optional flags), but these defaults can be
replace by invoking the program with file locations for the MILES file and sequences file, in that
order.

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
