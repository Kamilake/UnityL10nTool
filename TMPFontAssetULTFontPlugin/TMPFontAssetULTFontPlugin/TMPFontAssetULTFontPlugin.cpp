// TMPFontAssetULTFontPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"

#include <string>
#include <algorithm>
using namespace std;

#include "AssetsTools/AssetsFileReader.h"
#include "AssetsTools/AssetsFileFormat.h"
#include "AssetsTools/ClassDatabaseFile.h"
#include "AssetsTools/AssetsFileTable.h"
#include "AssetsTools/ResourceManagerFile.h"
#include "AssetsTools/AssetTypeClass.h"
#include "IULTPluginCommonInterface.h"
#include "IULTFontPluginInterface.h"
#include "GeneralPurposeFunctions.h"
#include "json/json.h"

UnityL10nToolAPI* UnityL10nToolAPIGlobal;
FontPluginInfo* FontPluginInfoGlobal;

FontAssetMaps fontAssetMapsGlobal;
vector<AssetMapOption> OptionsList;
Json::Value OptionsJson;
Json::Value ProjectConfig;

bool _cdecl SetProjectConfigJson(Json::Value pluginConfig) {
	ProjectConfig = pluginConfig;
	return true;
}

FontAssetMaps _cdecl GetPluginSupportAssetMap() {
	vector<FontAssetMap> FontAssetMapListFromResourcesAssets = GetFontAssetMapListFromMonoClassName(UnityL10nToolAPIGlobal, "resources.assets", "TMPro.TMP_FontAsset");
	vector<FontAssetMap> FontAssetMapListFromShared0Assets = GetFontAssetMapListFromMonoClassName(UnityL10nToolAPIGlobal, "sharedassets0.assets", "TMPro.TMP_FontAsset");
	fontAssetMapsGlobal.News.insert(fontAssetMapsGlobal.News.end(), FontAssetMapListFromResourcesAssets.begin(), FontAssetMapListFromResourcesAssets.end());
	fontAssetMapsGlobal.News.insert(fontAssetMapsGlobal.News.end(), FontAssetMapListFromShared0Assets.begin(), FontAssetMapListFromShared0Assets.end());
	for (vector<FontAssetMap>::iterator iterator = fontAssetMapsGlobal.News.begin();
		iterator != fontAssetMapsGlobal.News.end(); iterator++) {
		iterator->options = OptionsList;
	}

	/* Load Saveds */
	if (ProjectConfig.isMember("SelectedFontAssetMapList")) {
		Json::Value selectedFontAssetMapListJson = ProjectConfig["SelectedFontAssetMapList"];
		if (selectedFontAssetMapListJson.isArray()) {
			for (Json::ArrayIndex i = 0; i < selectedFontAssetMapListJson.size(); i++) {
				Json::Value selectedFontAssetMapJson = selectedFontAssetMapListJson[i];
				std::string assetsName;
				std::string assetName;
				std::string containerPath;
				std::string fontFamily;
				bool useContainerPath = false;

				if (selectedFontAssetMapJson["AssetsName"].isString()) {
					assetsName = selectedFontAssetMapJson["AssetsName"].asString();
				}
				if (selectedFontAssetMapJson["AssetName"].isString()) {
					assetName = selectedFontAssetMapJson["AssetName"].asString();
				}
				if (selectedFontAssetMapJson["ContainerPath"].isString()) {
					containerPath = selectedFontAssetMapJson["ContainerPath"].asString();
				}
				if (selectedFontAssetMapJson["UseContainerPath"].isBool()) {
					useContainerPath = selectedFontAssetMapJson["UseContainerPath"].asBool();
				}

				if (selectedFontAssetMapJson["Font Family"].isString()) {
					fontFamily = selectedFontAssetMapJson["Font Family"].asString();
				}

				for (vector<FontAssetMap>::iterator iterator = fontAssetMapsGlobal.News.begin();
					iterator != fontAssetMapsGlobal.News.end(); iterator++) {
					FontAssetMap fontAssetMap = *iterator;
					if (fontAssetMap.assetName == assetName &&
						fontAssetMap.assetsName == assetsName &&
						fontAssetMap.containerPath == containerPath) {
						iterator->useContainerPath = useContainerPath;
						iterator->options[0].Value = new wstring(WideMultiStringConverter.from_bytes(fontFamily));

						fontAssetMap.useContainerPath = useContainerPath;
						fontAssetMap.options = OptionsList;
						fontAssetMap.options[0].Value = new wstring(WideMultiStringConverter.from_bytes(fontFamily));
						fontAssetMapsGlobal.Saveds.push_back(fontAssetMap);
						break;
					}
				}
			}
		}
	}


	return fontAssetMapsGlobal;
}

