// JsonULTTextPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"

#include <string>
#include <algorithm>
using namespace std;

#include "IULTPluginCommonInterface.h"
#include "IULTTextPluginInterface.h"
#include "GeneralPurposeFunctions.h"
#include "json/json.h"

TextPluginInfo* TextPluginInfoGlobal;

LanguagePairDics _cdecl GetOriginalMapFromText(wstring OriginalText, LanguagePairDics languagePairDics) {
	Json::Value OriginalJson;
	JsonReader.parse(WideMultiStringConverter.to_bytes(OriginalText), OriginalJson);
	if (languagePairDics.size() == 0) {
		LanguagePairDics languagePairDicsLocal;
		vector<string> initialMemberNames = OriginalJson.getMemberNames();
		LanguagePairDic languagePairDic;
		vector<AssetMapOption> nestedOptions;
		AssetMapOption keyAssetMapOption = {
			L"",
			L"",
			(void*)new wstring(L"(key)"),
			(void*)NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE
		};
		AssetMapOption valueAssetMapOption = {
			L"",
			L"",
			new wstring(L"(value)"),
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE
		};
		AssetMapOption arrayAssetMapOption = {
			L"",
			L"",
			new wstring(L"(array)"),
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE
		};
		nestedOptions.push_back(keyAssetMapOption);
		nestedOptions.push_back(valueAssetMapOption);
		nestedOptions.push_back(arrayAssetMapOption);
		AssetMapOption pathAssetMapOption = {
			L"Path",
			L"",
			NULL,
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE
		};
		for (vector<string>::iterator iterator = initialMemberNames.begin();
			iterator != initialMemberNames.end(); iterator++) {
			AssetMapOption assetMapOption;
			assetMapOption.type = AssetMapOption::Type::OPTION_TYPE_WSTRING;
			assetMapOption.Value = new wstring(WideMultiStringConverter.from_bytes(*iterator));
			assetMapOption.nestedOptions = nestedOptions;

			pathAssetMapOption.nestedOptions.push_back(assetMapOption);
		}
		languagePairDic.InteractWithAssetOptions.push_back(pathAssetMapOption);
		languagePairDicsLocal[L"0"] = languagePairDic;
		return languagePairDicsLocal;
	}
}

wstring _cdecl GetTranslatedTextFromMap(LanguagePairDics TranslatedMap, wstring OriginalText) {
	return L"";
}







bool _cdecl GetTextPluginInfo(TextPluginInfo* textPluginInfo) {
	TextPluginInfoGlobal = textPluginInfo;

	wcsncpy_s(textPluginInfo->TextPluginName, L"Json", 4);
	textPluginInfo->GetOriginalMapFromText = GetOriginalMapFromText;
	textPluginInfo->GetTranslatedTextFromMap = GetTranslatedTextFromMap;

	return true;
}