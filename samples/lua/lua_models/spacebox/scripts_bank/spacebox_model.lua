
spaceboxmod = {}
spaceboxmod.dump = {}
spaceboxmod.view = {}

spaceboxmod.module = Module("skyboxmod", "spacebox")
spaceboxmod.module:load()
g:print(spaceboxmod.module:get_descr().. ' loaded')

-- stockage des instances modeles : paire {entity, renderer, body}
spaceboxmod.models = {}

spaceboxmod.layers =
{
	layer_0 = 
	{
		layer0_rendering =	
		{
			fx =
			{
				shaders = 
				{
					{ path='texture_vs.cso',mode=SHADER_COMPILED },
					{ path='texture_ps.cso',mode=SHADER_COMPILED }
				},
				rs_in = 
				{
					{ ope=RENDERSTATE_OPE_ENABLEZBUFFER, value="false" }
				},
				rs_out =
				{
					{ ope=RENDERSTATE_OPE_ENABLEZBUFFER, value="false" }
				}
			},
			textures =
			{
				[1] = 
				{
					{path='neb_front5.png', stage=0}
				},
				[2] = 
				{
					{path='neb_back6.png', stage=0}
				},
				[3] = 
				{
					{path='neb_left2.png', stage=0}
				},
				[4] = 
				{
					{path='neb_right1.png', stage=0}
				},
				[5] = 
				{
					{path='neb_top3.png', stage=0}
				},
				[6] = 
				{
					{path='neb_bottom4.png', stage=0}
				}
			},
			vertex_textures =
			{
			},
			shaders_params = 
			{
			},
			rendering_order = 10
		}
	}
}

spaceboxmod.createmodelview = function(p_rendergraph, p_entitygraph, p_entity_id, p_passes_bindings, p_parent_entity_id)

  local entity
  local renderer

  entity, renderer=commons.create_rendering_from_module(spaceboxmod.layers, spaceboxmod.module, "skyboxRender", p_passes_bindings)
  renderer:register_to_rendering(p_rendergraph)


  p_entitygraph:add_child(p_parent_entity_id,p_entity_id,entity)

  entity:add_aspect(TRANSFORM_ASPECT)

  local pair = { ['entity'] = entity, ['renderer'] = renderer, ['body'] = body }
  spaceboxmod.models[p_entity_id] = pair

  return entity
  
end

spaceboxmod.trashmodelview = function(p_rendergraph, p_entitygraph, p_entity_id)

  local entity = spaceboxmod.models[p_entity_id]['entity']
  local renderer = spaceboxmod.models[p_entity_id]['renderer']

  entity:remove_aspect(TRANSFORM_ASPECT)

  commons.trash.rendering(rg, spaceboxmod.module, entity, renderer)
  p_entitygraph:remove(p_entity_id)

  local pair = spaceboxmod.models[p_entity_id]
  pair['entity'] = nil
  pair['renderer'] = nil

  spaceboxmod.models[p_entity_id] = nil

end


spaceboxmod.view.unload = function(p_entity_id)
 
  local found_id = FALSE
  for k, v in pairs(spaceboxmod.models) do

    if k == p_entity_id then
	  found_id = TRUE
	end
  end

  if found_id == TRUE then
     model.view.unload(spaceboxmod.trashmodelview, p_entity_id)
  else
    g:print('Unknown entity '..p_entity_id)
  end
end

spaceboxmod.view.load = function(p_entity_id, p_passes_config, p_parent_entity_id)

  local found_id = FALSE
  for k, v in pairs(spaceboxmod.models) do

    if k == p_entity_id then
	  found_id = TRUE
	end
  end

  if found_id == TRUE then
    g:print('Entity '..p_entity_id..' already exists')
  else
    model.view.load('spacebox model', spaceboxmod.createmodelview, p_passes_config, nil, {x = 1000.0, y = 1000.0, z = 1000.0}, p_entity_id, p_parent_entity_id)
  end  
end