Json::Value _cdecl GetProjectConfigJson() {
	Json::Value exportJson;
	for (vector<FontAssetMap>::iterator iterator = fontAssetMapsGlobal.Saveds.begin();
		iterator != fontAssetMapsGlobal.Saveds.end(); iterator++) {
		Json::Value tempSelectedFontAssetMapJson;
		tempSelectedFontAssetMapJson["AssetsName"] = iterator->assetsName;
		tempSelectedFontAssetMapJson["AssetName"] = iterator->assetName;
		tempSelectedFontAssetMapJson["ContainerPath"] = iterator->containerPath;
		//tempSelectedFontAssetMapJson["SelectedOption"] = WideMultiStringConverter.to_bytes(iterator->selectedOption);
		tempSelectedFontAssetMapJson["UseContainerPath"] = iterator->useContainerPath;
		tempSelectedFontAssetMapJson["Font Family"] = WideMultiStringConverter.to_bytes(*((wstring*)iterator->options[0].Value));
		exportJson["SelectedFontAssetMapList"].append(tempSelectedFontAssetMapJson);
	}

	return exportJson;
}

Json::Value _cdecl GetPacherConfigJson() {
	return GetProjectConfigJson();
}

bool _cdecl CopyBuildFileToBuildFolder(wstring FontPluginRelativePath, wstring targetPath) {
	CopyDirTo((FontPluginRelativePath + L"TMPFontAsset\\").c_str(), (targetPath + L"TMPFontAsset\\").c_str());
	/*CopyFileW((FontPluginRelativePath + L"TMPFontAsset").c_str(), (targetPath + L"").c_str(), false);*/
	return true;
}

Json::Value PatcherConfigGlobal;
bool _cdecl SetPacherConfigJson(Json::Value patcherConfig) {
	PatcherConfigGlobal = patcherConfig;
	return true;
}

AssetsReplacer* ReplaceMaterial(string assetsName, AssetsFileTable* assetsFileTable, AssetFileInfoEx* assetFileInfoEx, AssetTypeInstance* assetTypeInstance, float _TextureHeight, float _TextureWidth) {
	AssetTypeValueField* m_FloatsArrayATVF = assetTypeInstance->GetBaseField()->Get("m_SavedProperties")->Get("m_Floats")->Get("Array");
	AssetTypeValueField** m_FloatsATVFChildrenArray = m_FloatsArrayATVF->GetChildrenList();
	int modifyCounter = 2;
	for (unsigned int i = 0; i < m_FloatsArrayATVF->GetChildrenCount() && modifyCounter>0; i++) {
		string first = m_FloatsATVFChildrenArray[i]->Get("first")->GetValue()->AsString();
		if (first == "_TextureHeight") {
			m_FloatsATVFChildrenArray[i]->Get("second")->GetValue()->Set(new float(_TextureHeight));
			modifyCounter--;
		}
		else if (first == "_TextureWidth") {
			m_FloatsATVFChildrenArray[i]->Get("second")->GetValue()->Set(new float(_TextureWidth));
			modifyCounter--;
		}
	}
	return UnityL10nToolAPIGlobal->makeAssetsReplacer(assetsName, assetsFileTable, assetFileInfoEx, assetTypeInstance);
}

