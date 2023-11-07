@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');


@mesh = Topaz.Resources.createAsset(type:Topaz.Asset.Type.Mesh, name:'testMesh');

mesh.setVertexCount(newCount:4);
mesh.defineVertices(
    vertices: [
       -1,-1, 0,    0, 1, 0,    0, 0,    1, 0, 1, 1,
        1,-1, 0,    0, 1, 0,    1, 0,    1, 0, 1, 1,
        1, 1, 0,    0, 1, 0,    1, 1,    1, 0, 1, 1,
       -1, 1, 0,    0, 1, 0,    0, 1,    1, 0, 1, 1
    ]
);

mesh.setObjectIndices(
    index: mesh.addObject(),
    indices:[
        0, 1, 2,
        2, 3, 0
    ]
);

@material = Topaz.Resources.createAsset(type:Topaz.Asset.Type.Material, name:'testMaterial');

@:getShader = ::(name) {
    @:asset = Topaz.Resources.createDataAssetFromPath(
        path:name, 
        name:name
    );
    return asset.getAsString();
}

material.setProgramSource(
    language:'GLSL ES2',
    vertexShader  : getShader(name:'vertex_es2.glsl'),             
    fragmentShader: getShader(name:'frag_es2.glsl')
);

material.setProgramSource(
    language:'GLSL',
    vertexShader  :getShader(name:'vertex.glsl'),             
    fragmentShader:getShader(name:'frag.glsl')
);



@tex = Topaz.Resources.createAsset(type:Topaz.Asset.Type.Image, name:'testTex');
tex.resize(width:2, height:2);
tex.addFrame();
tex.setFrameRGBAData(
    frame:0, 
    rgbaData:[
        255,   0,   0, 255,
        0,   255,   0, 255,
        0,     0, 255, 255,
        255, 255, 255, 255


    ]
);



@:createExample3D ::{
    @:this = Topaz.Entity.create(
        attributes : {
            onStep :: {
                stepper += 0.1;
                // special shorthand
                this.setRotation(
                    value : {
                        x:stepper, 
                        y:stepper*3, 
                        z:stepper*8
                    }
                );
            }
        }
    );

    @:v = Topaz.Shape3D.create();
    @stepper = 0;

    v.setMaterial(material);
    v.setMesh(mesh);
    v.setTexture(slot:0, image:tex);

    this.addComponent(component:v);
    
    return this;
}


@a = createExample3D();
a.setPosition(value: {x:0, y:0, z:-4})

@b = createExample3D();
b.setPosition(value:{x:2, y:0, z:1})
b.setScale(value:{x:0.7, y:0.3, z:0.3})


a.attach(child:b);
Topaz.ViewManager.getDefault().setRoot(newRoot:a);
