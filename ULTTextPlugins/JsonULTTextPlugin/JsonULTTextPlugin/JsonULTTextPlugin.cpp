// JsonULTTextPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
using namespace std;
#include <string>
#include <algorithm>

#include "IULTPluginCommonInterface.h"
#include "IULTTextPluginInterface.h"
#include "GeneralPurposeFunctions.h"
#include "json/json.h"

TextPluginInfo* TextPluginInfoGlobal;

//LanguagePairDics _cdecl GetOriginalMapFromText(wstring OriginalText, LanguagePairDics languagePairDics) {
//	Json::Value OriginalJson;
//	JsonParseFromString(WideMultiStringConverter->to_bytes(OriginalText), OriginalJson);
//	if (languagePairDics.size() == 0) {
//		LanguagePairDics languagePairDicsLocal;
//		vector<string> initialMemberNames = OriginalJson.getMemberNames();
//		LanguagePairDic languagePairDic;
//		vector<AssetMapOption> nestedOptions;
//		AssetMapOption keyAssetMapOption = {
//			L"",
//			L"",
//			(void*)new wstring(L"(key)"),
//			(void*)NULL,
//			AssetMapOption::Type::OPTION_TYPE_WSTRING,
//			AssetMapOption::Type::OPTION_TYPE_NONE
//		};
//		AssetMapOption valueAssetMapOption = {
//			L"",
//			L"",
//			new wstring(L"(value)"),
//			NULL,
//			AssetMapOption::Type::OPTION_TYPE_WSTRING,
//			AssetMapOption::Type::OPTION_TYPE_NONE
//		};
//		AssetMapOption arrayAssetMapOption = {
//			L"",
//			L"",
//			new wstring(L"(array)"),
//			NULL,
//			AssetMapOption::Type::OPTION_TYPE_WSTRING,
//			AssetMapOption::Type::OPTION_TYPE_NONE
//		};
//		nestedOptions.push_back(keyAssetMapOption);
//		nestedOptions.push_back(valueAssetMapOption);
//		nestedOptions.push_back(arrayAssetMapOption);
//		AssetMapOption pathAssetMapOption = {
//			L"Path",
//			L"",
//			NULL,
//			NULL,
//			AssetMapOption::Type::OPTION_TYPE_WSTRING,
//			AssetMapOption::Type::OPTION_TYPE_NONE
//		};
//		for (vector<string>::iterator iterator = initialMemberNames.begin();
//			iterator != initialMemberNames.end(); iterator++) {
//			AssetMapOption assetMapOption;
//			assetMapOption.type = AssetMapOption::Type::OPTION_TYPE_WSTRING;
//			assetMapOption.Value = new wstring(WideMultiStringConverter->from_bytes(*iterator));
//			assetMapOption.nestedOptions = nestedOptions;
//
//			pathAssetMapOption.nestedOptions.push_back(assetMapOption);
//		}
//		languagePairDic.InteractWithAssetOptions.push_back(pathAssetMapOption);
//		languagePairDicsLocal[L"0"] = languagePairDic;
//		return languagePairDicsLocal;
//	}
//}

