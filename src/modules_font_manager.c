#include <topaz/modules/font_manager.h>
#include <topaz/backends/font_renderer.h>
#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct topazFontManager_t {
    topaz_t * ctx;
    topazTable_t * registered;
    
    topazFontRendererAPI_t frAPI;
    topazSystem_Backend_t * frBackend;
    
};

topazFontManager_t * topaz_font_manager_create(
    topaz_t * context
) {
    topazFontManager_t * out = calloc(1, sizeof(topazFontManager_t));
    out->ctx = context;
    out->registered = topaz_table_create_hash_topaz_string();    
    
    
    out->frBackend = topaz_system_create_backend(
        topaz_context_get_system(ctx), 
        TOPAZ_STR_CAST("fontRenderer"), 
        &out->frAPI
    );
    

    return out;
}

void topaz_font_manager_destroy(
    topazFontManager_t * fontManager
) {
    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, fontManager->registered);
        !topaz_table_iter_is_end(iter, fontManager->registered);
        topaz_table_iter_proceed(iter, fontManager->registered)) {
        
        topaz_font_renderer_destroy(
            topaz_table_iter_get_value(iter)
        );
    }
    topaz_table_iter_destroy(iter);
    topaz_table_destroy(fontManager->registered);
    free(fontManager);
}


void topaz_font_manager_register_font(
    topazFontManager_t * fontManager,
    topazAsset_t * dataAsset
) {
    topazFontRenderer_t * f = topaz_table_find(
        fontManager->registered, 
        topaz_asset_get_name(
            dataAsset        
        )
    );
    if (f) {
        topaz_font_manager_unregister(
            fontManager, 
            topaz_asset_get_name(
                dataAsset        
            )
        );
    }
    

    f = topaz_font_renderer_create(
        fontManager->ctx,
        fontManager->frBackend,
        fontManager->frAPI
    );    
    
    topaz_table_insert(
        fontManager->registered,
        topaz_asset_get_name(
            dataAsset        
        ),
        f
    );
}




topazFontRenderer_t * topaz_font_manager_get_renderer(
    /// The font manager to query.
    topazFontManager_t * fontManager,
    
    /// The name of the font to get the renderer for.
    const topazString_t * fontName
) {
    return topaz_table_find(fontManager->registered, fontName);
}


topazFontRenderer_t * topaz_font_manager_get_renderer_any(
    /// The font manager to query.
    topazFontManager_t * fontManager
) {
    if (topaz_table_is_empty(fontManager->registered)) return NULL;
    topazTableIter_t * t = topaz_table_iter_create();
    topaz_table_iter_start(t, fontManager->registered);
    topazFontRenderer_t * f = topaz_table_iter_get_value(r);
    topaz_table_iter_destroy(t);
    return f;
}


void topaz_font_manager_preload_glyphs(
    /// The font manager to work with.
    topazFontManager_t * fontManager,
    
    /// The name of the font to preload.
    topazString_t * fontName,
    
    /// The size of the font to preload.
    int sizeRequest,
    
    /// A string of characters to preload.
    const topazString_t * characters
) {
    topazFontRenderer_t * r = topaz_table_find(fontManager->registered, fontName);
    if (!r) return;
    // the ref is never unref'd: it is assumed that 
    // the cached glyphs stay cached until the font is requested to be unregistered
    //
    uint32_t i;
    uint32_t len = topaz_string_get_length(characters);
    for(i = 0; i < len; ++i) {
        topaz_font_renderer_image_ref(
            r,
            topaz_string_get_char(characters),
            sizeRequest            
        );
    }
}



void topaz_font_manager_unregister_font(
    /// The font manager to modify.
    topazFontManager_t * fontManager,
    
    /// The name of the font to remove.
    const topazString_t * fontName
) {

    topazFontRenderer_t * r = topaz_table_find(fontManager->registered, fontName);
    if (!r) return;

    topaz_font_renderer_destroy(r);
    topaz_table_remove(fontManager->registered, fontName);
}

