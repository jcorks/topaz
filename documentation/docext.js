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


// populate defaults
docext_on_input();