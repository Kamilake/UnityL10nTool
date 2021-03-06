// RegExpULTTextPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
// http://cinrueom.tistory.com/6
#pragma warning(disable: 4996)


#include "stdafx.h"
using namespace std;
#include <string>
#include <algorithm>
#include <regex>

#include "IULTPluginCommonInterface.h"
#include "IULTTextPluginInterface.h"
#include "GeneralPurposeFunctions.h"
#include "json/json.h"

TextPluginInfo* TextPluginInfoGlobal;

LanguagePairDics _cdecl GetOriginalMapFromText(wstring OriginalText, LanguagePairDics languagePairDics) {
	if (languagePairDics.size() == 0) {
		LanguagePairDic languagePairDic;
		LanguagePair tempLanguagePair = { OriginalText, L"" };
		languagePairDic.Dic.insert(pair<wstring, LanguagePair>(L"0", tempLanguagePair));
		languagePairDics.insert(pair<wstring,LanguagePairDic>(L"0", languagePairDic));
	}
	return languagePairDics;
}

wstring _cdecl GetTranslatedTextFromMap(wstring OriginalText, LanguagePairDics TranslatedMap) {
	
	return TranslatedMap[L"0"].TranslatedFileText;
}

LanguagePairDics _cdecl GetUpdateFileTextFromMap(LanguagePairDics UpdateMap) {

	if (UpdateMap[L"0"].InteractWithFileTextOptions.size() == 0) {
		AssetMapOption fileNameOption = AssetMapOption(
			L"File Name:",
			L"Set File Name saved.",
			new wstring(L"Rename.txt"),
			NULL,
			AssetMapOption::OPTION_TYPE_WSTRING,
			AssetMapOption::OPTION_TYPE_NONE,
			vector<AssetMapOption>()
		);
		UpdateMap[L"0"].InteractWithFileTextOptions.push_back(fileNameOption);
	}
	UpdateMap[L"0"].TranslatedFileText = UpdateMap[L"0"].Dic[L"0"].Original;
	UpdateMap[L"0"].TranslatedFileName = *(wstring*)UpdateMap[L"0"].InteractWithFileTextOptions[0].Value;
	return UpdateMap;
}

LanguagePairDics _cdecl GetTranslatedMapFromFileText(LanguagePairDics translatedMap) {
	translatedMap[L"0"].Dic[L"0"].Translated = translatedMap[L"0"].TranslatedFileText;
	return translatedMap;
}

bool _cdecl GetTextPluginInfo(TextPluginInfo* textPluginInfo) {
	TextPluginInfoGlobal = textPluginInfo;

	wcsncpy_s(textPluginInfo->TextPluginName, L"Raw", 6);
	textPluginInfo->GetOriginalMapFromText = GetOriginalMapFromText;
	textPluginInfo->GetTranslatedTextFromMap = GetTranslatedTextFromMap;

	textPluginInfo->GetUpdateFileTextFromMap = GetUpdateFileTextFromMap;
	textPluginInfo->GetTranslatedMapFromFileText = GetTranslatedMapFromFileText;
	return true;
}