# LibMusicXML

## Validation tools

This folder contains validation tools intended to check the output of the following tools:
- xml2read : read a musicxml file and print it
- xml2guido : converts musicxml to guido music notation format
- xml2ly : converts musicxml to lilypond format
- xml2brl : converts musicxml to braille format

When running the validation, you should make sure that the above tools are on your path, or you should indicate the path to these tools.

The principle of the validation is the following :
- first, you have to boostrap the system with a first run. The output has to be checked manually or using any associated compilers (guido2svg, lilypond,...). Once validated, this first output will constitute a reference output.
- next and for any change in the library that requires to be checked, run the validation tools again and compare with the reference output: any difference between the different runs will be notified. For any of these differences, you'll have to check them manually, possibly to fix issues with the tools and/or to validate them.
- once a run has been validated, it can become your next reference output.

## Operations

Each run of the validation tools creates a folder named using the version taken from the `libmusicxmlversion.txt` file. You can reuse the same version between different runs, unless you remove the output folder (or part of it). Each time you create a reference output, you should increase the version number in `libmusicxmlversion.txt` (you can add suffixes as well).

### Generating output files
Simply run:
~~~~~~~~~
$ make
~~~~~~~~~
to speed up the generation process, you can parallelize the jobs (using `make` option `-j`).

### Validating the output
Simply run:
~~~~~~~~~
$ make validate VERSION='your_ref_folder_name'
~~~~~~~~~

### Getting help
~~~~~~~~~
$ make help
~~~~~~~~~

### Example

Assuming that the current version in `libmusicxmlversion.txt` is 3.19
~~~~~~~~~
$ make
# on output you'll find a folder named 3.19
... modify the library
... edit libmusicxmlversion.txt and change the version to 3.20
$ make
# on output you'll find a folder named 3.20
$ make validate VERSION=3.19
~~~~~~~~~


## Validation files

The validation tools takes all the `.xml` files from the `files` folder at the root of the repository as input files.
You can temporarily ignore some files using the `xxxpending.txt` files located in this folder:
- lilypending.txt: used to ignore files with `xml2ly`
- braillepending.txt: used to ignore files with `xml2brl`

Of course, ignoring files should just be a temporary solution and the `xxxpending.txt` files are expected to be empty.

**WARNING**
You can freely add new files (or folders) to the `files` folder but you should never move existing files: it breaks the validation process since the file location is critical for the comparison with a previous output.
