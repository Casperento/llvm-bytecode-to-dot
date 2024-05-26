# LLVM - Bytecode to DOT
A LLVM pass that prints bytecode programs into the dot format.

This is a [DCC888 class](https://homepages.dcc.ufmg.br/~fernando/classes/dcc888/) project assignment.

## Description

You must write your own pass, and it must have a few requirements:

- Each instruction must be printed with an opcode;
- All the arguments of an instruction must be printed next to this instruction;
- Arguments that do not have names, such as getelementptr in function calls, should not be printed;
- Type information should not be printed.

## Build

To build this project, you need to run the following shell commands:

*Assuming you have __ninja-build__ installed...*

```bash
$ mkdir -p build
$ cd build
$ cmake -G Ninja -DLLVM_INSTALL_DIR=/path/to/llvm-project/build ..
$ cmake --build .
```

### File Generation

Once you build the project, all source files are generated as follows:

- All tests' sources are compiled into LLVM IR code, using the following command:

```bash
$ clang -S -emit-llvm -o "Filename.c.ll" "Filename.c" -Xclang -disable-O0-optnone
```

- Since the pass is built out-of-tree, the **libBytecodeToDot.so** file is compiled and loaded dynamically as plugin using **opt** by the following command:

```bash
$ opt -load-pass-plugin "build/libBytecodeToDot.so" -passes=mem2reg,lcssa,BytecodeToDotPass -S "FileName.c.ll" -o "FileName.c.ll" 2>&1
```

**Note**: both commands were added as custom commands in the *CMakeLists.txt* file.

Also, every DOT file is generated in the same directory as its relative **.ll** file.

#### PDF

To generate the PDF files from the DOT files, run the following commands:

```bash
$ chmod +x pdf_gen.sh
$ ./pdf_gen.sh
```

The PDF files can be found in the **tests/Stanford** folder.
