
FALSE=0
TRUE=1

PI=3.1415927

-- physics

GRAVITY_ENABLED=1
GRAVITY_DISABLED=0

SHAPE_BOX=0
SHAPE_SPHERE=1
SHAPE_MESHE=2

BODY_MODE=1
COLLIDER_MODE=2
ATTRACTOR_COLLIDER_MODE=3

-- type aspect
BODY_ASPECT=0
CAMERA_ASPECT=1
PHYSICS_ASPECT=2
RENDERING_ASPECT=3
SERVICE_ASPECT=4
TIME_ASPECT=5
TRANSFORM_ASPECT=6
INFOS_ASPECT=7
RESOURCES_ASPECT=8
ANIMATION_ASPECT=9

-- args loading shaders
SHADER_COMPILED=1
SHADER_NOT_COMPILED=0

-- time scale
NORMAL_TIME=0
MUL2_TIME=1
MUL4_TIME=2
MUL10_TIME=3
MUL100_TIME=4
MUL500_TIME=5
SEC_1HOUR_TIME=6
SEC_1DAY_TIME=7
SEC_30DAYS_TIME=8
SEC_1YEAR_TIME=9
DIV2_TIME=10
DIV4_TIME=11
DIV10_TIME=12
FREEZE=13

-- render purpose
RENDERPURPOSE_COLOR=0
RENDERPURPOSE_FLOAT=1
RENDERPURPOSE_FLOAT32=2
RENDERPURPOSE_FLOATVECTOR=3

-- render target
RENDERTARGET_GPU=0
RENDERTARGET_CPU=1

NO_TEXTURESTAGE_CONNECTION=-1

renderer=Renderer()
rg=RenderPassNodeGraph('rg')
rg:create_root('final_pass')
rg:update_renderingqueues()
    

eg=EntityNodeGraph('eg')
root_entity=Entity()

root_entity:add_aspect(RENDERING_ASPECT)
root_entity:add_aspect(TIME_ASPECT)
root_entity:configure_timemanager(NORMAL_TIME)

root_entity:add_aspect(INFOS_ASPECT)
root_entity:setup_info( "entity_name", "root_entity" )



root_entity:connect_renderingaspect_rendergraph(rg)
eg:set_root('root', root_entity )


-- creation cote lua de l'enum RenderState::Operation
RENDERSTATE_OPE_NONE=0
RENDERSTATE_OPE_SETCULLING=1
RENDERSTATE_OPE_ENABLEZBUFFER=2
RENDERSTATE_OPE_SETTEXTUREFILTERTYPE=3
RENDERSTATE_OPE_SETVERTEXTEXTUREFILTERTYPE=4
RENDERSTATE_OPE_SETFILLMODE=5
RENDERSTATE_OPE_ALPHABLENDENABLE=6
RENDERSTATE_OPE_ALPHABLENDOP=7
RENDERSTATE_OPE_ALPHABLENDFUNC=8
RENDERSTATE_OPE_ALPHABLENDDEST=9
RENDERSTATE_OPE_ALPHABLENDSRC=10


-- log levels
TRACE=0
DEBUG=1
WARN=2
ERROR=3
FATAL=4

GLOBALE_FORCE = 0
LOCALE_FORCE = 1


NORMALES_DISCARDED = 0
NORMALES_AUTO = 1
NORMALES_AUTO_SMOOTH = 2
NORMALES_FROMLOADER = 3
NORMALES_FROMLOADER_SMOOTH = 4
NORMALES_COMPUTED = 5


TB_DISCARDED = 0
TB_AUTO = 1
TB_FROMLOADER = 2
TB_COMPUTED = 3

-- animations event
ANIMATION_BEGIN = 0
ANIMATION_END = 1

-- resources event
BLOB_LOAD = 0
BLOB_LOADED = 1
ASSET_SETLOADEDBLOB = 2
SHADERCACHE_CREATION = 3
SHADER_COMPILATION = 4
SHADER_COMPILED = 5

RENDERINGQUEUE_UPDATED = 0
RENDERINGQUEUE_PASS_BEGIN = 1
RENDERINGQUEUE_PASS_END = 2

TRANSFORMATION_WORLD_MATRIX = 0
TRANSFORMATION_VIEW_MATRIX = 1
TRANSFORMATION_PROJ_MATRIX = 2


print_memsize=function() 
	g:print('Total mem = '..g:total_mem()..' byte(s)') 
end
