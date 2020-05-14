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


#ifndef H_TOPAZDC__COLOR__INCLUDED
#define H_TOPAZDC__COLOR__INCLUDED

#include <topaz/containers/string.h>
typedef struct topaz_t topaz_t;

/*

    Asset
    -----

    An asset represents an external set of data, usually a separate file,
    that can be used to interface with 

*/

typedef struct topazAsset_t topazAsset_t;

typedef enum {
    topazAsset_Type_None,
    topazAsset_Type_Image,
    topazAsset_Type_Font,
    topazAsset_Type_Audio,
    topazAsset_Type_Model,
    topazAsset_Type_Particle,
    topazAsset_Type_RawData,
    topazAsset_Type_Actor,
    topazAsset_Type_Count,    
} topazAsset_Type;



/// Callback pertinent to data.
/// Success is returned.
///
typedef int (*topaz_asset_data_callback)(
    topazAsset_t * asset, 
    void *         dataIn, 
    uint64_t       dataSizeBytes,  
    void *         userdata
);




typedef struct topazAsset_Attributes_t topazAsset_Attributes_t;

/// Serves as the interface for asset objects
struct topazAsset_Attributes_t {

    /// Called when the asset has received all that bytes.
    /// In most cases, this is when the asset will finalize
    /// the data. In the case of streaming, this is called
    /// after stream_end has been called, signaling the end 
    /// of input data.
    /// dataIn will is a pointer representing the entire buffer 
    /// of data loaded in. dataSizeBytes is the length of that 
    /// buffer in bytes.
    ///
    topaz_asset_data_callback on_load;

    /// Called when the asset has been passed streaming data
    /// For more flexible performannce, asset streaming is 
    /// buffered, meaning not every topaz_asset_stream() call
    /// will invoke an on-stream callback. Instead, once a threshold
    /// is reached, on_stream will be invoked.
    /// See topaz_asset_stream_set_threashold()
    ///
    topaz_asset_data_callback on_stream;

    /// Called when the strream is cancelled.
    ///
    topaz_asset_data_callback on_stream_cancel;

    /// Called when the asset is requested to be destroyed
    ///
    topaz_asset_callback on_unload;


    /// User-provided data.
    ///
    void * userData;
};






/// Creates a new asset.
///
topazAsset_t * topaz_asset_create(topaz_t *, const topazAsset_Attributes_t *);


/// Loads raw data from memory into an asset. 
/// When done, triggers on_load. If successful, returns TRUE.
/// If the asset had already been loaded, on_unload is called first.
///
int topaz_asset_load(topazAsset_t *, const void *, uint64_t numBytes);


/// Begins streaming data.
/// When streaming has started, all topaz_asset_stream_*
/// functions will be enabled.
///
void topaz_asset_stream_start(topazAsset_t *);

/// Streams new data into the asset. This may 
/// trigger an on_stream callback
///
void topaz_asset_stream(topazAsset_t *, const void *, uint64_t numBytes);

/// Sets the buffer size. As data is streamed to the asset,
/// data is collected until this threshold is reached. WHen its reahed, 
/// the data is passed to the on_stream callback for further processing.
/// This can be used to control how often data is processed when 
/// posting many individual stream requests. Setting a threshold of 
/// 0 indicates that ever topaz_asset_stream() triggers an
/// on_stream call.
///
void topaz_asset_stream_set_threshold(topazAsset_t *, uint64_t);

/// Flushes the stream buffer. If theres any waiting data 
/// whose size is below the stream threshold, then that 
/// data is immediately sent to the asset via on_stream.
///
void topaz_asset_stream_flush(topazAsset_t *);

/// Indicates that all the data thats been streamed 
/// represents the entire asset. Once this is called,
/// any remaining data is flushed and on_load is triggered.
///
void topaz_asset_stream_end(topazAsset_t *);

/// Indicates that any streamed data should be thrown out.
void topaz_asset_stream_cancel(topazAsset_t *);



topazString_t * topaz_asset_get_name(const topazAsset_t *);

topazAsset_Type topaz_asset_get_type(const topazAsset_t *);

#endif


