# UI-Power-Tools™

UI Power Tools™ is a plugin for Unreal that helps you create User Interfaces. 

It's goals are:
- A better out of the box experience for UI development in Unreal Engine
- Tools and workflows for projects that are creating large amounts of UI

### What does it provide?
## User Interface Component System
UICS is similar to [Entity Component Systems](https://en.wikipedia.org/wiki/Entity_component_system) used in games, but for UI.  It's made for when you are building a large amount of screens and need to share functionality across them.

Provided in the plugin with UICS are the [Data](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Data-Screen-Component), [View](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/View-Screen-Component), [Action](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Action-Screen-Component), and [Entry](https://codeberg.org/) Screen Components

## Screen Manager
The [Screen Manager](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Screen-Manager) is globally accessible and used to display screens.  It allows you to stack screens on top of each other and optionally hide screens below themn when they go on the manager. 

The Screen Manager is Partially Implemented. Usable, but lacks features like transitions and layers.

See "The Future" for more information on upcoming features

## Example Project

[The UIPT Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) has several UI screens implemented showing how UIPT works.

# Status
UI Power Tools v1 is currently in BETA.  There may be minor API Changes between v1 Beta and Release.

# Installation
[Get a release version](https://codeberg.org/LowPolyCount/UI-Power-Tools/releases) and place in your engine or project plugins folder. Example: YourProject/Plugins/UIPowerTools

In your Project's Build.cs file, add "UIPowerTools" to PublicDependencyModuleNames and then add the following:

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.Add("UIPowerToolsEd");

			// test requires editor due to tests using AutomationEditorCommon
			if (Target.WithAutomationTests)
			{
				PublicDependencyModuleNames.Add("UIPowerToolsTest");
			}
		}

# Setup
### UICS 
The plugin gives you a class named "Screen" which works as the base for any UI you create. It derives from UCommonActivatableWidget
- Create a new Widget Blueprint and choose "Screen" as the Parent Class.
- Open the Widget, Click on Graph View and In the Details panel you'll see "UI Component System" under the Screen category. 
- From there, you can start adding components to your screen. 

If you don't want to use the given Screen class, you can make use of the functionality by having your own widget class implement IScreenInterface.

### ScreenManager
The ScreenManager gives you a way to easily add a Screen to the viewport and the order in which they are drawn. 

To setup the ScreenManager:
- Create a New Blueprint Class and Choose HudActor as the parent class
- Open the Blueprint you created. Under the "Screens to Display at Start" property, you can add any screens that you would like to be created and put on the ScreenManager when the HudActor's BeginPlay() is called.
- Open any gamemodes you want to use the ScreenManager and you Set the HUD Class to use the Blueprint you created. You will likely want to create other Blueprints for your different gamemodes.

In Blueprint, you can access the ScreenManager by calling the ScreenManager node which is global. 
In C++, you can call the static function UScreenManager::Get(UObject* WorldContextObject)

You can remove a Screen by calling it's Close() function.

# Help & Learning
### Wiki
[The Wiki](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki) has information on how to use UI Power Tools. This is in it's beginning stages, we're continually working on it.

### Example Project
The [UI Power Tools Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) has examples of how to use UICS to build screens and use the Screen Manager. 

### Discord
You can ask questions on [The UIPT Discord Server](https://discord.gg/nBnvbf8TnU)

# The Future
The following features are planned for development:
- Additional UICS Components
- Composable Widgets
- ScreenManager support for multiple layers and animations
- Better logging and debugging tools
- Screen Viewer Tool showing which screens are on the ScreenManager
- Screen and Widget Templates
- Focus Display Tool
- Verse Support

#License 
UIPT is distributed under [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0)

# FAQ

### Can I use MVVM with UICS?
Yes!  UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, blueprint, or some other method. 

### Who made this?
This was created by [Joel Gonzales | "LowPolyCount"](https://www.lowpolycount.com)

### Why was this made?
I'm tired of having to rewrite the same things whenever I change jobs. The laziest way to fix it is by making an open source version that can be used at any job.  I'm also terrible at making money.

### Well that's cool. 
That wasn't a question. 

### Sorry!
It's Ok. 

UI Power Tools is a Trademark of Joel Gonzales.

