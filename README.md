# UI-Power-Tools™

UI Power Tools™ is a plugin for Unreal that helps you create User Interfaces. 

It's goals are:
- A better out of the box experience for UI development in Unreal Engine
- Tools and workflows for projects that are creating large amounts of UI

### What does it provide?
- [User Interface Component System (UICS)](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/UICS): COMPLETE
    - [Data](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Data-Screen-Component), [View](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/View-Screen-Component), [Action](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Action-Screen-Component), [Entry](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Entry-Screen-Component) Components: COMPLETE
- [Screen Manager](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Screen-Manager): Partial Implementation
- Screen Templates: Only in Example Project for now
- Composable Widgets: Not Started

See "The Future" for more information on upcoming features

## Status
UI Power Tools v1 is currently in BETA.  You should expect some API Changes between v1 Beta and Release.

[The UIPT Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) has UI screens implemented showing how UIPT works.

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

If you don't want to use the given Screen class, you can make use of the functionality by having your own widget class implement IScreenInterface

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

# FAQ

### Can I use MVVM with UICS?
Yes!  UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, or just plain blueprint to do that. 

### Who made this?
This was created by [Joel Gonzales aka "LowPolyCount"](https://www.lowpolycount.com)

### Why was this made?
I'm tired of having to make a new UI Content Creation System whenever I change jobs. The laziest way to fix this is by making an open source version that can be used at any job.  I'm also terrible at making money.

### Well that's cool. 
That wasn't a question. 

### Sorry!
It's Ok. 

UI Power Tools is a Trademark of Joel Gonzales.

