STYLE GUIDE
===========

This project will adhere to the following style guidelines:
- 4 spaces in-place of tabs
- All structs shall be typedefed with the project name "topaz" followed by a CamelCaseName of the struct. Its suffix shall be "_t"
- All functions:
	- shall use the project name "topaz" as a prefix to serve as the namespace
	- shall be "verb" oriented in its name
	- shall use snake_case for their symbol names
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


/*
    ExampleClass
    ------------
    
    Description of ExampleClass.

*/
typedef struct topazExampleClass_t topazExampleClass_t;



/// Description of this enum overall
///
typedef enum {
    /// Value A contains this description.
    ///
    topazExampleClas_Enum_ValueA,

    /// Value B contains this description.
    ///
    topazExampleClas_Enum_ValueB

} topazExampleClass_Enum;



/// A naked, open struct used as a helper to this class.
///
typedef struct {
    /// explanation of this member
    ///
    int dataA;

    /// explanation of this member.
    ///
    float dataB;

} topazExampleClass_HelperStruct;







/// Description of the creation function.
///
topazExampleClass_t * topaz_example_class_create();


/// Description of the destruction function.
///
void topaz_example_class_destroy(topazExampleClass_t *);


/// Description of member function
///
int topaz_example_class_do_action(topazExampleClass_t *, int argument);


#endif
```


