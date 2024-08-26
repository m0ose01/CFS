# README

![CI Status](https://github.com/m0ose01/CFS/actions/workflows/ci.yml/badge.svg?branch=main)

## About

The Cambridge Electronic Design File System (CFS) is the file format used by the Signal Software Suite to record electrophysiological data, such as data from Transcranial Magnetic Stimulation experiments.

CED distributes a C library to read and write these files.
However, this library only compiles on Windows.
This library is an attempt to reimplement part of CED's CFS library using only platform-agnostic code compliant with the C standard.
Some outdated features will be dropped, such as support for MS-DOS, or near/far pointers for 16-bit segmented memory architectures.

## Goals 

Re-implement the read-only parts of CED's public API.

## Installation

### Build from source

Ensure CMake version 3.22 or later is installed.

#### Unix-like systems

```bash
git clone https://github.com/m0ose01/cfs.git && cd cfs
mkdir build && cd build

cmake ..
cmake --build .
```

This will produce 2 binary files:
- `libcfsapi.so`, a shared library containing a number of the CFS routines.
- `cfstocsv`, a command line program to convert CFS files to CSV format.

## Usage

cfstocsv takes a CFS file as its first argument, and emits a csv file, containing the channel data for each data section.

```
./cfstocsv <filename> <output>
```

If the second argument is omitted, output goes to stdout.

`libcfsapi.so` can be used as a drop-in replacement for the CFS32.dll file provided by CED in some cases.
The following functions from the CFS library have been implemented.

- OpenCFSFile
- CloseCFSFile
- GetGenInfo
- GetFileInfo
- GetFileChan
- GetDSChan
- GetChanData

This project is currently a work-in-progress, and should not yet be used in a real research environment.
Currently, only reading of 32-bit 'equalspaced' integer channel data is supported/tested.
This is something I would like to rectify in the future if possible, by generating test files with the original CFS library.
