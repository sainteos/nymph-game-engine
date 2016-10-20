# Nymph Game Engine #
2D Chaiscript Based Game Engine

Welcome to the Nymph Game Engine codebase. This README is in progress. Documentation can be found [here](http://sainteos.github.io/nymph-game-engine/).

Nymph Game Engine is a Chaiscript based game engine. It uses a renderer that is OpenGL 3.3/GLSL 3.3 based, currently only 2D graphics are supported (though, I will not rule out that I won't touch 3D one day). It supports loading Tiled .tmx files, including support for lighting, and animations. 

Sample script file:
-------------------

    class MapLoading {
      attr thiss
      attr animation_map
      attr scene_generator
    
      //Available Constructor
      def MapLoading() {
      }
      //Define this function in the class if we need access
      //to the C++ Script Object this pointer.
      //This will usually be used for the event system,
      //To add Observers to Subjects (which the ScriptObject is both)
      def setScriptObjectHandle(handle) {
        this.thiss := handle
      }
    
      //Must define onStart, onUpdate, onDestroy, handleEvent
      def onStart() {
        //Load the animation index, whose location is loaded from config
        //The animation index holds each tagged animation so they are 
        //dynamically callable
        //When a scene is loaded, these animations load into placeholders
        //On the map
        this.animation_map = Map(config.getString("animation_database"))
        
        //Create a scene generator
        //Which is a fancy factory class that will generate scenes
        //From TMX maps
        this.scene_generator = SceneGenerator(this.animation_map, texture_manager, shader_manager)
        
        //Loading a vector of things from config!
        var map_names = config.getStringVector("maps")
    
        for(var i = 0; i < map_names.size(); ++i) {
          var map_path = map_names[i]
          //Load a map
          var map = Map(map_path)
          
          //Load a scene from map. Maps can be loaded with variable sized
          //Patch widths. This is a performance saver!
          var scene = this.scene_generator.createSceneFromMap(config.getInt("patch_width"), config.getInt("patch_height"), map)
          //Available support functions defined in C++
          var name = this.scene_generator.getStrippedMapName(map_path)
          scene.setName(name)
          engine.addScene(scene)
        }
        
        //Notify observers of a LoadMapEvent, loaded from config
        this.thiss.notify(LoadMapEvent_create(config.getString("active_map")))
      }
    
      def onUpdate(delta) {
    
      }
    
      def onDestroy() {
    
      }
    
      def handleEvent(event) {
    
      }
    }

