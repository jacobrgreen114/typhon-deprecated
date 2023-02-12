# Typhon Language

<!-- TOC -->
* [Typhon Language](#typhon-language)
    * [Data Types](#data-types)
    * [Constants](#constants)
    * [Variables](#variables)
    * [Functions](#functions)
    * [Structs](#structs)
    * [Classes](#classes)
    * [Objects](#objects)
    * [Concepts](#concepts)
    * [Interfaces](#interfaces)
    * [Aliases](#aliases)
    * [Modifiers](#modifiers)
<!-- TOC -->

### Data Types

> - Booleans
>   - ```bool``` - 8-bit boolean
>   - ```bool32``` - 32-bit boolean
> - Unsigned Integers
    >     - ```u8``` : 8-bit unsigned int
>     - ```u16``` : 16-bit unsigned int
>     - ```u32``` : 32-bit unsigned int
>     - ```u64``` : 64-bit unsigned int
>     - ```uword``` : native unsigned int
> - Signed Integers
    >     - ```i8``` : 8-bit signed int
>     - ```i16``` : 16-bit signed int
>     - ```i32``` : 32-bit signed int
>     - ```i64``` : 64-bit signed int
>     - ```iword``` : native signed int
> - Characters
    >     - ```char``` - UTF-8 character
>     - ```char16``` - UTF-16 character
>     - ```char32``` - UTF-32 character
> - Strings
    >     - ```string``` - UTF-8 string
>     - ```string16``` - UTF-16 string
>     - ```string32``` - UTF-32 string

### Constants

> A ```const``` is a compile time constant variable.
>
> - Equivalance in other languages
    >     - ```constexpr``` variable in C++
>     - ```const``` variable in C#
>
> ```
> const min_width : u32 = 0;
> const max_width = 1920u32;
> ```

### Variables

> A ```var``` is a variable.
>
> ```
> var uninitialized : i32;
> var auto_deduced_type = 0;
> var explicit_type_and_assignment : i32 = 0;
> ```
> 
> All variables are implicitly non-mutable after declaration.
> To allow a variable to be modified after declaration, 
> the keyword [```mut```](#mutability) must be prepended to the declaration
> 
> ``` 
> var non-mutable = 0;
> [mut] var mutable : u32 = 1;
> ```

### Functions

> A ```func``` is a function. In the event of any confusion, please click [**here**](https://stackoverflow.com/).
>
> ```
> func static_function(x : i32) {
>   // do stuff here
> }
> 
> func member_function(self : &type_name) -> return_type {
>   // do stuff here
> }
> ```

### Structs

> A ```struct``` is a stack allocated structure.
>
> **Note** : In future revisions, 
> the ```struct``` concept may be changed to [```class```](#classes) and a c-style ```struct``` may take its place.
> 
> - Equivalance in other languages
>    - ```class``` in C++
>    - ```struct``` in C#
>
> ```
> struct Vector {
>   var x : float;
> }
> struct Vector2 : public Vector {
>   var y : float;
> }
> struct Vector3 : public Vector2 {
>   var z : float;
> }
> ```

### Classes

> A ```class``` is currently a reserved for future language expansion.

### Objects

> An object is a heap allocated structure.
>
> - Equivalance in other languages
>   - ```class``` in C#
>
> ```
> object Window {
>   var handle : HWND;
> }
> 
> func show(self : &Window) -> void { 
>   // show window logic here
> }
> ```

### Concepts

> A ```concept``` is a meta type used in templates to specify a static interface that a type must adhere to.
>
> - Equivalance in other languages
>   - ```concept``` in C++
>
> ```
> concept Iterable {
>   func begin(self : mut Self&) -> ForwardIterator;
>   func end(self : mut Self&) -> ForwardIterator;
> }
> ```

### Interfaces

> An ```interface``` declares a set of virtual functions that must be defined by an object.
>
> - Equivalance in other languages
>   - ```interface``` in C#
>
> ```
> interface IRunnable {
>   func run(self : mut Self&) -> void;
> }
> ```

### Aliases

> An ```alias``` creates a name that references a specific type.
> Useful for creating a simple name for long templated types.
>
> **Note** : ```alias``` keyword may change to keyword ```type``` in future revisions of the language.
>
> - Equivalance in other languages
>   - ```typdef``` in C
>   - ```using``` in C++
>   - ```type``` in rust
>
> ```
> alias ByteArray = u8[];
> ```

### Modifiers
> Modifiers are prepended to declared types in the form of a modifier list.
> 
> **Note** : Modifier lists my get changed to C# style modifiers in future language revisions.
> 
> ```
> [public, static]
> func main() -> i32;
> ```
> 
> #### Mutability
>> 
>> - ```mut``` - allows a variable to be modified after declaration
>
> #### Static
> >- ```static``` - converts a member to a static
>
> 
> #### Access
>> Access modifiers change the scope of code that is allowed to access the declared type.
>> If no access modifier is specified, access is implicitly set to ```private```, unless it is defined at the module scope, 
>> in which case access is implicitly set to ```internal```.
>> 
>> **Note** : 
>>
>> - ```private``` - allows access to the current scope
>> - ```internal``` - allows access to the current project
>> - ```protected``` - allows access to derived types
>> - ```public``` - allows access to all scopes