gendoc = (function(){
    const header = topaz.resources.loadAsset('txt', 'html_header', 'html_header').string;
    const footer = topaz.resources.loadAsset('txt', 'html_footer', 'html_footer').string;

    
    return {
        createDoc : function() {
            return {
                _content : ''+header,
                _level : 0,

                // creates an element string
                createElement : function(content, tag, attribs) {
                    return '<'+tag+(attribs?' '+attribs:'')+'>\n'+content+'\n</'+tag+'>\n';
                },

                // adds text content
                addContent : function(t) {
                    this._content += t;
                },

                write : function(filename) {
                    this._content += footer;
                    const asset = topaz.resources.fetchAsset(topaz.resources.assetType_Data, filename);
                    
                    var bytes = [];
                    for(var i = 0; i < this._content.length; ++i) {
                        bytes.push(this._content.charCodeAt(i));
                    }

                    asset.bytes = bytes;
                    topaz.resources.writeAsset(asset, 'txt', filename);
                }
            }
        }

    }
})();
