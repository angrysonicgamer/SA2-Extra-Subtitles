#include "pch.h"
#include "OtherMods.h"
#include "Mod/Text/Encoding.h"
#include "Mod/Messages/Message.h"
#include "IniFile.hpp"


Module OtherMods::RenderFix;
Module OtherMods::MenuOverhaul;
Module OtherMods::RetranslatedStoryComplete;


void OtherMods::Init()
{
	RenderFix.Init(L"sa2-render-fix");
	MenuOverhaul.Init(L"MenuOverhaul");
	RetranslatedStoryComplete.Init(L"SA2Subtitles");
}


void OtherMods::EnableMenuOverhaulTextFix()
{
	if (!MenuOverhaul.IsLoaded()) return;

	for (int language = Language_Japanese; language <= Language_Italian; language++)
	{
		AdvertiseGamepad[language][2] = AdvertiseKeyboard[language][2] = AdvertiseGamepad[language][7];		// "View extra game contents."
		AdvertiseGamepad[language][3] = AdvertiseKeyboard[language][3] = AdvertiseGamepad[language][11],	// "Configure game settings."
		AdvertiseGamepad[language][4] = AdvertiseKeyboard[language][4] = AdvertiseGamepad[language][6];		// "Quit game."
	}
}

void OtherMods::EnableRFExitMessagesFix(const HelperFunctions& helperFunctions)
{
	if (!RenderFix.IsLoaded()) return;

	auto rf = helperFunctions.Mods->find_by_dll(RenderFix.ModHandle);
	IniFile rfConfig(std::string(rf->Folder) + "\\config.ini");
	int rfLetterSpacing = rfConfig.getInt("font", "width", 2);

	if (rfLetterSpacing >= 2) // "Compact" spacing
	{
		PauseMenuAndMessages[Language_Japanese].ExitMessages[0] = EncodeSJIS(L"ゲームをやめますか？\nセーブしていない\n部分の進行は失われます。");
		PauseMenuAndMessages[Language_Japanese].ExitMessages[1] = EncodeSJIS(L"このステージを始めから遊びますか？\nここまでの進行は失われます。");
		PauseMenuAndMessages[Language_Japanese].ExitMessages[4] = EncodeSJIS(L"ゲームを終了します。\nセーブしていない\n部分の進行は失われます。\n本当にゲームを終了しますか？");

		PauseMenuAndMessages[Language_English].ExitMessages[0] = Encode1252(L"Are you sure you want\nto quit the game?\nYour unsaved progress will be lost.");
		PauseMenuAndMessages[Language_English].ExitMessages[1] = Encode1252(L"Are you sure you want\nto restart the stage?\nYour unsaved progress will be lost.");
		PauseMenuAndMessages[Language_English].ExitMessages[4] = Encode1252(L"This action will end the game.\nAny unsaved progress will be lost.\nAre you sure you want to quit?");

		PauseMenuAndMessages[Language_French].ExitMessages[0] = Encode1252(L"Veux-tu vraiment\nquitter la partie ?\nToute progression\nnon sauvegardée sera perdue.");
		PauseMenuAndMessages[Language_French].ExitMessages[1] = Encode1252(L"Veux-tu vraiment\nrecommencer le niveau ?\nToute progression\nnon sauvegardée sera perdue.");
		PauseMenuAndMessages[Language_French].ExitMessages[4] = Encode1252(L"Ceci terminera la partie.\nToute progression\nnon sauvegardée sera perdue.\nVoulez-vous vraiment quitter ?");

		PauseMenuAndMessages[Language_Spanish].ExitMessages[0] = Encode1252(L"¿Seguro que quieres\nsalir de la partida?\nSe perderá todo progreso\nno guardado.");
		PauseMenuAndMessages[Language_Spanish].ExitMessages[1] = Encode1252(L"¿Seguro que quieres\nreiniciar la fase?\nSe perderá todo progreso\nno guardado.");
		PauseMenuAndMessages[Language_Spanish].ExitMessages[4] = Encode1252(L"Esto terminará la partida.\nTodo progreso no guardado\nse perderá. ¿Estás seguro\nde que quieres abandonar?");

		PauseMenuAndMessages[Language_German].ExitMessages[0] = Encode1252(L"Willst du das Spiel\nwirklich verlassen?\nNicht gespeicherte Fortschritte\ngehen verloren.");
		PauseMenuAndMessages[Language_German].ExitMessages[1] = Encode1252(L"Willst du das Level\nwirklich neu starten?\nNicht gespeicherte Fortschritte\ngehen verloren.");
		PauseMenuAndMessages[Language_German].ExitMessages[4] = Encode1252(L"Diese Aktion beendet das Spiel.\nUngespeicherter Fortschritt\ngeht verloren.\nDas Spiel wirklich verlassen?");

		PauseMenuAndMessages[Language_Italian].ExitMessages[0] = Encode1252(L"Sicuro di voler\nabbandonare la partita?\nI progressi non salvati\nandranno perduti.");
		PauseMenuAndMessages[Language_Italian].ExitMessages[1] = Encode1252(L"Sicuro di voler\nricominciare la fase?\nI progressi non salvati\nandranno perduti.");
		PauseMenuAndMessages[Language_Italian].ExitMessages[4] = Encode1252(L"Questo terminerà la partita.\nI progressi non salvati andranno\nperduti. Sicuro di voler\nabbandonare il gioco?");
	}
}

void OtherMods::CheckRetranslatedStoryComplete()
{
	if (RetranslatedStoryComplete.IsLoaded())
	{
		std::wstring retranslatedStoryLoaded =
			L"\"Retranslated Story -COMPLETE-\" mod is loaded.\n\n"
			L"It also features extra subtitles feature\nbut only retranslated English variant.\n"
			L"If you want to play with subtitles matching English dub,\ndisable \"Retranslated Story -COMPLETE-\".\n"
			L"If you want retranslated English subtitles, disable this mod.\n\n"
			L"Press \"OK\" to continue without changes\nor \"Cancel\" to exit the game.";
		
		Message::Warning(retranslatedStoryLoaded);
	}
}