AssetsReplacer* ReplaceAtlas(string assetsname, AssetsFileTable* assetsFileTable, AssetFileInfoEx* assetFileInfoEx, AssetTypeInstance* assetTypeInstance, int m_CompleteImageSize, string atlasPath, int m_Width, int m_Height) {
	AssetTypeValueField* assetTypeValueField = assetTypeInstance->GetBaseField();
	assetTypeInstance->GetBaseField()->Get("m_Width")->GetValue()->Set(new INT32(m_Width));
	assetTypeInstance->GetBaseField()->Get("m_Height")->GetValue()->Set(new INT32(m_Height));
	assetTypeInstance->GetBaseField()->Get("m_CompleteImageSize")->GetValue()->Set(new INT32(m_CompleteImageSize));
	assetTypeInstance->GetBaseField()->Get("m_StreamData")->Get("offset")->GetValue()->Set(new UINT32(0));
	assetTypeInstance->GetBaseField()->Get("m_StreamData")->Get("size")->GetValue()->Set(new UINT32(m_CompleteImageSize));
	assetTypeInstance->GetBaseField()->Get("m_StreamData")->Get("path")->GetValue()->Set((void*)(new string("TMPFontAsset\\"+atlasPath))->c_str());
	return UnityL10nToolAPIGlobal->makeAssetsReplacer(assetsname, assetsFileTable, assetFileInfoEx, assetTypeInstance);
}

