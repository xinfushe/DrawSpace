
g:signal_rendersceneend("eg")


--planet_transform:release()
--resurgam_planet_config:cleanup()

--resurgam_planet_entity:remove_aspect(TRANSFORM_ASPECT)

--planetmod.view.unload(planet_name)

spaceboxmod.view.unload('spacebox0')
spherebump.view.unload('sphere')


camera2_entity:release_info("referent_body")
eg:remove('camera2_entity')
commons.trash.camera( camera2_entity, camera2_pos)

bellerophon_entity:release_info("entity_name")
bellerophon_entity:remove_aspect(INFOS_ASPECT)
bellerophon.view.unload('ship')

model.releasemaincamera()




text_renderer:release()
text2_renderer:release()
text3_renderer:release()
text4_renderer:release()
text5_renderer:release()

g:remove_appruncb('run')

rg:remove_pass('texture_pass')
commons.trash.final_pass(rg, 'final_pass')

--eg:unset_camera()

rg:update_renderingqueues()
g:release_assets()


g:remove_mousemovecb("onmousemove")
g:remove_keydowncb("keydown")
g:remove_keyupcb("keyup")

g:remove_resourceeventcb("onresourceevent")


gui:show_gui(FALSE)
gui:unload_alllayouts()




g:show_mousecursor(TRUE)
g:set_mousecursorcircularmode(FALSE)

root_entity:release_world()
root_entity:remove_aspect(PHYSICS_ASPECT)

mvt_mod:unload()