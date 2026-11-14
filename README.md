UICS is a plugin for Unreal Engine that helps you Make User Interfaces with less code, and the code you do write more shareable across your UI. It gives you Screen Components that let you build screens out of reusable parts. UICS is similar to [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) which is widely used in games. 

## Usecases
### Removing Boilerplace
UICS reduces the amount of boilerplace code that Unreal requires. It does this by giving you Screen Components that are data driven to handle the tasks you do the most: Getting Data, Displaying that Data, and doing actions in response to the user. 

### Large Games
UICS Gives you a workflow that allows you to share the code you need to write.  The bigger the project, the more cases where code can be shared. 

### Shared Resources
Widgets get access to all the components on their screen, it makes it easier to handle shared resources and ata. If multiple widgets need to know when a character is loaded, you can create a component containing that functionality. Then your widgets can easily access the results instead of passing data through multiple layers of widgets. 


## Example of Use
As an example, you're creating a store that the player can buy items from. Here is how ethe provided components help you:
* [Data Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Data-Screen-Component) provides the structure for building an array of data where each entry would represent an item. It provides the structure for you retrieve all the items for the shop, and then gives options for filtering, so you could remove all items the player has bought.
* [View Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/View-Screen-Component) takes the data array and creates display widgets and gives them their data. It works with almost any panel, has events for when a widget's focus changes, it's selection changes, and more. 
* [Action Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Action-Screen-Component) will do do an action such as buying an item, in response to a user action on a display widget such as button press, hover-over, etc.  The Display Widgets can also ask it if the action is possible, so if a user doesn't have enough money to buy an item, you could grey it out.

These components work together if they are linked, so once Data Component has retrieved it's array of data, the View Component would automatically take that and create the widgets for you, no code required. 

# Installation
- If it doesn't exist, create a Plugins directory in your project
- [Download a release version](https://codeberg.org/LowPolyCount/UI-Power-Tools/releases) and unzip to the plugins directory
- Restart your project and it will prompt you to build the plugin.  

# Quick Start Guide

# Help & Learning
### Tutorials
- If you want to see what UICS is capable of, follow [A Step-by-Step Tutorial on how to use UICS to Create a Screen](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Tutorial-Using-Data%2C-View%2C-Action-Components)

### Example Project
- [The UICS Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) is where UICS is developed and has many UI screens showing how UICS works.

### Wiki
- [The Wiki](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki) has information on how to use UICS.

# Status

UICS is stable meaning it can be used in production code.  It has not yet hit v1 meaning there may be API changes. [View remaining v1 issues to resolve](https://codeberg.org/LowPolyCount/UI-Power-Tools/milestone/55170)

# License 
- Licensed under MIT License

# FAQ
### Can I use MVVM with UICS?
Yes, UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, blueprint, slate, or whatever method you want.

### What if I find an issue? 
Please [Make an Issue](https://codeberg.org/LowPolyCount/UI-Power-Tools/issues/new). You can use your github account to login to Codeberg. 

### Where can I find help?
[The Wiki](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki) has information that may help or you can ask in [The Discord Server](https://discord.gg/nBnvbf8TnU)

### Can Screen Components be used as the Widget Components that have started to show up in Unreal?
No, right now Unreal's built in Widget Components have some limitations that Screen Components don't:
* They can't be used as defaults on a widget
* They don't tell you when Pre-Construct, Construct, Destruct events have happened on their widget.
* They can't provide Blueprint Compiler Warnings/Errors
* A lot of work is required to get their events to show in the editor

While Screen Components can only be used on Screens, I am investigating how they could be expanded to be used on every widget, but that is not a high priority. 

### What if I want to use this with an existing codebase?
While We provide Base Classes that let you use this plugin "Out of the Box", we do let you override these classes with (hopefully) minimal effort. See [Overriding Provided Classes](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/UIPT-Overriding-Provided-Classes)

### Is there a talk where I can learn more about doing things with Widget Screen Components?

This isn't a talk about using the plugin, but I did give a talk at UnrealFest 2025 more about the philosphy and how it was used in Marvel Midnight suns. [How to Build UI at Scale: Lessons from Marvel’s 'Midnight Suns' | Unreal Fest Orlando 2025](https://www.youtube.com/watch?v=Pm2jswrHSck) The first talk on this subject was given at GDC 2023 called [UI Engineering patterns from 'Marvel's Midnight Suns'](https://gdcvault.com/play/1029385/UI-Engineering-Patterns-from-Marvel)  

### Who made this?
This was created by [Joel Gonzales | "LowPolyCount"](https://www.lowpolycount.com)

### Why was this made?
I'm tired of having to rewrite the same things whenever I change jobs. The laziest way to fix it is by making an open source version that can be used at any job.  If others find use with this, then all the better. 

### Well that's cool. 
That wasn't a question. 

### Sorry!
It's Ok. 

