# Nymph Game Engine #
##### 2D Tiled, Chaiscript, and OpenGL Based Game Engine

Welcome to the Nymph Game Engine codebase. This README is in progress. Doxygen generated documentation can be found [here](https://sainteos.github.io/nymph-game-engine-docs/).

Nymph Game Engine is a C++ engine that uses chaiscript for game scripting. It uses tiled maps for levels and sprite animations. It uses a renderer that is OpenGL 3.3/GLSL 3.3 based, currently only 2D graphics are supported (though, I will not rule out that I won't touch 3D one day). It supports loading Tiled .tmx files, including support for lighting, and animations. It uses YSE for sound.

## Dependencies
* [freetype2](https://www.freetype.org/)
* [DevIL](https://github.com/DentonW/DevIL)
* [png](http://www.libpng.org/pub/png/libpng.html)
* [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
* [zlib](https://zlib.net/)
* [tinyxml2](https://github.com/leethomason/tinyxml2) >= 6.0.0
* [tmxparser](https://github.com/sainteos/tmxparser)
* [yse](http://www.attr-x.net/yse/)
* [easylogging++](https://github.com/muflihun/easyloggingpp)
* [glm](https://glm.g-truc.net/0.9.8/index.html)
* [glfw](http://www.glfw.org/)
* [glew](https://github.com/nigels-com/glew)
* [chaiscript](http://chaiscript.com/)

## Build
#### Build Dependencies
* ruby
* lua
* premake4
* GNU make
* gcc/g++ or clang/clang++ (I have successfully built with both)

#### Build Commands
```bash
ruby script_registration_generator.rb    #Generates chaiscript modules for engine classes
premake4 gmake                           #Generates makefile
make                                     #Generates executable
```

## Run
At The moment, this engine is tested with an unnamed game in progress that exists as [nymph-game-one](https://github.com/sainteos/nymph-game-one). This project contains all tmx maps, textures, sounds, etc. (and eventually scripts too). Currently, the [config](https://github.com/sainteos/nymph-game-engine/blob/master/config/default.json) lets you define various input variable including tmx map locations, default starting map, an animation database I use to cache different animations for different characters (this will probably change in the future).

Given all of these things are in place and correctly pointed to in the config file running is as simple as running the executable:

```bash
./TileEngine.out
```

## Sample script file
```chaiscript
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
```

## Config Options
1. `window_title`

    This sets the title of the window.
2. `fullscreen`

    True or false to make the window fullscreen.
3. `active_map`

    The name of the map that should load first. This must be listed within `maps`.
4. `maps`

    A list of strings containing the paths to all of the tmx maps to load.

5. `animation_database`

    This contains all of the possible animations for characters in the game. This is unwieldy and will almost certainly change in the future.

6. `screen_width`

    Sets the width of the screen.

7. `screen_height`

    Sets the height of the screen.

8. `screen_width_tiles`

    This is how many tiles should be distributed and seen across the screen.
9. `screen_height_tiles`

    This is how many tiles should be distribute and seen up and down on the screen.
10. `near_plane`

    Sets the z location of the near plane of the view frustum.

11. `far_plane`

    Sets the z location of the far plane of the view frustum.

12. `camera_x`

    Sets where the camera should start on the x axis.

13. `camera_y`

    Sets where the camera should start on the y axis.

14. `patch_width`

    When tiles are rendered, they are rendered in patches or batches for performance. This defines how many tiles wide that patch is.

15. `patch_height`

    When tiles are rendered, they are rendered in patches or batches for performance. This defines how many tiles wide that patch is.

16. `fonts_location`

    The directory location containing the fonts to be used.

17. `sounds_location`

    The directory location containing the sounds to be used.

18. `pixels_per_unit`

    How many pixels are within 1 unit of font space for truetype.

19. `camera_speed`

    How fast the camera moves by default in tiles per second.

20. `ui_z_slots`

    Sets a discrete number of ui slots to render things within. Lower number = better performance if you have few UI, Higher number = better performance if you have a lot of UI.

21. `save_file`

    The location in which to dump the script memory when save is called. This will be used for saving games.

## Caution
This engine is very much a work in progress. It does a lot at the moment, but it doesn't do enough yet.

## Copyright (or something)
I don't care if you use this, just credit me please. Lots of blood, sweat, and tears have gone into this.
