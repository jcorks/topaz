/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/


#ifndef H_TOPAZDC__THREAD_API__INCLUDED
#define H_TOPAZDC__THREAD_API__INCLUDED


typedef struct topazFilesys_t topazFilesys_t;

/*

    FilesysAPI
    -----
    
    The set of functions that define how the filesystem abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    An abstraction for filsystems allows for more flexible management 
    of runtime assets, including reading assets from remote sources,
    multithreaded asset loading, ROM devices / files as directory sources,
    as well as traditional / fully-fledged filesystem access.


*/
typedef struct {


    /// Change to the specified directory.
    /// Returns TRUE on success.
    int (*api_filesys_change_dir)(topazFilesys_t *, const topazString_t *);

    /// \brief Change to the specified child directory.
    /// NOTICE: you only need to specify the name of the directory
    /// Ex) if you query the directory and it returns true, then
    /// this function will succeed as well.
    ///	
    int (*api_filesys_go_to_child)(topazFilesys_t *, const topazString_t *);


    /// Change directory to the parent directory.
    ///
    int (*api_filesys_go_to_parent)(topazFilesys_t *);

    /// \brief return the current working directory.
    ///
    topazString_t * (*api_get_cwd)();

    /// \brief Creates a directory with the given name.
    /// The directory is relative to the current working directory.
    ///
    bool (*api_create_dir)(const topazString_t *);

    /// Reads a file within the directory
    ///
    topazIBuffer * (*api_read)(const topazString_t *);


    /// Writes the given file 
    ///
    void (*api_write)(const topazString_t *, const topazOBuffer_t *);


    /// \brief return all the object names in the current working directory as 
    /// an array of topazString_t
    ///  	
    topazArray_t * (*api_query_directory)();


    /// \brief returns whether the given name is a file or note.
    int (*api_is_file)(const topazString_t * t);

} topazFilesysAPI_t;



#endif
