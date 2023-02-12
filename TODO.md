# Todo

## Language

- [ ] finalize modifier syntax
- [ ] C/C++ interop mechanisms
- [ ] garbage collection maybe???
- [ ] reflection???

## Project Files

- [ ] project file xml schema
- [ ] project file parsing

## Frontend

- [ ] token streaming into parser

### Lexical Analysis

- [ ] keyword const
- [ ] keyword alias
- [ ] keyword null

### Syntax Analysis

- [x] variable parsing
- [x] function parsing
- [x] struct parsing
- [ ] object parsing
- [ ] concept parsing
- [ ] interface parsing
- [ ] alias parsing
- [ ] template parsing
- [ ] array parsing
- [ ] pointer / reference parsing
- [ ] modifier parsing
- [ ] inheritance parsing

### Semantic Analysis

- [ ] link identifiers
- [ ] auto type deduction
- [ ] type checking
- [ ] mutability checks
- [ ] access checks

## Backend

### Generation

- [x] source file
- [ ] internal header
- [ ] public symbol tree
- [ ] determine which file to place declarations based on access modifiers
- [ ] private symbol obfuscation

### Compilation

- [ ] compile commands
- [ ] project linking
- [ ] project public package mechanism

## Libraries

### Core

Bare metal standard library

- [ ] primative types

### Std

Standard library with OS Support

- [ ] String
- [ ] Text Support
    - [ ] Readers
    - [ ] Writers
    - [ ] Encoding Conversion
- [ ] Console Support
    - [ ] In
    - [ ] Out
    - [ ] Err
- [ ] File Support
- [ ] Memory Support
- [ ] Network Support
    - [ ] Socket
    - [ ] UdpSocket
    - [ ] TcpSocket
        - [ ] Client
        - [ ] Server
    - [ ] UnixSocket
- [ ] Serialization Support
    - [ ] Xml
    - [ ] Json

### Std.Ui

Standard library for UI Development

- [ ] Window
- [ ] Controls
    - [ ] Label
    - [ ] Button
    - [ ] TextBox
    - [ ] Radial
    - [ ] ScrollBar
    - [ ] MenuBar
        - [ ] MenuItem
    - [ ] List
    - [ ] Table
- [ ] Panels
    - [ ] StackPanel
    - [ ] GridPanel
    - [ ] DockPanel

#### Collections

- [ ] list
- [ ] linked list
- [ ] map
- [ ] set