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


#ifndef H_TOPAZDC__IOX__INCLUDED
#define H_TOPAZDC__IOX__INCLUDED

#include <topaz/asset.h>
#include <topaz/backends/api/iox_api.h>
#include <topaz/system.h>



/*

    IO Translator
    -----
    
    The IO Translator (IOX) is responsible for interpreting byte data, translating and processing it 
    to something useful, and taking this useful data to populate an asset or file.



*/

typedef struct topazIOX_t topazIOX_t;







/// Creates a new iox object. The starting path is 
/// the same upon first creating the filsys instance.
///
topazIOX_t * topaz_iox_create(
    topaz_t *, 
    topazSystem_Backend_t *, 
    topazIOXAPI_t
);


/// Destroys and cleans up a iox API
///
void topaz_iox_destroy(topazIOX_t *);


/// Retrieves the backend for this iox object.
///
topazSystem_Backend_t * topaz_iox_get_backend(topazIOX_t *);


/// Returns the API for this iox.
///
topazIOXAPI_t topaz_iox_get_api(topazIOX_t *);


/// Gets the extensions associated with this iox.
///
const topazArray_t * topaz_iox_get_extensions(const topazIOX_t *);

/// Returns the asset type expected for this iox
///
topazAsset_Type topaz_iox_get_asset_type(const topazIOX_t *);


/// Writes the state of the asset to a byte buffer.
/// If an error occurs or the asset is invalid, the returned size 
/// is 0.
/// If the requested extension isnt recognized by the io translator, 
/// the returned size is 0.
///
void * topaz_iox_encode(topazIOX_t *, topazAsset_t *, uint64_t * byteCount, const topazString_t * extension);


/// Streams raw data from memory into an asset all at once. 
/// If successful, returns TRUE. This is equivalent to:
/// topaz_iox_stream_start()
/// topaz_iox_stream(allData)
/// topaz_iox_stream_finish()
///
int topaz_iox_load(topazIOX_t *, topazAsset_t *, const void * dataIn, uint64_t numBytes);




/// Signals to the iox to accept a new asset to begin to decode.
///
void topaz_iox_stream_start(topazIOX_t *, topazAsset_t *);



/// Sets the buffer size. As data is streamed to the asset,
/// data is collected until this threshold is reached. WHen its reahed, 
/// the data is passed to the on_stream callback for further processing.
/// This can be used to control how often data is processed when 
/// posting many individual stream requests. Setting a threshold of 
/// 0 indicates that ever topaz_iox_stream() triggers an
/// on_stream call.
///
void topaz_iox_stream_set_threshold(topazIOX_t *, topazAsset_t *, uint64_t);


/// Called when the asset has been passed streaming data
/// For more flexible performannce, asset streaming is 
/// buffered, meaning not every topaz_iox_stream() call
/// will invoke an on-stream callback. Instead, once a threshold
/// is reached, on_stream will be invoked.
/// See topaz_iox_stream_set_threshold()
/// If a problem occurs, the decoding process may be cancelled. If 
/// such a thing were to happen, FALSE would be returned.
///
int topaz_iox_stream(topazIOX_t *, topazAsset_t *, const void * dataIn, uint64_t byteSize);


/// Flushes the stream buffer. If theres any waiting data 
/// whose size is below the stream threshold, then that 
/// data is immediately sent to the asset via on_stream.
/// If a problem occurs, the decoding process may be cancelled. If 
/// such a thing were to happen, FALSE would be returned.
///
int topaz_iox_stream_flush(topazIOX_t *, topazAsset_t *);



/// Called when the asset has received all bytes.
/// In most cases, this is when the asset will finalize
/// the data. In the case of streaming, this is called
/// after stream_end has been called, signaling the end 
/// of input data.
/// dataIn will is a pointer representing the entire buffer 
/// of data loaded in. dataSizeBytes is the length of that 
/// buffer in bytes.
///
void topaz_iox_stream_finish(topazIOX_t *, topazAsset_t *);

/// Called when the stream is cancelled. If the asset is not currently being 
/// streamed, no action is taken.
///
void topaz_iox_stream_cancel(topazIOX_t *, topazAsset_t *);

/// Returns whether the asset is question it currently being streamed.
///
int topaz_iox_is_streaming(topazIOX_t *, topazAsset_t *);


#endif