AssetMapOption GetOriginalMapFromTextInternal(Json::Value json, AssetMapOption assetMapOptionIn);
wstring _cdecl GetTranslatedTextFromMap(wstring OriginalText, LanguagePairDics TranslatedMap);
vector<wstring> FindArrayElement(AssetMapOption assetMapOptionIn);
AssetMapOption GetKeyValueAssetMapOption(Json::Value json, AssetMapOption assetMapOptionIn);
map<wstring, LanguagePair> GetDic(Json::Value json, vector<AssetMapOption> assetMapOptions);
LanguagePairDics _cdecl GetOriginalMapFromText(wstring OriginalText, LanguagePairDics languagePairDics) {
	Json::Value OriginalJson;
	JsonParseFromString(WideMultiStringConverter->to_bytes(OriginalText), OriginalJson);
	if (languagePairDics.size() == 0) {
		AssetMapOption keyAssetMapOption = AssetMapOption(
			L"Key Path: ",
			L"",
			NULL,
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE,
			vector<AssetMapOption>()
		);
		AssetMapOption valueAssetMapOption = AssetMapOption(
			L"Value Path: ",
			L"",
			NULL,
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE,
			vector<AssetMapOption>()
		);
		AssetMapOption arrayAssetMapOption = AssetMapOption(
			L"Array Path: ",
			L"",
			NULL,
			NULL,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			AssetMapOption::Type::OPTION_TYPE_NONE,
			vector<AssetMapOption>()
		);
		languagePairDics[L"0"].InteractWithAssetOptions.push_back(arrayAssetMapOption);
		languagePairDics[L"0"].InteractWithAssetOptions.push_back(keyAssetMapOption);
		languagePairDics[L"0"].InteractWithAssetOptions.push_back(valueAssetMapOption);
	}
	vector<wstring> updateLanguagePairDicsKeyList;
	for (LanguagePairDics::iterator iterator = languagePairDics.begin();
		iterator != languagePairDics.end(); iterator++) {
		if (iterator->second.InteractWithAssetOptions.size() == 0) {
			AssetMapOption keyAssetMapOption = AssetMapOption(
				L"Key Path: ",
				L"",
				NULL,
				NULL,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				AssetMapOption::Type::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			AssetMapOption valueAssetMapOption = AssetMapOption(
				L"Value Path: ",
				L"",
				NULL,
				NULL,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				AssetMapOption::Type::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			AssetMapOption arrayAssetMapOption = AssetMapOption(
				L"Array Path: ",
				L"",
				NULL,
				NULL,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				AssetMapOption::Type::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			languagePairDics[iterator->first].InteractWithAssetOptions.push_back(arrayAssetMapOption);
			languagePairDics[iterator->first].InteractWithAssetOptions.push_back(keyAssetMapOption);
			languagePairDics[iterator->first].InteractWithAssetOptions.push_back(valueAssetMapOption);
		}
		AssetMapOption arrayAssetMapOption = GetOriginalMapFromTextInternal(OriginalJson, iterator->second.InteractWithAssetOptions[0]);
		iterator->second.InteractWithAssetOptions[0] = arrayAssetMapOption;
		vector<wstring> findArrayElement = FindArrayElement(arrayAssetMapOption);
		int sizeOfFindArrayElement = findArrayElement.size();
		if (sizeOfFindArrayElement != 0 && findArrayElement[sizeOfFindArrayElement - 1] == L"(Set As Array)") {
			Json::Value keyValueJson = OriginalJson;
			for (int i = 0; i < sizeOfFindArrayElement - 1; i++) {
				keyValueJson = keyValueJson[WideMultiStringConverter->to_bytes(findArrayElement[i])];
			}
			AssetMapOption keyAssetMapOption = GetKeyValueAssetMapOption(keyValueJson, iterator->second.InteractWithAssetOptions[1]);
			iterator->second.InteractWithAssetOptions[1] = keyAssetMapOption;
			AssetMapOption valueAssetMapOption = GetKeyValueAssetMapOption(keyValueJson, iterator->second.InteractWithAssetOptions[2]);
			iterator->second.InteractWithAssetOptions[2] = valueAssetMapOption;
			vector<wstring> findKeyElement = FindArrayElement(keyAssetMapOption);
			vector<wstring> findValueElement = FindArrayElement(valueAssetMapOption);
			int sizeOfFindKeyElement = findKeyElement.size();
			int sizeOfFindValueElement = findValueElement.size();
			if (sizeOfFindKeyElement != 0 && findKeyElement[sizeOfFindKeyElement -1] == L"(Set As Key or Value)"
				&& sizeOfFindValueElement != 0 && findValueElement[sizeOfFindValueElement -1] == L"(Set As Key or Value)") {
				iterator->second.Dic = GetDic(OriginalJson, iterator->second.InteractWithAssetOptions);
				if (sizeOfFindArrayElement >= 2) {
					if (iterator->first != findArrayElement[sizeOfFindArrayElement - 2]) {
						updateLanguagePairDicsKeyList.push_back(iterator->first);
					}
				}
			}
		}
	}
	for (vector<wstring>::iterator iterator = updateLanguagePairDicsKeyList.begin();
		iterator != updateLanguagePairDicsKeyList.end(); iterator++) {
		LanguagePairDic tempLanguagePairDic = languagePairDics[*iterator];
		languagePairDics.erase(*iterator);
		vector<wstring> findArrayElement = FindArrayElement(tempLanguagePairDic.InteractWithAssetOptions[0]);
		vector<wstring> findKeyElement = FindArrayElement(tempLanguagePairDic.InteractWithAssetOptions[1]);
		vector<wstring> findValueElement = FindArrayElement(tempLanguagePairDic.InteractWithAssetOptions[2]);
		int sizeOfFindArrayElement = findArrayElement.size();
		int sizeOfFindKeyElement = findKeyElement.size();
		int sizeOfFindValueElement = findValueElement.size();
		languagePairDics[findArrayElement[sizeOfFindArrayElement - 2]] = tempLanguagePairDic;
	}
	return languagePairDics;
}

map<wstring, LanguagePair> GetDic(Json::Value json, vector<AssetMapOption> assetMapOptions) {
	map<wstring, LanguagePair> results;
	AssetMapOption arrayAssetMapOption = assetMapOptions[0];
	AssetMapOption keyAssetMapOption = assetMapOptions[1];
	AssetMapOption valueAssetMapOption = assetMapOptions[2];
	vector<wstring> findArrayElement = FindArrayElement(arrayAssetMapOption);
	vector<wstring> findKeyElement = FindArrayElement(keyAssetMapOption);
	vector<wstring> findValueElement = FindArrayElement(valueAssetMapOption);
	int sizeOfFindArrayElement = findArrayElement.size();
	int sizeOfFindKeyElement = findKeyElement.size();
	int sizeOfFindValueElement = findValueElement.size();
	Json::Value arrayJson = json;
	for (int i = 0; i < sizeOfFindArrayElement - 1; i++) {
		arrayJson = arrayJson[WideMultiStringConverter->to_bytes(findArrayElement[i])];
	}
	if (arrayJson.isArray()) {
		for (Json::ArrayIndex i = 0; i < arrayJson.size(); i++) {
			Json::Value keyJson = arrayJson[i];
			Json::Value valueJson = arrayJson[i];
			for (int j = 0; j < sizeOfFindKeyElement - 1; j++) {
				keyJson = keyJson[WideMultiStringConverter->to_bytes(findKeyElement[j])];
			}
			for (int j = 0; j < sizeOfFindKeyElement - 1; j++) {
				valueJson = valueJson[WideMultiStringConverter->to_bytes(findValueElement[j])];
			}
			LanguagePair tempLanguagePair = { WideMultiStringConverter->from_bytes(valueJson.asString()), L"" };
			results.insert(pair<wstring, LanguagePair>(WideMultiStringConverter->from_bytes(keyJson.asString()), tempLanguagePair));
		}
	}
	return results;
}

AssetMapOption GetKeyValueAssetMapOption(Json::Value json, AssetMapOption assetMapOptionIn) {
	if (json.isArray()) {
		return GetKeyValueAssetMapOption(json[0], assetMapOptionIn);
	}
	if (json.isObject() == false) {
		if (assetMapOptionIn.nestedOptions.size() == 0) {
			AssetMapOption child = AssetMapOption(
				L"",
				L"",
				NULL,
				(void*)new wstring(L"(Set As Key or Value)"),
				AssetMapOption::Type::OPTION_TYPE_NONE,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				vector<AssetMapOption>()
			);
			assetMapOptionIn.nestedOptions.push_back(child);
			assetMapOptionIn.Value = assetMapOptionIn.nestedOptions[0].ValueAsChild;
		}
		return assetMapOptionIn;
	}
	if (assetMapOptionIn.Value == NULL || *(wstring*)assetMapOptionIn.Value == L"") {
		if (assetMapOptionIn.nestedOptions.size() == 0) {
			vector<string> memberNames = json.getMemberNames();
			for (vector<string>::iterator iterator = memberNames.begin();
				iterator != memberNames.end(); iterator++) {
				AssetMapOption child = AssetMapOption(
					L"",
					L"",
					NULL,
					(void*)new wstring(WideMultiStringConverter->from_bytes(*iterator)),
					AssetMapOption::Type::OPTION_TYPE_WSTRING,
					AssetMapOption::Type::OPTION_TYPE_WSTRING,
					vector<AssetMapOption>()
				);
				assetMapOptionIn.nestedOptions.push_back(child);
			}
			AssetMapOption setAsArrayChild = AssetMapOption(
				L"",
				L"",
				NULL,
				(void*)new wstring(L"(Set As Key or Value)"),
				AssetMapOption::Type::OPTION_TYPE_NONE,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				vector<AssetMapOption>()
			);
			assetMapOptionIn.nestedOptions.push_back(setAsArrayChild);
		}
	}
	else {
		wstring value = *(wstring*)assetMapOptionIn.Value;
		if (value == L"(Set As Key or Value)") {
			return assetMapOptionIn;
		}
		for (vector<AssetMapOption>::iterator iterator = assetMapOptionIn.nestedOptions.begin();
			iterator != assetMapOptionIn.nestedOptions.end(); iterator++) {
			if (value == *(wstring*)iterator->ValueAsChild) {
				*iterator = GetKeyValueAssetMapOption(json[WideMultiStringConverter->to_bytes(value)], *iterator);
			}
		}
	}
	return assetMapOptionIn;
}

vector<wstring> FindArrayElement(AssetMapOption assetMapOptionIn) {
	vector<wstring> paths;
	if (assetMapOptionIn.Value == NULL || *(wstring*)assetMapOptionIn.Value == L"") {
		return paths;
	}
	else {
		paths.push_back(*(wstring*)assetMapOptionIn.Value);
		for (vector<AssetMapOption>::iterator iterator = assetMapOptionIn.nestedOptions.begin();
			iterator != assetMapOptionIn.nestedOptions.end(); iterator++) {
			if (*(wstring*)iterator->ValueAsChild == *(wstring*)assetMapOptionIn.Value) {
				vector<wstring> childPaths = FindArrayElement(*iterator);
				paths.insert(paths.end(), childPaths.begin(), childPaths.end());
				return paths;
			}
		}
	}
	return paths;
}

AssetMapOption GetOriginalMapFromTextInternal(Json::Value json, AssetMapOption assetMapOptionIn) {
	if (assetMapOptionIn.Value == NULL || *(wstring*)assetMapOptionIn.Value == L"") {
		if (assetMapOptionIn.nestedOptions.size() == 0) {
			if (json.isArray()) {
				AssetMapOption setAsArrayChild = AssetMapOption(
					L"",
					L"",
					NULL,
					(void*)new wstring(L"(Set As Array)"),
					AssetMapOption::Type::OPTION_TYPE_NONE,
					AssetMapOption::Type::OPTION_TYPE_WSTRING,
					vector<AssetMapOption>()
				);
				assetMapOptionIn.nestedOptions.push_back(setAsArrayChild);
				assetMapOptionIn.Value = (void*)new wstring(L"(Set As Array)");
			}
			else {
				vector<string> memberNames = json.getMemberNames();
				for (vector<string>::iterator iterator = memberNames.begin();
					iterator != memberNames.end(); iterator++) {
					AssetMapOption child = AssetMapOption(
						L"",
						L"",
						NULL,
						(void*)new wstring(WideMultiStringConverter->from_bytes(*iterator)),
						AssetMapOption::Type::OPTION_TYPE_WSTRING,
						AssetMapOption::Type::OPTION_TYPE_WSTRING,
						vector<AssetMapOption>()
					);
					assetMapOptionIn.nestedOptions.push_back(child);
				}
				AssetMapOption setAsArrayChild = AssetMapOption(
					L"",
					L"",
					NULL,
					(void*)new wstring(L"(Set As Array)"),
					AssetMapOption::Type::OPTION_TYPE_NONE,
					AssetMapOption::Type::OPTION_TYPE_WSTRING,
					vector<AssetMapOption>()
				);
				assetMapOptionIn.nestedOptions.push_back(setAsArrayChild);
			}
		}
	}
	else {
		wstring value = *(wstring*)assetMapOptionIn.Value;
		if (value == L"(Set As Array)") {
			for (vector<AssetMapOption>::iterator iterator = assetMapOptionIn.nestedOptions.begin();
				iterator != assetMapOptionIn.nestedOptions.end(); iterator++) {
				if (iterator->typeAsChild == AssetMapOption::OPTION_TYPE_WSTRING &&
					iterator->ValueAsChild != NULL && *(wstring*)iterator->ValueAsChild == L"(Set As Array)") {
					return assetMapOptionIn;
				}
			}
			AssetMapOption setAsArrayChild = AssetMapOption(
				L"",
				L"",
				NULL,
				(void*)new wstring(L"(Set As Array)"),
				AssetMapOption::Type::OPTION_TYPE_NONE,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				vector<AssetMapOption>()
			);
			assetMapOptionIn.nestedOptions.push_back(setAsArrayChild);
			return assetMapOptionIn;
		}
		for (vector<AssetMapOption>::iterator iterator = assetMapOptionIn.nestedOptions.begin();
			iterator != assetMapOptionIn.nestedOptions.end(); iterator++) {
			if (value == *(wstring*)iterator->ValueAsChild) {
				*iterator = GetOriginalMapFromTextInternal(json[WideMultiStringConverter->to_bytes(value)], *iterator);
			}
		}
	}
	return assetMapOptionIn;
}

map<wstring, LanguagePair> GetTranslatedTextFromMapInternalDic;
Json::Value GetTranslatedTextFromMapInternalArray(
	Json::Value OriginalJson,
	AssetMapOption arrayPathOption,
	AssetMapOption keyPathOption,
	AssetMapOption valuePathOption
);
Json::Value GetTranslatedTextFromMapKeyValue(Json::Value json, AssetMapOption keyPathOption, AssetMapOption valuePathOption);
wstring GetTranslatedTextFromMapInternalKey(Json::Value json, AssetMapOption keyPathOption);
Json::Value GetTranslatedTextFromMapInternalValue(Json::Value json, wstring key, AssetMapOption valuePathOption);
wstring _cdecl GetTranslatedTextFromMap(wstring OriginalText, LanguagePairDics TranslatedMap) {
	Json::Value OriginalJson;
	JsonParseFromString(WideMultiStringConverter->to_bytes(OriginalText), OriginalJson);
	for (LanguagePairDics::iterator iterator = TranslatedMap.begin();
		iterator != TranslatedMap.end(); iterator++) {
		GetTranslatedTextFromMapInternalDic = iterator->second.Dic;
		OriginalJson = GetTranslatedTextFromMapInternalArray(
			OriginalJson,
			iterator->second.InteractWithAssetOptions[0],
			iterator->second.InteractWithAssetOptions[1],
			iterator->second.InteractWithAssetOptions[2]
		);
	}
	return WideMultiStringConverter->from_bytes(OriginalJson.toStyledString());
}

Json::Value GetTranslatedTextFromMapInternalArray(
	Json::Value OriginalJson,
	AssetMapOption arrayPathOption,
	AssetMapOption keyPathOption,
	AssetMapOption valuePathOption
) {
	if (*(wstring*)arrayPathOption.Value == L"(Set As Array)") {
		for (Json::ArrayIndex i = 0; i < OriginalJson.size(); i++) {
			OriginalJson[i] = GetTranslatedTextFromMapKeyValue(OriginalJson[i], keyPathOption, valuePathOption);
		}
	}
	else {
		for (vector<AssetMapOption>::iterator iterator = arrayPathOption.nestedOptions.begin();
			iterator != arrayPathOption.nestedOptions.end(); iterator++) {
			if (*(wstring*)iterator->ValueAsChild == *(wstring*)arrayPathOption.Value) {
				string key = WideMultiStringConverter->to_bytes(*(wstring*)arrayPathOption.Value);
				OriginalJson[key]
					= GetTranslatedTextFromMapInternalArray(OriginalJson[key], *iterator, keyPathOption, valuePathOption);
				break;
			}
		}
	}
	return OriginalJson;
}

Json::Value GetTranslatedTextFromMapKeyValue(Json::Value json, AssetMapOption keyPathOption, AssetMapOption valuePathOption) {
	wstring key = GetTranslatedTextFromMapInternalKey(json, keyPathOption);
	json = GetTranslatedTextFromMapInternalValue(json, key, valuePathOption);
	return json;
}

wstring GetTranslatedTextFromMapInternalKey(Json::Value json, AssetMapOption keyPathOption) {
	if (*(wstring*)keyPathOption.Value == L"(Set As Key or Value)") {
		return WideMultiStringConverter->from_bytes(json.asString());
	}
	else {
		for (vector<AssetMapOption>::iterator iterator = keyPathOption.nestedOptions.begin();
			iterator != keyPathOption.nestedOptions.end(); iterator++) {
			if (*(wstring*)keyPathOption.Value == *(wstring*)iterator->ValueAsChild) {
				return GetTranslatedTextFromMapInternalKey(json[WideMultiStringConverter->to_bytes(*(wstring*)keyPathOption.Value)], *iterator);
			}
		}
	}
}

Json::Value GetTranslatedTextFromMapInternalValue(Json::Value json, wstring key, AssetMapOption valuePathOption) {
	if (*(wstring*)valuePathOption.Value == L"(Set As Key or Value)") {
		map<wstring, LanguagePair>::iterator languagePairItr = GetTranslatedTextFromMapInternalDic.find(key);
		if (languagePairItr != GetTranslatedTextFromMapInternalDic.end()) {
			if (languagePairItr->second.Translated != L"") {
				json = WideMultiStringConverter->to_bytes(languagePairItr->second.Translated);
			}
		}
	}
	else {
		for (vector<AssetMapOption>::iterator iterator = valuePathOption.nestedOptions.begin();
			iterator != valuePathOption.nestedOptions.end(); iterator++) {
			if (*(wstring*)valuePathOption.Value == *(wstring*)iterator->ValueAsChild) {
				string valuePath = WideMultiStringConverter->to_bytes(*(wstring*)valuePathOption.Value);
				json[valuePath] = GetTranslatedTextFromMapInternalValue(json[valuePath], key, *iterator);
			}
		}
	}
	return json;
}


LanguagePairDics _cdecl GetUpdateFileTextFromMap(LanguagePairDics UpdateMap) {
	for (LanguagePairDics::iterator iterator = UpdateMap.begin();
		iterator != UpdateMap.end(); iterator++) {
		if (iterator->second.InteractWithFileTextOptions.size() !=4) {
			iterator->second.InteractWithFileTextOptions.clear();
			AssetMapOption fileNameOption = AssetMapOption(
				L"File Name:",
				L"Set File Name saved.",
				new wstring(iterator->first + L".json"),
				NULL,
				AssetMapOption::OPTION_TYPE_WSTRING,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			AssetMapOption keyNameOption = AssetMapOption(
				L"Key Path: ",
				L"Set Key Path.",
				new wstring(L"key"),
				NULL,
				AssetMapOption::OPTION_TYPE_WSTRING,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			AssetMapOption valueNameOption = AssetMapOption(
				L"Original Text Path: ",
				L"Set non-translated text path.",
				new wstring(L"OriginalText"),
				NULL,
				AssetMapOption::OPTION_TYPE_WSTRING,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			AssetMapOption translatedNameOption = AssetMapOption(
				L"Translated Text Path: ",
				L"Set translated text path.",
				new wstring(L"TranslatedText"),
				NULL,
				AssetMapOption::OPTION_TYPE_WSTRING,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			iterator->second.InteractWithFileTextOptions.push_back(fileNameOption);
			iterator->second.InteractWithFileTextOptions.push_back(keyNameOption);
			iterator->second.InteractWithFileTextOptions.push_back(valueNameOption);
			iterator->second.InteractWithFileTextOptions.push_back(translatedNameOption);
		}
		void* fileNamePtr = iterator->second.InteractWithFileTextOptions[0].Value;
		void* keyPathPtr = iterator->second.InteractWithFileTextOptions[1].Value;
		void* originalTextPathPtr = iterator->second.InteractWithFileTextOptions[2].Value;
		void* translatedTextPathPtr = iterator->second.InteractWithFileTextOptions[3].Value;
		if (fileNamePtr && keyPathPtr && originalTextPathPtr && translatedTextPathPtr) {
			wstring fileName = *(wstring*)fileNamePtr;
			iterator->second.TranslatedFileName = fileName;
			wstring keyPath = *(wstring*)keyPathPtr;
			wstring originalTextPath = *(wstring*)originalTextPathPtr;
			wstring translatedTextPath = *(wstring*)translatedTextPathPtr;
			if (!(fileName.empty() && keyPath.empty() && originalTextPath.empty() & translatedTextPath.empty())) {
				Json::Value json(Json::arrayValue);
				for (map<wstring, LanguagePair>::iterator dicItr = iterator->second.Dic.begin();
					dicItr != iterator->second.Dic.end(); dicItr++) {
					Json::Value jsonChild;
					jsonChild[WideMultiStringConverter->to_bytes(keyPath)] = WideMultiStringConverter->to_bytes(dicItr->first);
					jsonChild[WideMultiStringConverter->to_bytes(originalTextPath)] = WideMultiStringConverter->to_bytes(dicItr->second.Original);
					jsonChild[WideMultiStringConverter->to_bytes(translatedTextPath)] = WideMultiStringConverter->to_bytes(dicItr->second.Translated);
					json.append(jsonChild);
				}
				iterator->second.TranslatedFileText = WideMultiStringConverter->from_bytes(json.toStyledString());
			}
		}
	}
	return UpdateMap;
}

LanguagePairDics _cdecl GetTranslatedMapFromFileText(LanguagePairDics translatedMap) {
	for (LanguagePairDics::iterator iterator = translatedMap.begin();
		iterator != translatedMap.end(); iterator++) {
		string keyPath = WideMultiStringConverter->to_bytes(*(wstring*)iterator->second.InteractWithFileTextOptions[1].Value);
		string originalTextPath = WideMultiStringConverter->to_bytes(*(wstring*)iterator->second.InteractWithFileTextOptions[2].Value);
		string translatedTextPath = WideMultiStringConverter->to_bytes(*(wstring*)iterator->second.InteractWithFileTextOptions[3].Value);
		Json::Value json;
		JsonParseFromString(WideMultiStringConverter->to_bytes(iterator->second.TranslatedFileText), json);
		if (json.isArray()) {
			for (Json::ArrayIndex i = 0; i < json.size(); i++) {
				Json::Value jsonChild = json[i];
				wstring key;
				wstring originalText;
				wstring translatedText;
				if (jsonChild.isMember(keyPath)) {
					key = WideMultiStringConverter->from_bytes(jsonChild[keyPath].asString());
					if (jsonChild.isMember(originalTextPath)) {
						originalText = WideMultiStringConverter->from_bytes(jsonChild[originalTextPath].asString());
					}
					if (jsonChild.isMember(translatedTextPath)) {
						translatedText = WideMultiStringConverter->from_bytes(jsonChild[translatedTextPath].asString());
					}
					LanguagePair languagePair = {
						originalText,
						translatedText
					};
					map<wstring, LanguagePair>::iterator dicItr = iterator->second.Dic.find(key);
					if (dicItr != iterator->second.Dic.end()) {
						iterator->second.Dic[key] = languagePair;
					}
					else {
						iterator->second.Dic.insert(pair<wstring, LanguagePair>(key, languagePair));
					}
				}
				else {
					continue;
				}
			}
		}
	}
	return translatedMap;
}




bool _cdecl GetTextPluginInfo(TextPluginInfo* textPluginInfo) {
	TextPluginInfoGlobal = textPluginInfo;

	wcsncpy_s(textPluginInfo->TextPluginName, L"Json", 4);
	textPluginInfo->GetOriginalMapFromText = GetOriginalMapFromText;
	textPluginInfo->GetTranslatedTextFromMap = GetTranslatedTextFromMap;

	textPluginInfo->GetUpdateFileTextFromMap = GetUpdateFileTextFromMap;
	textPluginInfo->GetTranslatedMapFromFileText = GetTranslatedMapFromFileText;
	return true;
}