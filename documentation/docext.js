var docext_on_input = function() {
    const el = document.getElementById('searchbar');
    const results = document.getElementById('searchresults');

    // clear child nodes
    const children = results.children;
    while(children.length) {
        results.removeChild(children[0]);
    }

    // default output
    const keys = Object.keys(searchIndex);
    for(var i = 0; i < keys.length; ++i) {
        const key = keys[i];
        if ((el.value == '' && searchIndex[keys[i]].mainClass == keys[i]) || 
            (el.value != '' && key.toUpperCase().includes(el.value.toUpperCase()))
            ) {
            const element = document.createElement('div');
            element.innerHTML = '<a href="'+searchIndex[keys[i]].link+'"><div class="navlist-item">'+keys[i]+'</div></a>';
            results.append(element);
        }
    }
}


var docext_generate_banner = function() {
    const el = document.getElementById('navbar');
    for(var i = 0; i < doclinks.length; ++i) {
        const element = document.createElement('a');
        //element.className = 'navitem';
        //element.href = doclinks[i]+'.html';
        //element.innerHTML = "<div>"+doclinks[i]+"</div>";
        element.innerHTML = '<a href="'+doclinks[i]+'.html"><div class="navitem">'+doclinks[i]+'</div></a>';

        el.prepend(element);
    }

}


// populate defaults
docext_on_input();

docext_generate_banner();