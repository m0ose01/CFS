# README

## About

The Cambridge Electronic Design File System (CFS) is the file format used by the Signal Software Suite to record electrophysiological data, such as data from Transcranial Magnetic Stimulation experiments.

CED distributes a C library to read and write these files.
However, this library only compiles on Windows.
This library is an attempt to reimplement part of CED's CFS library using only platform-agnostic code compliant with the C standard.
Some outdated features will be dropped, such as support for MS-DOS, or near/far pointers for 16-bit segmented memory architectures.

## Goals 

Re-implement the read-only parts of CED's public API.

## Installation

1. Clone this repository:

```
git clone https://github.com/m0ose01/CFS.git
```

2. Execute `make` to build the binary.

If you are using Windows, you will need to install [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm).

## Usage

In the clone directory, execute:

```
./bin/cfs <cfs file> <output file>
```

The output file is currently a CSV file, which contains the channel data for each data section.
In future, this functionality will be turned into a standalone program.

This project is currently a work-in-progress, and should not yet be used in a real research environment.
Currently, only reading of 32-bit 'equalspaced' integer channel data is supported/tested.
This is something I would like to rectify in the future if possible, by generating test files with the original CFS library.
