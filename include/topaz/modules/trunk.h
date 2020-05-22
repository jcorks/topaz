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


#ifndef H_TOPAZDC__TRUNK__INCLUDED
#define H_TOPAZDC__TRUNK__INCLUDED


#include <asset.h>
typedef struct topaz_t topaz_t;



/*
    Trunk
    -----
    Manages assets
*/
typedef struct topazTrunk_t topazTrunk_t;




///
topazTrunk_t * topaz_trunk_create(topaz_t *);

///
void topaz_trunk_destroy(topazTrunk_t *);




/// Sets the resource path.
/// The resource path is the root from which 
/// assets may be loaded by path.
/// The default path is system dependent, but is usually 
/// the directory of the binary.
/// If successful, returns TRUE.
///
int topaz_trunk_set_resource_path(topazTrunk_t *, const topazString_t *);


/// Convenience function that attempts to load a new asset 
/// from disk directly. This is recommended for small assets that are 
/// effectively instanteous. If a more robust solution is needed,
/// consider topaz_trunk_fetch_asset().
///
/// Like with topaz_trunk_fetch_asset, if an asset of the given name 
/// already exists, the preloaded asset is returned. If not, 
/// then a new asset is created whos name will match the path given.
/// Then, the data is attempted to be loaded from disk. The name given is 
/// first checked to see if its a partial path relative to 
/// the trunk's resource path (topaz_trunk_set_resource_path()). Then 
/// its interpreted as a full path.
/// If the data could not be inerpreted as the given type, or the source 
/// data is unavailable, NULL is returned.
///
topazAsset_t * topaz_trunk_load_asset(
    topazTrunk_t *,
    const topazString_t * fileType,
    const topazString_t * name
);



/// Either creates a new asset if the given name doesn't exist, or 
/// returns an existing asset.
/// In the case that a new asset needs to be created, 
/// if the fileType is not able to be interpreted, then 
/// NULL is returned. This is only checked if the name doesn't exist.
/// In the case that a new asset is created, the returned asset 
/// can then be used directly to load data. See asset.h
/// 
topazAsset_t * topaz_trunk_fetch_asset(
    topazTrunk_t *,
    const topazString_t * fileType,
    const topazString_t * name
);




/// Returns the list of recognized loading extensions 
///
const topazAsset_t * topaz_trunk_supported_extensions(const topazTrunk_t *);


/// Adds a new possible extension to be read 
/// topazAsset_LoadingProfile_t defines the set of function s
/// to interact with a 
///
void topaz_trunk_add_extension(
    topazTrunk_t *, 
    topazAsset_Type,
    const topazAsset_LoadingProfile_t *
);



#endif
