## About

A mod for Sonic Adventure 2 that displays subtitles for most voice lines with no text originally (or the text existed but was never displayed) that you can encounter during normal play.

Subtitles will be displayed for the following:
* menu screens
* idle voices (common, stage specific)
* other stage-specific voices (e.g. story mode-only Eggman's lines for Crazy Gadget)
* rank voices
* victory lines
* some gameplay voices (e.g. item get, hunting, kart racing, Omochao bullying, etc.)
* boss battle voices
* 2P Battle voices (some gameplay lines and intros)
* last episode ending cutscene voices (with no changes to the original text that is present in event text files)

Any subtitle group can be disabled in the mod config. Note that some voices are used in not just one case (e.g. one of Shadow's common idle voices is also used as his 2P alternate costume intro voice).

Currently, the mod will NOT display subtitles for voices in soundbanks (e.g. light dash or kart racing boost voices). It also doesn't display subtitles for FMV's (most likely, that would require hardsubbing the FMV's and have multiple versions of them for all languages).

Currently supported languages:
* English (dub)
* French (translated from English by Kesnos)
* Spanish (translated from English by kawaiikaorichan)
* Japanese (experimental)

Other features in the mod config (for all languages in the game):
* fix main menu text if [Menu Overhaul](https://gamebanana.com/mods/33169) mod is enabled
* fix exit messages displaying out of the text box if [Render Fix](https://gamebanana.com/mods/452445) mod is enabled and uses "Compact" or higher letter spacing (but probably not higher than "Comfy")

All subtitles are stored in JSON files made specifically for this mod.

Subtitles were generally tested to fit 4:3 aspect ratio with the following font settings in Render Fix config: "Comfy" letter spacing (matching console versions), Comic Sans Latin font (also like console versions), vanilla Japanese font (which is wider than the console one).

This mod does NOT feature retranslated English subtitles as they are a part of [Retranslated Story -COMPLETE-](https://gamebanana.com/mods/437858) mod. If you're playing with the said mod AND this mod, it will display a warning message.

Contributions for other languages (German, Italian) would be appreciated.

## Using API

The mod features a single exported function that allows other mods to load their custom sets of extra subtitles without implementing the whole thing, just by calling a single function. This is the function declaration:

```c++
void LoadCustomExtraSubs(const char* jsonPath, Languages language, int codepage)
```

First, set up a function pointer to the exported function in your mod's code. The function takes 3 arguments: path to JSON file with extra subtitles, language and codepage. The function pointer would look like this:

```c++
void (*LoadCustomExtraSubs)(const char* jsonPath, Languages language, int codepage) = nullptr;
```
Then, in your mod's `Init` function or any other function that would be called in `Init` you need to retrieve the exported function. You can do this using the mod loader API or WinAPI.

Using the mod loader API:

```c++
auto sa2ExtraSubs = helperFunctions.Mods->find("sa2-extra-subtitles");
if (sa2ExtraSubs != nullptr)
{
	LoadCustomExtraSubs = sa2ExtraSubs->GetDllExport<decltype(LoadCustomExtraSubs)>("LoadCustomExtraSubs");
	// call the retrieved function later
}
```
Using WinAPI:

```c++
HMODULE sa2ExtraSubs = GetModuleHandle(L"SA2ExtraSubtitles");
if (sa2ExtraSubs != nullptr)
{
	LoadCustomExtraSubs = (decltype(LoadCustomExtraSubs))GetProcAddress(sa2ExtraSubs, "LoadCustomExtraSubs");
	// call the retrieved function later
}
```
After retrieving the function pointer check if it is not null and then call it like this:

```c++
if (LoadCustomExtraSubs != nullptr)
{
	std::string jsonPath = std::string(modPath) + "\\ExtraSubs\\Custom.json"; // modPath is the path to YOUR mod, the rest might be anything you want
	LoadCustomExtraSubs(jsonPath.c_str(), Language_English, 1252);
}
```

## Usage

You need [SA Mod Manager](https://github.com/X-Hax/SA-Mod-Manager) to play this mod.

[Mod page on Gamebanana](https://gamebanana.com/mods/583770)
