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


#ifndef H_TOPAZDC__DECODER_API__INCLUDED
#define H_TOPAZDC__DECODER_API__INCLUDED

#include <topaz/asset.h>
typedef struct topaz_t topaz_t;
typedef struct topazDecoder_t topazDecoder_t;
typedef struct topazArray_t topazArray_t;

/*

    DecoderAPI
    -----
    
    The set of functions that define how the decoder abstraction should 
    behave.

    These API functions are called as underlying implementations for someof the symbols 
    within <topaz/backends/decoder.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazDecoderAPI_t topazDecoderAPI_t;





/// Each function is an implementation-facing copy of 
/// the user-side API for topazDecoder_t. See <topaz/backends/decoder.h>
///
struct topazDecoderAPI_t {
    /// Creates a decoder.
    ///
    void * (*decoder_create) (
        topazDecoder_t *, 
        topaz_t *, 
        /// "extension" are the extensions to associate with 
        /// this decoder. The implementation should populate 
        /// this array with new topazString_t * instances 
        /// that represent the extension to map to this 
        /// decoder.
        topazArray_t * extensions,

        /// "type" is the type that this decoder 
        /// assumes and uses throughout to populate the asset with useful data.
        /// The type essentially determines which subclass of topazAsset_t will 
        /// be used within the decoder implementation.
        topazAsset_Type * type,    

        /// This is the recommended buffer size to use when buffering streams.
        ///
        uint64_t * recommendedBufferSize
    );

    /// Destroys the decoder. All active streams will be cancelled by the decoder 
    /// before calling this destructor.
    ///
    void (*decoder_destroy) (topazDecoder_t *, void *);

    /// Starts the decoding process for a specific asset. The decoder may generate 
    /// data specific to this asset to assist in the loading process. This data 
    /// will be passed to all the decoder_stream_* family of functions as "streamData"
    ///
    void * (*decoder_stream_start) (topazDecoder_t *, void *, topazAsset_t *);

    /// Adds data to the stream. Its the decoder's responsibility to accumulate 
    /// data or not. Since some assets work on a stream basis and dont necessarily 
    /// require all data at once, it's left up to the implementation to make that 
    /// decision. The return value dictates if the stream operation is valid. If 
    /// 0 is returned, the decoder stream will end and decoder_stream_cancel will be called,
    /// ending the stream process.
    /// If a stream is cancelled within this call using the user-facing cancel function,
    /// undefined behavior will occur.
    ///
    int (*decoder_stream) (topazDecoder_t *, void *, topazAsset_t *, void * streamData, const void * data, uint64_t dataIn);


    /// Called when either the program request cancellation of stream OR if a streaming operation 
    /// fails partway through (one reason could be invalid data). In the case that 
    /// it's from an invalid stream, fromFailed will be true. 
    ///
    void (*decoder_stream_cancel) (topazDecoder_t *, void *, topazAsset_t *, void * streamData, int fromFailed);

    /// Called when all data for an asset has been streamed. At this point, the decoder implementation 
    /// can finalize any asset data.
    ///
    int (*decoder_stream_finish) (topazDecoder_t *, void *, topazAsset_t *, void * streamData);

};



#endif
