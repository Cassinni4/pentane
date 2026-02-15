# Pentane

A (work-in-progress) user-configurable plugin loader and modding framework for the following games:
- Rhythm Racing Engine
	- Cars: The Video Game (PC) 🟨 *Experimental!*
	- Cars: Mater-National Championship (PC) 🟨 *Experimental!*
- Octane Engine
	- Cars 2: The Video Game (PC) ✅ *Production-Ready!*
	- Cars 2: Arcade (PC) ✅ *Production-Ready!*
	- Toy Story 3: The Video Game (PC) 🟨 *Experimental!*
	- Cars 3: Driven to Win (Xbox One) ❌ *Non-Functional!*
    - Disney Infinity 3.0 Gold Edition (PC) 🟨 *Experimental!*
- nu2
	- Lego Star Wars: The Complete Saga (PC) 🟨 *Experimental!*

For documentation and usage information, head on over to [our website](https://high-octane-dev.github.io/)!

# Downloads
Head to the [Releases](https://github.com/Cassinni4/pentane/releases/latest) page to get the latest build!

## Installation and Usage

**BEFORE DOING ANYTHING**

Depending on which game .dll you download, you will have the following:

- Pentane-TCS.dll
- Pentane-DI3G.dll

THESE NEED TO BE RENAMED TO "Pentane.dll"!

You need to hex-edit the .exe file to use the IN3 and TCS Pentane ports.
For IN3, (with the .exe open in a hex-editor) search for "bink2w32.dll" Once found, change it in the text area to "Pentane.dll". There will be an extra 2 (it will look like "Pentane2.dll"), you need to go into the hex portion and set that extra character to 00. To see what it should look like at the end, take a look at the image below
<img width="544" height="100" alt="Image" src="https://github.com/Cassinni4/pentane/blob/main/source/in3.png" />
For TCS, you need to use [Steamless](https://github.com/atom0s/Steamless/releases/download/v3.1.0.5/Steamless.v3.1.0.5.-.by.atom0s.zip) on the .exe. Then, open you hex-editor and search for "binkw32.dll". Replace this with "Pentane.dll"

# Building
Pentane uses Visual Studio 2022 (MSBuild) as its build system, as it plays nicely with vcpkg; which is what it uses for pulling dependencies.
Pentane uses [vcpkg](https://vcpkg.io) in manifest mode, and depends on the following libraries:
- tomlplusplus (Used for `toml` parsing)
- detours (Required by sunset)
- zydis (Required by sunset)

Additionally, Pentane depends on [sunset](https://github.com/itsmeft24/sunset), an inline/replacement hooking library. As of writing, sunset is directly included in the repository, but will likely be *converted into a git submodule* in the future.

# Credits
- [itsmeft24](https://github.com/itsmeft24) (Creating the original Pentane)
- flaynirite (Russian Localization Help)
- [Code](https://github.com/Daavel) (German/Polish Localization Help)
- [maximilian](https://github.com/DJmax0955) (Polish Localization Help)
- [OwenTheProgrammer](https://github.com/OwenTheProgrammer) (French Localization Help)
- Timix91 (Russian Localization Help)
- [MythicalBry](https://github.com/MythicalBry) (Spanish Localization Help)
- [Raytwo](https://github.com/Raytwo) (French Localization Help)

# Inspirations
- [Raytwo/ARCropolis](https://github.com/Raytwo/ARCropolis)
- [skyline-dev/skyline](https://github.com/skyline-dev/skyline)
