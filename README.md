# Typhon

Modern statically typed and aot compiled language for the 21st century.

The goal is to hand developers as many tools as possible to solve the problems at hand.
You should be able to write highly optimized or highly modular code whenever and wherever you want.
Typhon will let "you, do you" without trying to force any dogma or eliteness down your throat.

<!-- TOC -->
* [Typhon](#typhon)
  * [Language](#language)
  * [Development Plan](#development-plan)
<!-- TOC -->

## Language
Information on Typhon's language can be found at : ["**docs/language.md**"](docs/language.md) 

## Development Plan

> Typhon is currently in very early stages of development.
> 
> The frontend from tokenization up to sematic analysis should remain more or less the same as development goes on.
> For the initial version(s) of Typhon, the backend will not reinvent the wheel by utilizing current compilers and build systems.
> This will currently be done by generating C/C++ code and CMake files. 
> This will ultimately allow for the compiler and build system of your choice.
> 
> In future versions, Typhon will most likely generate to something lower like LLVM IR, Assembly, or straight to machine code.
> 
> 
> The current pressing tasks are:
> - [ ] finalize syntax
> - [ ] implement checks (types, mutability, etc.)
> - [ ] finish code generation
> - [ ] implement compilation
>
> 
> #### Full todo list : [**TODO.md**](TODO.md)



