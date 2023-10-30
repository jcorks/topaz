/*
Copyright (c) 2023, Johnathan Corkery. (jcorkery@umich.edu)
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
#ifndef H_TOPAZDC__LAYOUT__INCLUDED
#define H_TOPAZDC__LAYOUT__INCLUDED


typedef struct topazEntity_t topazEntity_t;
typedef struct topazString_t topazString_t;
typedef struct topazRenderer_Attributes_t topazRenderer_Attributes_t;
typedef struct topaz_t topaz_t;
typedef struct topazAsset_t topazAsset_t;


typedef struct topazLayout_Rectangle_t topazLayout_Rectangle_t;
/// A layout rectangle used for calculation.
///
struct topazLayout_Rectangle_t {
    /// The x position of the rectangle.
    ///
    int x;
    /// The y position of the rectangle.
    ///
    int y;
    /// The width position of the rectangle.
    ///
    int width;
    /// The height position of the rectangle.
    ///
    int height;
};

typedef struct topazLayout_Sizing_t topazLayout_Sizing_t;
/// Contains the style specification for a layout,
/// determining the self and child spacing.
/// Some attributes override others.
/// If any attributes are zero, they are not 
/// prioritized.
///
struct topazLayout_Sizing_t {
    /// Defines the baseline margin, which is the distance from the donated space that this item 
    /// may have its content reside.
    /// This sets the margin as the same value across 
    /// all directions.
    ///
    int margin; 
    
    
    /// The baseline padding, which is the distance from the content space that this item 
    /// may have its child(ren) reside.
    /// This sets the padding as the same value across 
    /// all directions.
    ///
    int padding;
    
    /// Offsets to the baseline margin. 
    /// Applies adjustments to the provided margin
    /// allowing for more dynamic and specific setups. 
    ///
    topazLayout_Rectangle_t marginOffset;

    /// Offsets to the baseline padding. 
    /// Applies adjustments to the provided padding
    /// allowing for more dynamic and specific setups. 
    ///
    topazLayout_Rectangle_t paddingOffset;
    
    /// When children of Horizontal or Vertical layouts, this specifies 
    /// how much space this layout should be donated of available size remaining 
    /// in the parent. This is a number expressed as a fraction [0, 1] of how 
    /// much space it should take in the direction of the layout. When calculated,
    /// this fraction refers to the space available AFTER pixelSizeRequests are 
    /// calculated among siblings.
    ///
    float sizeRequest;
    
    /// When children of Horizontal or Vertical layouts, this specifies
    /// exactly how much pixel space to take up of the parent layout. This is 
    /// expressed as a single number.
    int pixelSizeRequest;       

};


/// Standard function for all layout attribute callbacks
///
typedef void (*topaz_layout_attribute_callback)(
    /// The layout interacting with the callback.
    topazEntity_t * entity, 

    /// The data bound to this callback.
    void * data
);


typedef struct topazLayout_Attributes_t topazLayout_Attributes_t;
/// Callbacks on certain events of the layout.
/// Currently, only one exists.
///
struct topazLayout_Attributes_t {

    /// Function called when the layout has recalculated
    ///
    topaz_layout_attribute_callback   on_recalculate;

    /// When the entity steps normally. Note that this is 
    /// called after the step of the layout.
    ///
    topaz_layout_attribute_callback   on_step;

    /// Custom data
    ///
    void * userData;

};


/// Creates a new layout entity.
/// When given children, it will distribute space among them.
/// The children to be considered must also be layouts.
///
topazEntity_t * topaz_layout_create(
    /// The topaz context.
    topaz_t * context,
    /// The attributes of the layout.
    const topazLayout_Attributes_t * attribs    
);

/// Creates a horizontal layout entity, which distributes 
/// space horizontally.
topazEntity_t * topaz_layout_create_horizontal(
    /// The topaz context.
    topaz_t * context
);

topazEntity_t * topaz_layout_create_vertical(
    /// The topaz context.
    topaz_t * context
);




/// Sets a new set of spacing rules for this Item.
/// If no sizeRequest or pixelSizeRequest is present, 
/// the remaining available space is divided equally among
/// children.
/// For convenience, the entity is returned, allowing for chaining 
/// on creation.
/// 
topazEntity_t * topaz_layout_setup(
    /// The layout to setup.
    ///
    topazEntity_t * layout,

    /// The sizing attributes of the layout.
    ///
    const topazLayout_Sizing_t * sizing
);



/// Establishes a new width and height for the Item,
/// which will modify the space available for use.
/// 
/// This is only needed for toplevel items that do not 
/// have a parent item.
/// 
void topaz_layout_resize(
    /// The layout to resize.
    ///
    topazEntity_t * layout,

    /// The new width.
    ///
    int width,
    
    /// The new height.
    ///
    int height
);


/// Immediately recalculates the space available for this 
/// Item and requests children be recalculated.
/// 
/// Under normal circumstances, this is called for you, but 
/// if changes arent immediate enough, this can be called 
/// by hand.
///
void topaz_layout_recalculate(
    /// The layout to recalculate.
    ///
    topazEntity_t * layout
);

/// Sets the pixel size request. 
/// See topazLayout_Sizing_t for more info.
///
void topaz_layout_set_pixel_size_request(
    /// The layout to modify.
    ///
    topazEntity_t * layout,

    /// The new value.
    ///
    int pixelSizeRequest
);

/// Gets the pixel size request. 
/// See topazLayout_Sizing_t for more info.
///
int topaz_layout_get_pixel_size_request(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);


/// Sets the size request. 
/// See topazLayout_Sizing_t for more info.
///
void topaz_layout_set_size_request(
    /// The layout to modify.
    ///
    topazEntity_t * layout,

    /// The new value.
    ///
    float sizeRequest
);

/// Gets the size request. 
/// See topazLayout_Sizing_t for more info.
///
float topaz_layout_get_size_request(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);

/// Sets the margin offset. 
/// See topazLayout_Sizing_t for more info.
///
void topaz_layout_set_margin_offset(
    /// The layout to modify.
    ///
    topazEntity_t * layout,

    /// The new value.
    ///
    const topazLayout_Rectangle_t * marginOffset
);

/// Gets the margin offset. 
/// See topazLayout_Sizing_t for more info.
///
const topazLayout_Rectangle_t * topaz_layout_get_margin_offset(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);

/// Sets the padding offset. 
/// See topazLayout_Sizing_t for more info.
///
void topaz_layout_set_padding_offset(
    /// The layout to modify.
    ///
    topazEntity_t * layout,
    /// The new value.
    ///
    const topazLayout_Rectangle_t * marginOffset
);

/// Gets the padding offset. 
/// See topazLayout_Sizing_t for more info.
///
const topazLayout_Rectangle_t * topaz_layout_get_padding_offset(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);


/// Gets the space that is allotted for children within the layout.
///
const topazLayout_Rectangle_t * topaz_layout_get_child_space(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);

/// Gets the space that is allotted for the entire layout.
///
const topazLayout_Rectangle_t * topaz_layout_get_content_space(
    /// The layout to query.
    ///
    const topazEntity_t * layout
);

/// Softly requests to recalculate the child space and content space 
/// based on the current position and bounds.
/// The layout is recalculated on next step.
/// 
void topaz_layout_request_recalculate(
    /// The layout to request.
    ///
    topazEntity_t * layout
);


#endif
    


