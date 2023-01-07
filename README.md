# Typhon
Modern statically typed and aot compiled language for the 21st century.


The goal is to hand the developers as many tools as possible to solve the problems at hand.
You should be able to write highly optimized or highly modular code whenever and wherever you want.
Typhon will let "you, do you" without trying to force any dogma or eliteness down your throat.

### Why
#### Languages Before
- ```C``` - builds upon ```ASM``` by providing
  - automatic stack management by function
  - data groups in the form of structs
  
- ```C++``` - builds upon ```C``` by providing :
  - alternatives to composition with inheritance
  - safer structures with constructors, destructors, and raii
  - code generation with templates
  - misc syntax cleanups
  
- ```C#``` - builds upon ```C++``` by providing :
  - dedicated heap allocated types with garbage collection
  - interfaces

#### Lessons Learned
[ TODO ]

## Syntax

### Data Types
- Booleans
  - ```bool``` - 8-bit boolean
  - ```bool32``` - 32-bit boolean
- Unsigned Integers
  - ```u8``` : 8-bit unsigned int
  - ```u16``` : 16-bit unsigned int
  - ```u32``` : 32-bit unsigned int
  - ```u64``` : 64-bit unsigned int
  - ```uword``` : native unsigned int
- Signed Integers
  - ```i8``` : 8-bit signed int
  - ```i16``` : 16-bit signed int
  - ```i32``` : 32-bit signed int
  - ```i64``` : 64-bit signed int
  - ```iword``` : native signed int
- Characters
  - ```char8``` - UTF-8 character
  - ```char16``` - UTF-16 character
  - ```char32``` - UTF-32 character
- Strings
  - ```string8``` - UTF-8 string
  - ```string16``` - UTF-16 string
  - ```string32``` - UTF-32 string

### Access Modifiers
- ```private``` - allows access to the current scope
- ```module``` - allows access to the current file
- ```internal``` - allows access to the current project
- ```protected``` - allows access to derived types
- ```public``` - allows access to all scopes

### Variable Definition
Declared by keyword ```var``` followed by an identifer.
```
var uninitialized : i32;
var auto_deduced_type = 0;
var explicit_type_and_assignment : i32 = 0;
```

### Function Definition
Declared by keyword ```func``` followed by an identifer.
```
func static_function(x : i32) {
  // do stuff here
}

func member_function(self : &type_name) -> return_type {
  // do stuff here
}
```

### Struct Definition
Declared by keyword ```struct``` followed by an identifer.
A struct is a stack allocated structure equivalent to a struct in c++ or c#.
```
struct Vector {
  var x : float;
}
struct Vector2 : public Vector {
  var y : float;
}
struct Vector3 : public Vector2 {
  var z : float;
}
```

### Object Definition
Declared by keyword ```object``` followed by an identifer.
An object is a heap allocated structure equivalent to a class in java or c#.

```
object Window {
  var handle : HWND;
}

func show(self : &Window) -> void { 
  // show window logic here
}
```

### Interface Definition
Declared by keyword ```interface``` followed by an identifer.
```
interface IRunnable {
  func run(self : mut Self&) -> void;
}

object Application implements IRunnable {
  var keep_running : bool;
}

func run(self : mut Application&) {
  while (self.keep_running){
    // do stuff here
  }
}
```

### Aliased Definition
Declared by keyword ```alias``` followed by an identifer.
```
alias ByteArray = u8[];
```
