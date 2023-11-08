topaz supports 2 built-in languages for scripting: either the ubiquitous 
[Lua](https://www.lua.org) or the new language [Matte](https://jcorks.github.io/matte/doc-quick.html), which 
I developed for more maintainable and stable long-term development.

Scripting vs. C
---------------

Scripting in topaz maps much of the API in C to the scripting language that was compiled with topaz.
There are very few differences outside of omission of lower-level functions that are rarely used 
and cleanup features that aren't needed (as some are transparently handled by the scripting 
garbage collection / resource layer).

Using scripting effectively can significantly accelerate the development cycle of your topaz 
project without sacrificing much of the power that topaz provides. This, of course, comes 
at the cost of resource consumption and possible difficulties with garbage collection. 

It is possible to do further mixing of both C native code and scripting features by providing your own 
bindings using the built-in scripting backend layer API in C (demo coming soon). Doing so 
can balance the performance costs of scripting with the enhanced development time.

Scripting also comes with the benefit of built-in, standardized debugging capabilities provided 
by topaz directly. topaz features a step-wise, GDB-like debugger that utilizes breakpoints and 
in-scope evaluation for determining the source of issues. Because of the nature of scripting, it is 
also possible to recover from many errors that would otherwise be difficult in native C.


Lua vs. Matte
-------------

Both Lua and Matte provide relatively the same feature set (tables / objects with any key types, closures with reference capture,
first-class functions), but differ in a few ways. 

Lua is a tried-and-true go-to for embeddable scripting and features a balance of speed and power. It is much more lightweight
than Matte, and because of its fame, there are many tools out in the wild that can be leveraged in the Lua environment
within topaz. However, some features and differences within Lua can lead to hard-to-debug code, such as its 
relative lenience with undeclared variables, the global scope, and 0 vs 1 indexing, especially in the 
game development realm. 

Matte is a language still in development by me, but is nonetheless able to be used for projects. It features 
more protection against bugs and unintended consequences, but at the cost of more resources and more verbose code.
It is a small language in its feature set, but has classes, JSON support, isolated modules, gradual typing, and other features 
that make it more appropriate for collaboration in the longterm. However, due to me being the sole developer 
of Matte so far, the resource penalty is significant since much of the runtime is not yet mature and needs further
optimization. Despite that, I have found success and using Matte in my own projects.

Either of these choices will be supported in the longterm, so feel free to experiment.
