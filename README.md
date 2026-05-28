# UI Power Tools - Make Better UI™

UI Power Tools is a plugin for Unreal Engine to help you make User Interfaces

## Features
### User Interface Component System (UICS)
UICS is similar to the [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) used in games, but made for UI and provides you helpful components such as:
* The [Data Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Data-Screen-Component) gives you a way to retrieve and filter data for your screen
* The [View Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/View-Screen-Component) works with any panel and will gives you functions and events to know when a widget has changed focus, it's selection, etc. It can automatically create widgets for you from Data retrieved through a Data Component.
* The [Action Component](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Action-Screen-Component) will do something in response to a user action such as button press, hover, etc

These components can work together reducing the amount of code needed and giving you a consistent way to build your UI. 

### Screen Manager
The [Screen Manager](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Screen-Manager) is globally accessible and used to display screens.  It allows you to stack screens on top of each other and optionally hide screens below themn when they go on the manager. 

# Quick Start Guide
- If it doesn't exist, create a Plugins directory in your project
- [Download a release version](https://codeberg.org/LowPolyCount/UI-Power-Tools/releases) and unzip to the plugins directory
- Restart your project and it will prompt you to build the plugin.  


# Help & Learning
### Tutorials
- If you want to see what UI Power Tools is capable of, follow [A Step-by-Step Tutorial on how to use UI Power Tools to Create a Screen](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/Tutorial-Using-Data%2C-View%2C-Action-Components)

### Example Project
- [The UIPT Example Project](https://codeberg.org/LowPolyCount/UIPowerTools_Example) has several UI screens implemented showing how UIPT works.

### Wiki
- [The Wiki](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki) has information on how to use UI Power Tools.

### Discord
- You can ask questions on [The UIPT Discord Server](https://discord.gg/nBnvbf8TnU)

# Status

UI Power Tools is considered stable meaning it can be used in production code.  It has not yet hit v1 meaning there may be API changes. [View remaining v1 issues to resolve](https://codeberg.org/LowPolyCount/UI-Power-Tools/milestone/55170)

# License 
- Licensed under [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0)

# FAQ
### Is there a talk where I can learn more about doing things with Widget Screen Components?
Yes, A talk was given At UnrealFest 2025, see [How to Build UI at Scale: Lessons from Marvel’s 'Midnight Suns' | Unreal Fest Orlando 2025](https://www.youtube.com/watch?v=Pm2jswrHSck)

The first talk on this subject was given at GDC 2023 called [UI Engineering patterns from 'Marvel's Midnight Suns'](https://gdcvault.com/play/1029385/UI-Engineering-Patterns-from-Marvel) But the UnrealFest talk has more up to date information. 

### Can I use MVVM with UICS?
Yes, UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, blueprint, slate, or whatever method you want.

### What if I find an issue? 
Please [Make an Issue](https://codeberg.org/LowPolyCount/UI-Power-Tools/issues/new). You can use your github account to login to Codeberg. 

### Where can I find help?
[The Wiki](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki) has information that may help or you can ask in [The Discord Server](https://discord.gg/nBnvbf8TnU)

### What if I want to use this with an existing codebase?
While We provide Base Classes that let you use this plugin "Out of the Box", we do let you override these classes with (hopefully) minimal effort. See [Overriding Provided Classes](https://codeberg.org/LowPolyCount/UI-Power-Tools/wiki/UIPT-Overriding-Provided-Classes)

### Who made this?
This was created by [Joel Gonzales | "LowPolyCount"](https://www.lowpolycount.com)

### Why was this made?
I'm tired of having to rewrite the same things whenever I change jobs. The laziest way to fix it is by making an open source version that can be used at any job.  If others find use with this, then all the better. 

### Well that's cool. 
That wasn't a question. 

### Sorry!
It's Ok. 

