@:Topaz = import(module:'Topaz');


// This script is not autorun, so another script within topaz must 
// import it. In our case, myScript1.js which is autorun imports this
// and uses the data here.
return ::{
    @:script2data = 42;
    Topaz.log(message:'Hello from myScript2.mt!');    
    return script2data;
}
