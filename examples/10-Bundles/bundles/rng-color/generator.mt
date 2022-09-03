// This is the script that handles making the colors
@:Topaz    = import(module:'Topaz');
@:settings = import(module:'RNG-Color.Settings');

// We want different results each time we run, so 
// we will seed the RNG from the current time.
settings.rngSeed = String(from:Topaz.time);


// Gets a weighted rng value from the min / max.
@:getRngValue :: {
    return settings.min + settings.nextValue * (settings.max - settings.min);
};

// Note that here a function is returned to generate the color.
return ::{
    return {
        r : getRngValue(),
        g : getRngValue(),
        b : getRngValue(),
        a : 1
    };
};
