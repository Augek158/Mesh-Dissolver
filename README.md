# Mesh-Dissolver
Maya plugin written in C++ using the Maya API.

By: August Ek & Ramin Assadi

### Instructions for quick reloading of plugins
*Method from "Complete Maya Programming" by David A. D. Gould*.

1. Open up Maya and the script editor.
2. Write the following command in the script editor but don't execute! **Note**: `$pluginFile` should be the path to your plugin.

  ```
  {
  string $pluginFile = "/Path/To/Your/Plugin.bundle";
    if (\`pluginInfo -query -loaded $pluginFile\` &&
  	  !\`pluginInfo -query -unloadOk $pluginFile\`) {
  	  file -f -new;
    }
  unloadPlugin Plugin.bundle;
  }
  ```
3. Select all code and drag it to the shelf. Maya will create an icon on the spot of the shelf. This icon will now trigger the command.
4. Repeat but now with the code:

  ```
  {
  string $pluginFile = "/Path/To/Your/Plugin.bundle";
  loadPlugin $pluginFile;
  }
  ```
5. The first shelf button will unload the plugin and the second will load it into Maya. 
