# UI-Power-Tools - Make Better UI™

UI Power Tools is a plugin for Unreal Engine to help you make User Interfaces

## Features
### User Interface Component System (UICS)
UICS is similar to the [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) used in games, but made for UI. It helps you reuse functionality across your game or project and reduce the amount of code that needs to be written. The Provided Screen Components like Data and View, will help you put together a UI screen with minimal code. 

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

### Can I use MVVM with UICS?
Yes, UICS doesn't specify how you set the visual elements of your widgets so you can use MVVM, blueprint, slate, or whatever method you want.

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

