# Open Unreal Utilities Sample Project

This is a sample project for the Open Unreal Utilities plugin for Unreal Engine 4.
Please check the [plugin folder itself](Plugins/OpenUnrealUtilities) for a more detailed documentation of the plugin itself.
The plugin source code is [hosted separately on github](https://github.com/JonasReich/OpenUnrealUtilities). 

This repository contains a full copy of the plugin and its version history embedded as git subtree.
Therefore if you want to make extensions to the plugin you can clone this repository instead of the
main plugin repo to ensure the plugin can be built without any other project dependencies.

Changes can be pulled from the main plugin repo (or a fork thereof) by using subtree pull:

`` git subtree pull --prefix Plugins/OpenUnrealUtilities <remote-repo> <remote-ref>``

In the same way changes initially committed in this repo can be pushed to the main plugin repo via subtree push:

``git subtree push --prefix Plugins/OpenUnrealUtilities <remote-repo> <remote-ref>``

Please check the [git subtree man-pages](https://manpages.debian.org/testing/git-man/git-subtree.1.en.html) for a more detailed explanation of this process.
