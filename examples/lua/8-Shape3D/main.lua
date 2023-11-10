
local mesh = Topaz.Resources.createAsset('testMesh', Topaz.Asset.Type.Mesh);

mesh:setVertexCount(4);
mesh:defineVertices(
    {
       -1,-1, 0,    0, 1, 0,    0, 0,    1, 0, 1, 1,
        1,-1, 0,    0, 1, 0,    1, 0,    1, 0, 1, 1,
        1, 1, 0,    0, 1, 0,    1, 1,    1, 0, 1, 1,
       -1, 1, 0,    0, 1, 0,    0, 1,    1, 0, 1, 1
    }
);

mesh:setObjectIndices(
    mesh:addObject(),
    {
        0, 1, 2,
        2, 3, 0
    }
);

local material = Topaz.Resources.createAsset('testMaterial', Topaz.Asset.Type.Material);

local getShader = function(name)
    local asset = Topaz.Resources.createDataAssetFromPath(
        name, 
        name
    );    
    return asset:getAsString();
end

material:setProgramSource(
    'GLSL ES2',
    getShader('vertex_es2.glsl'),             
    getShader('frag_es2.glsl')
);

material:setProgramSource(
    'GLSL',
    getShader('vertex.glsl'),             
    getShader('frag.glsl')
);



local tex = Topaz.Resources.createAsset('testTex', Topaz.Asset.Type.Image);
tex:resize(2, 2);
tex:addFrame();
tex:setFrameRGBAData(
    0, 
    {
        255,   0,   0, 255,
        0,   255,   0, 255,
        0,     0, 255, 255,
        255, 255, 255, 255
    }
);



local createExample3D = function()
    local this;

    local v = Topaz.Shape3D.create();
    local stepper = 0;


    this = Topaz.Entity.create(
        {
            onStep = function()
                stepper = stepper + 0.1;

                this:setRotation(
                    {
                        x=stepper, 
                        y=stepper*3, 
                        z=stepper*8
                    }
                );
            end
        }
    );


    v:setMaterial(material);
    v:setMesh(mesh);
    v:setTexture(0, tex);

    this:addComponent(v);
    
    return this;
end


local a = createExample3D();
a:setPosition({x=0, y=0, z=-4})

local b = createExample3D();
b:setPosition({x=2, y=0, z=1})
b:setScale({x=0.7, y=0.3, z=0.3})


a:attach(b);
Topaz.ViewManager.getDefault():getViewport():attach(a);