map<string, string> copyList;
map<string, vector<AssetsReplacer*>> _cdecl GetPatcherAssetReplacer() {
	map<string, vector<AssetsReplacer*>> replacers;
	Json::Value selectedFontAssetMapList = PatcherConfigGlobal["SelectedFontAssetMapList"];
	for (Json::ArrayIndex i = 0; i < selectedFontAssetMapList.size(); i++) {
		Json::Value assetLogicalReplacerJson = selectedFontAssetMapList[i];
		string assetsName = "";
		string assetName = "";
		string containerPath = "";
		string fontFamily = "";
		bool useContainerPath = false;

		if (assetLogicalReplacerJson["AssetsName"].isString()) {
			assetsName = assetLogicalReplacerJson["AssetsName"].asString();
		}
		if (assetLogicalReplacerJson["AssetName"].isString()) {
			assetName = assetLogicalReplacerJson["AssetName"].asString();
		}
		if (assetLogicalReplacerJson["ContainerPath"].isString()) {
			containerPath = assetLogicalReplacerJson["ContainerPath"].asString();
		}
		if (assetLogicalReplacerJson["UseContainerPath"].isBool()) {
			useContainerPath = assetLogicalReplacerJson["UseContainerPath"].asBool();
		}
		if (assetLogicalReplacerJson["Font Family"].isString()) {
			fontFamily = assetLogicalReplacerJson["Font Family"].asString();
		}
		Json::Value selectedOptionJson;
		if (!fontFamily.empty()) {
			string selectedOptionStr = readFile2(FontPluginInfoGlobal->relativePluginPath + L"TMPFontAsset\\" + WideMultiStringConverter.from_bytes(fontFamily) + L".json");
			JsonReader.parse(selectedOptionStr, selectedOptionJson);
		}

		if (((useContainerPath && !containerPath.empty())
			|| (!useContainerPath && !assetsName.empty() && !assetName.empty()))
			&& (selectedOptionJson.getMemberNames().size() != 0)) {
			string monoPath = "";
			// for material
			float _TextureHeight;
			float _TextureWidth;
			// for atlas
			int m_Width = 0;
			int m_Height = 0;
			int m_CompleteImageSize;
			string atlasPath = "";
			if (selectedOptionJson["monoPath"].isString()) {
				monoPath = selectedOptionJson["monoPath"].asString();
			}
			if (selectedOptionJson["_TextureHeight"].isDouble()) {
				_TextureHeight = selectedOptionJson["_TextureHeight"].asFloat();
			}
			if (selectedOptionJson["_TextureWidth"].isDouble()) {
				_TextureWidth = selectedOptionJson["_TextureWidth"].asFloat();
			}
			if (selectedOptionJson["m_Width"].isInt()) {
				m_Width = selectedOptionJson["m_Width"].asInt();
			}
			if (selectedOptionJson["m_Height"].isInt()) {
				m_Height = selectedOptionJson["m_Height"].asInt();
			}
			if (selectedOptionJson["m_CompleteImageSize"].isInt()) {
				m_CompleteImageSize = selectedOptionJson["m_CompleteImageSize"].asInt();
			}
			if (selectedOptionJson["atlasPath"].isString()) {
				atlasPath = selectedOptionJson["atlasPath"].asString();
			}

			string assetsFileName;
			AssetsFileTable* assetsFileTable;
			AssetFileInfoEx* assetFileInfoEx;
			AssetTypeInstance* assetTypeInstance;
			AssetTypeValueField* baseAssetTypeValueField;

			//INT64 PathId;
			try {
				if (useContainerPath) {
					map<string, pair<INT32, INT64>>::const_iterator FilePathIdIterator = UnityL10nToolAPIGlobal->FindFileIDPathIDFromContainerPath->find(containerPath);
					if (FilePathIdIterator == UnityL10nToolAPIGlobal->FindFileIDPathIDFromContainerPath->end()) {
						continue;
					}
					map<INT32, string>::const_iterator assetsFileNameIterator = UnityL10nToolAPIGlobal->FindAssetsNameFromPathIDOfContainerPath->find(FilePathIdIterator->second.first);
					if (assetsFileNameIterator == UnityL10nToolAPIGlobal->FindAssetsNameFromPathIDOfContainerPath->end()) {
						continue;
					}
					assetsFileName = assetsFileNameIterator->second;
					map<string, AssetsFileTable*>::const_iterator assetsFileTableIterator = UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->find(assetsFileName);
					if (assetsFileTableIterator == UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->end()) {
						continue;
					}
					assetsFileTable = assetsFileTableIterator->second;
					assetFileInfoEx = assetsFileTable->getAssetInfo(FilePathIdIterator->second.second);
				}
				else {
					assetsFileName = assetsName;
					map<string, AssetsFileTable*>::const_iterator assetsFileTableIterator = UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->find(assetsFileName);
					if (assetsFileTableIterator == UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->end()) {
						continue;
					}
					assetsFileTable = assetsFileTableIterator->second;
					INT64 assetIndex = UnityL10nToolAPIGlobal->FindAssetIndexFromName(assetsFileTable, assetName);
					if (assetIndex == -1) {
						continue;
					}
					assetFileInfoEx = &assetsFileTable->pAssetFileInfo[assetIndex];
				}
			}
			catch (exception e) {
				continue;
			}
			if (assetFileInfoEx == NULL) {
				continue;
			}
			assetTypeInstance = UnityL10nToolAPIGlobal->GetDetailAssetTypeInstanceFromAssetFileInfoEx(assetsFileTable, assetFileInfoEx);
			baseAssetTypeValueField = assetTypeInstance->GetBaseField();
			if (baseAssetTypeValueField->IsDummy())
			{
				continue;
			}
			string modifyStr = readFile2(FontPluginInfoGlobal->relativePluginPath + L"TMPFontAsset\\" + WideMultiStringConverter.from_bytes(monoPath));
			Json::Value modifyJson;
			JsonReader.parse(modifyStr, modifyJson);
			AssetsReplacer* monoAssetsReplacer = UnityL10nToolAPIGlobal->makeAssetsReplacer(assetsFileName, assetsFileTable, assetFileInfoEx, assetTypeInstance, modifyJson);

			INT64 materialPathId = baseAssetTypeValueField->Get("material")->Get("m_PathID")->GetValue()->AsInt64();
			AssetFileInfoEx* materialAssetFileInfoEx = assetsFileTable->getAssetInfo(materialPathId);
			AssetTypeInstance* materialAssetTypeInstance = UnityL10nToolAPIGlobal->GetDetailAssetTypeInstanceFromAssetFileInfoEx(assetsFileTable, materialAssetFileInfoEx);
			AssetsReplacer* materialAssetsReplacer = ReplaceMaterial(assetsFileName, assetsFileTable, materialAssetFileInfoEx, materialAssetTypeInstance, _TextureHeight, _TextureWidth);

			INT64 atlasPathId = baseAssetTypeValueField->Get("atlas")->Get("m_PathID")->GetValue()->AsInt64();
			AssetFileInfoEx* atlasAssetFileInfoEx = assetsFileTable->getAssetInfo(atlasPathId);
			AssetTypeInstance* atlasAssetTypeInstance = UnityL10nToolAPIGlobal->GetDetailAssetTypeInstanceFromAssetFileInfoEx(assetsFileTable, atlasAssetFileInfoEx);
			AssetsReplacer* atlasAssetsReplacer = ReplaceAtlas(assetsFileName, assetsFileTable, atlasAssetFileInfoEx, atlasAssetTypeInstance, m_CompleteImageSize, atlasPath, m_Width, m_Height);

			replacers[assetsFileName].push_back(monoAssetsReplacer);
			replacers[assetsFileName].push_back(materialAssetsReplacer);
			replacers[assetsFileName].push_back(atlasAssetsReplacer);
			copyList.insert(pair<string, string>(atlasPath, atlasPath));
		}
	}
	return replacers;
}

