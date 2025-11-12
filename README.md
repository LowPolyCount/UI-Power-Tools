# UI-Power-Tools
## Make Better UI™

UI Power Tools is a plugin for Unreal Engine to help you make User Interfaces

### What does it provide?
## User Interface Component System (UICS)
UICS is similar to [Entity Component Systems](https://en.wikipedia.org/wiki/Entity_component_system) used in games, but for UI.  It's made for when you are building a large amount of screens and need to share functionality across them.

Provided in the plugin with UICS are the [Data](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Data-Screen-Component), [View](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/View-Screen-Component), [Action](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Action-Screen-Component), and [Entry](https://codeberg.org/) Screen Components

## Screen Manager
The [Screen Manager](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Screen-Manager) is globally accessible and used to display screens.  It allows you to stack screens on top of each other and optionally hide screens below themn when they go on the manager. 

The Screen Manager is Partially Implemented. Usable, but lacks features like transitions and layers.

## Example Project

[The UIPT Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) has several UI screens implemented showing how UIPT works.

The Roadmap section has information on upcoming features

# Installation
[Get a release version](https://codeberg.org/LowPolyCount/UI-Power-Tools/releases) and place in your engine or project plugins folder. Example: YourProject/Plugins/UIPowerTools

In your Project's Build.cs file, add "UIPowerTools" to PublicDependencyModuleNames and then add the following:
...
        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.Add("UIPowerToolsEd");

			if (Target.WithAutomationTests)
			{
				PublicDependencyModuleNames.Add("UIPowerToolsTest");
			}
		}
...
# Setup
### UICS 
You can utilize it in two ways:
1) Use the provided UScreen Class which has UICS already setup:
	- Create a new Widget Blueprint and choose "Screen" as the Parent Class.
	- Open the Widget, Click on Graph View and In the Details panel you'll see "UI Component System" under the Screen category. 
	- From there, you can start adding components to your screen. 

2) Add UICS to a UUserWidget of your choice
	- The Widget will need to inherit the IUICSScreenAccessor interface as well as override some functions. See [ULeaderboardScreen](https://codeberg.org/LowPolyCount/UIPowerTools_Example/src/branch/main/Source/UIPowerTools_Example/UI/LeaderboardScreen.h) in the example project as an example of this.

### ScreenManager
The ScreenManager will let you add a Screen to the viewport and will display screens in the order they are added with the ability to hide screens. The current status of the Screenmanager is that it is usable, but bare as far as features go. 

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

# Status

UI Power Tools v1 is currently in BETA.  There may be minor API Changes between v1 Beta and Release. v1 has been focused on implementing UICS, a bare bones screen manager, and providing examples & documentation of how to use everything. 

# Roadmap

v1 will get the following before leaving beta:
* Complete quality of life enhancements to UICS
* Completed Documentation on UICS and the provided screen components

v2 will feature:
- Completed Screen Manager with Layer, Animation support
- Better logging and debugging
- Screen Viewer Tool showing which screens are on the ScreenManager

After that?
- Screen and Widget Templates
- Focus Display Tool
- Verse Support
- Additional UICS Components
- Composable Widgets?

#License 
UIPT is distributed under [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0)

# FAQ

### Can I use MVVM with UICS?
Yes.  UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, blueprint, or some other method. 

### Who made this?
This was created by [Joel Gonzales | "LowPolyCount"](https://www.lowpolycount.com)

### Why was this made?
I'm tired of having to rewrite the same things whenever I change jobs. The laziest way to fix it is by making an open source version that can be used at any job.  If others find use with this, then all the better. 

### Well that's cool. 
That wasn't a question. 

### Sorry!
It's Ok. 

UI Power Tools is a Trademark of Joel Gonzales.

