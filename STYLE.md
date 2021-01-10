STYLE GUIDE
===========

This project will adhere to the following style guidelines:
- 4 spaces in-place of tabs
- All structs shall be typedefed with the project name "topaz" followed by a CamelCaseName of the struct. Its suffix shall be "_t"
- All functions:
	- shall use the project name "topaz" as a prefix to serve as the namespace
	- shall be "verb" oriented in its name
	- shall use snake_case for their symbol names
    - shall have its declaration be split across multiple lines if the argument list contains arguments
    - shall have each argument commented and named
    - shall have function pointer arguments named
- All class-based structs of the public user interface shall:
	- be opaque 
	- uniquely defined within a header bearing the same name as the struct, effectively representing a "class"
	- utilize "member functions" as a function whose first argument is a pointer to the opaque class object
	- shall have a _create() and a _destroy()





Example function:
```
int topaz_my_function(int argument) {
    return argument+1;
}

```


Example switch: 
```
switch(value) {
  case CASE_1:
    handle_logic();
    do = something+else;
    break;

  case CASE_2:
    handle_other();
    break;
}
```


Example if-else:
```
if (value == 2) {
    something();
} else if (value == 2) {
    something_else();
} else {
    neither();
}
```


Example header (all headers will employ the opaque class pattern):
```
#ifndef H_TOPAZDC__EXAMPLE_CLASS__INCLUDED
#define H_TOPAZDC__EXAMPLE_CLASS__INCLUDED

#include <topaz/dependencies.h>



/// Description of ExampleClass.
///
typedef struct topazExampleClass_t topazExampleClass_t;



/// Description of this enum overall
///
typedef enum topazExampleClass_Enum topazExampleClass_Enum
enum topazExampleClass_Enum {
    /// Value A contains this description.
    ///
    topazExampleClas_Enum_ValueA,

    /// Value B contains this description.
    ///
    topazExampleClas_Enum_ValueB
}


typedef struct topazExampleClass_HelperOpenStruct topazExampleClass_HelperOpenStruct

/// A naked, open struct used as a helper to this class.
///
struct topazExampleClass_HelperOpenStruct {
    /// explanation of this member
    ///
    int dataA;

    /// explanation of this member.
    ///
    float dataB;
};





/// Description of the creation function.
///
topazExampleClass_t * topaz_example_class_create();


/// Description of the destruction function.
///
void topaz_example_class_destroy(

    /// Comment on the argument
    topazExampleClass_t *
    
);


/// Description of member function
///
int topaz_example_class_do_action(

    /// Comment on the argument
    topazExampleClass_t *, 

    /// Comment on the argument
    int argument
);


/* 

    ExampleClass_Helper
    -----

    A second helper class that shares its implementation with 
    the main class.

*/

typedef struct topazExampleClass_Helper_t topazExampleClass_Helper_t;

/// Description of the creation function.
///
topazExampleClass_Helper_t * topaz_example_class_helper_create(

    /// Comment on the argument    
    topazExampleClass_t *
);


/// Description of the destruction function.
///
void topaz_example_class_helper_destroy(

    /// Comment on the argument    
    topazExampleClass_Helper_t *    
);




#endif
```


