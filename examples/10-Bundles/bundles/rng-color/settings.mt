// Settings to globally control how RNG is handled for the colors

@:Topaz  = import(module:'Topaz');
@:class  = import(module:'Matte.Core.Class');
@:assert = import(module:'Assertion.Test');


// Here we actually return an instance to the class anonymously.
// Note the .new() at the end.
return class(
    name : 'RNG-Color.Settings',
    define :::(this) {
        @:rng = Topaz.RNG.new();
        @rngMin = 0.3;
        @rngMax = 0.8;
    
    
        this.interface = {
        
            // Optionally, a user can set a global seed.
            rngSeed : {
                set ::(value => String) {
                    rng.seed = value;
                }
            },
            
            // We can set a minimum value for rng values.
            min : {
                set ::(value => Number) {
                    // here we use the depency "assertion" bundle to access 
                    // the standard assert function
                    assert(condition:value >= 0 && value <= 1);
                    rngMin = value;
                },
                
                get ::<- rngMin
            },

            // We can set a max value for rng values.            
            max : {
                set ::(value => Number) {
                    assert(condition:value >= 0 && value <= 1);
                    rngMax = value;
                },
                
                get ::<- rngMax
            },

            // Accesses the next rng value.            
            nextValue : {
                get :: {
                    return rng.value;
                }
            }
        };
    }
).new();