bool _cdecl CopyResourceFileToGameFolder(wstring FontPluginRelativePath, wstring targetPath) {
	if (CreateDirectoryW((targetPath + L"TMPFontAsset\\").c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
	}
	else {}
	for (map<string, string>::iterator iterator = copyList.begin(); iterator != copyList.end(); iterator++) {
		CopyFileW((FontPluginRelativePath + L"TMPFontAsset\\" + WideMultiStringConverter.from_bytes(iterator->first)).c_str(),
			(targetPath + L"TMPFontAsset\\" + WideMultiStringConverter.from_bytes(iterator->second)).c_str(), false);
	}
	//CopyFileW((FontPluginRelativePath + L"TMPFontAsset\\").c_str(), (targetPath + L"TMPFontAsset").c_str(), false);
	return true;
}

bool _cdecl SetPluginSupportAssetMap(FontAssetMaps supportAssetMaps) {
	fontAssetMapsGlobal = supportAssetMaps;
	return true;
}

bool _cdecl GetFontPluginInfo(UnityL10nToolAPI* unityL10nToolAPI, FontPluginInfo* fontPluginInfo) {
	UnityL10nToolAPIGlobal = unityL10nToolAPI;
	FontPluginInfoGlobal = fontPluginInfo;
	wcsncpy_s(fontPluginInfo->FontPluginName, L"TMPFontAsset", 12);
	fontPluginInfo->SetProjectConfigJson = SetProjectConfigJson;
	fontPluginInfo->GetPluginSupportAssetMap = GetPluginSupportAssetMap;
	fontPluginInfo->SetPluginSupportAssetMap = SetPluginSupportAssetMap;
	fontPluginInfo->GetProjectConfigJson = GetProjectConfigJson;
	fontPluginInfo->GetPacherConfigJson = GetPacherConfigJson;
	fontPluginInfo->CopyBuildFileToBuildFolder = CopyBuildFileToBuildFolder;

	fontPluginInfo->SetPacherConfigJson = SetPacherConfigJson;
	fontPluginInfo->GetPatcherAssetReplacer = GetPatcherAssetReplacer;
	fontPluginInfo->CopyResourceFileToGameFolder = CopyResourceFileToGameFolder;

	string optionsJsonStr = readFile2(fontPluginInfo->relativePluginPath + L"TMPFontAsset\\Options.json");
	JsonReader.parse(optionsJsonStr, OptionsJson);
	AssetMapOption assetMapOption = { 
		L"Font Family",
		L"Select Font Family Name to use",
		NULL,
		NULL,
		AssetMapOption::Type::OPTION_TYPE_WSTRING,
		AssetMapOption::Type::OPTION_TYPE_NONE,
		std::vector<AssetMapOption>()
	};
	if (OptionsJson.getMemberNames().size() != 0) {
		vector<string> tempOptionsList = OptionsJson.getMemberNames();
		for (vector<string>::iterator iterator = tempOptionsList.begin();
			iterator != tempOptionsList.end(); iterator++) {
			wstring* fontFamilyName = new wstring(WideMultiStringConverter.from_bytes(*iterator));
			AssetMapOption assetMapOptionFontFamilyEnum = {
				L"",
				L"",
				NULL,
				fontFamilyName,
				AssetMapOption::Type::OPTION_TYPE_NONE,
				AssetMapOption::Type::OPTION_TYPE_WSTRING,
				std::vector<AssetMapOption>()
			};
			assetMapOption.nestedOptions.push_back(assetMapOptionFontFamilyEnum);
		}
		OptionsList.push_back(assetMapOption);
	}
	else {
		//장차 없에고, 옵션이 없으면 오류를 내는 쪽으로
		throw exception("OptionsJson error");
	}
	return true;
}