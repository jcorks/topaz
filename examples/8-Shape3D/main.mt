@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');


@mesh = Topaz.Resources.createAsset(type:Topaz.Asset.TYPE.MESH, name:'testMesh');
mesh.vertices = [
   -1,-1, 0,    0, 1, 0,    0, 0,    1, 0, 1, 1,
    1,-1, 0,    0, 1, 0,    1, 0,    1, 0, 1, 1,
    1, 1, 0,    0, 1, 0,    1, 1,    1, 0, 1, 1,
   -1, 1, 0,    0, 1, 0,    0, 1,    1, 0, 1, 1
];

mesh.addObject(
    indices:[
        0, 1, 2,
        2, 3, 0
    ]
);

@material = Topaz.Resources.createAsset(type:Topaz.Asset.TYPE.MATERIAL, name:'testMaterial');
material.setProgramSource(
    language:'GLSL ES2',
    vertexShader  :Topaz.Resources.createAsset(path:'vertex_es2.glsl', name:'vertex_es2.glsl').string,             
    fragmentShader:Topaz.Resources.createAsset(path:'frag_es2.glsl',   name:'frag_es2.glsl').string
);

material.setProgramSource(
    language:'GLSL',
    vertexShader  :Topaz.Resources.createAsset(path:'vertex.glsl', name:'vertex.glsl').string,             
    fragmentShader:Topaz.Resources.createAsset(path:'frag.glsl',   name:'frag.glsl').string
);



@tex = Topaz.Resources.createAsset(type:Topaz.Asset.TYPE.IMAGE, name:'testTex');
tex.resize(width:2, height:2);
tex.addFrame();
tex.setRGBA(
    index:0, 
    rgbaData:[
        255,   0,   0, 255,
        0,   255,   0, 255,
        0,     0, 255, 255,
        255, 255, 255, 255


    ]
);



@:Example3D = class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @:v = Topaz.Shape3D.new();
        @stepper = 0;

        this.constructor = ::{
            v.material = material;
            v.mesh = mesh;
            v.setTexture(slot:0, texture:tex);
            v.setAttributes(
                textureFilterHint: Topaz.RENDERER.TEXTURE_FILTER_HINT.NONE
            );

            

            this.components = [v];


            this.onStep = ::{
                stepper += 0.1;
                // special shorthand
                this.rotation = {
                    x:stepper, 
                    y:stepper*3, 
                    z:stepper*8
                }
            }
        }
    }
);


@a = Example3D.new();
a.position = {x:0, y:0, z:-4}

@b = Example3D.new();
b.position = {x:2, y:0, z:1}
b.scale = {x:0.7, y:0.3, z:0.3}



a.children = [b];
Topaz.defaultDisplay.root = a;
