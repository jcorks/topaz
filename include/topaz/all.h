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

#ifndef H_TOPAZDC__ALL__INCLUDED
#define H_TOPAZDC__ALL__INCLUDED


// convenience header that imports ALL of the 
// main headers.    

#include <topaz/compat.h>


#include <topaz/assets/image.h>
#include <topaz/assets/data.h>
#include <topaz/assets/sound.h>
#include <topaz/assets/material.h>
#include <topaz/assets/mesh.h>


#include <topaz/backends/audio_manager.h>
#include <topaz/backends/display.h>
#include <topaz/backends/filesystem.h>
#include <topaz/backends/input_manager.h>
#include <topaz/backends/renderer.h>
#include <topaz/backends/script.h>
#include <topaz/backends/time.h>
#include <topaz/backends/iox.h>

#include <topaz/containers/array.h>
#include <topaz/containers/bin.h>
#include <topaz/containers/string.h>
#include <topaz/containers/table.h>

#include <topaz/components/clock.h>
#include <topaz/components/scheduler.h>
#include <topaz/components/shape2d.h>
#include <topaz/components/shape3d.h>
#include <topaz/components/state_control.h>
#include <topaz/components/text2d.h>
#include <topaz/components/automation.h>

#include <topaz/modules/graphics.h>
#include <topaz/modules/input.h>
#include <topaz/modules/view_manager.h>
#include <topaz/modules/script_manager.h>
#include <topaz/modules/font_manager.h>
#include <topaz/modules/graphics.h>
#include <topaz/modules/console.h>
#include <topaz/modules/resources.h>
#include <topaz/modules/audio.h>

#include <topaz/asset.h>
#include <topaz/viewport.h>
#include <topaz/color.h>
#include <topaz/component.h>
#include <topaz/entity.h>
#include <topaz/input_device.h>
#include <topaz/layout.h>
#include <topaz/math.h>
#include <topaz/matrix.h>
#include <topaz/particle.h>
#include <topaz/rbuffer.h>
#include <topaz/refbank.h>
#include <topaz/render2d.h>
#include <topaz/rng.h>
#include <topaz/spatial.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <topaz/transform.h>
#include <topaz/vector.h>
#include <topaz/version.h>
#include <topaz/wbuffer.h>





#endif
