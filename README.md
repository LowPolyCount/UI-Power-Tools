# UI-Power-Tools™

UI Power Tools™ is a plugin for Unreal that helps you create User Interfaces. 

It's goals are:
- A better out of the box experience for UI development in Unreal Engine
- Tools and workflows for projects that are creating large amounts of UI

### What does it provide?
Currently Implemented is:
- [User Interface Component System (UICS)](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/UICS)
- [Screen Manager](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Screen-Manager)

See "The Future" for planned features

## Status
UI Power Tools is currently in BETA

# Installation
Download the source and place in your engine or project plugins folder. Example: YourProject/Plugins/UIPowerTools

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
You can ask questions and help others on [The UIPT Discord Server](https://discord.gg/nBnvbf8TnU)

# The Future
The following features are planned for development:
- additional UICS Components
- ScreenManager support for multiple layers
- Composable Widgets
- Support features like better logging
- Verse Support

Upcoming tools are:
- A ScreenViewer which will show you Screens on the ScreenManager along with Debug information
- A FocusViewer which can visually show where focus is or was in the widget hierarachy

# FAQ

### Who made this?
This was created by Joel "LowPolyCount" Gonzales.  

### Why was this made?
From LowPolyCount: "I'm tired of having to make a new UI Content Creation System whenever I change jobs. The laziest way to fix this is making an open source version that can be used at multiple jobs.  I'm also terrible at making money."

### Well that's cool. 
That wasn't a question. 

### Sorry. So if there are other questions that get asked often, will they be added here?
Yes, You can expect big updates to this and the other documentation. 


### Anything else?

UI Power Tools is a Trademark of Joel Gonzales.

