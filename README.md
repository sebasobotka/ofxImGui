# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

## Upgrade Notes
 - Upgrade to ImGui version 1.54
 - Added ImGuiExtension with Combo and ListBox which can take a `vector<string>` as a parameter (example-imgviewer)
 - Added method for adding fonts ( + .otf) and using polish characters (example-demo)
 - Changed class name Helpers to ImGuiHelpers because of conflict with other addons (ofxCv)
 - Versions 1.50 and higher use the keyword `ofxImGui` as a namespace for all classes in the addon, while previous versions just had a class called `ofxImGui`. If you're upgrading from a previous version, make sure to replace all instances of class `ofxImGui` with `ofxImGui::Gui`.

### Compatibility
Master branch may not be tested on all platforms. See [Releases](https://github.com/jvcleave/ofxImGui/releases/) for more extensively tested versions.

Release versions tested with OF 0.9.x on
 - Raspberry Pi 1 and 2
 - Mac OS 10.11, Xcode 7
 - iOS 9.2 and 8.1.2, Xcode 7
 - Linux Desktop
 - Windows 10, Visual Studio 2015

### Examples

![Screenshot](images/Screenshot.png)

#### example-demo    
Works on Desktop, uses imgui demo windows.

#### example-helpers
Works on Desktop, uses ofxImGui helper functions for interfacing with `ofParameter`.

#### example-ios  
iOS specific with keyboard input helper.
