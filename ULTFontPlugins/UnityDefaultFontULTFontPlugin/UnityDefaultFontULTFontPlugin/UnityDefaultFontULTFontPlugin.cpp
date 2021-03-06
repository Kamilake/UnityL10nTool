// UnityDefaultFontULTFontPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include <string>
#include <set>
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

class UnityDefaultFontResource {
public:
	enum ResourceTypeEnum {
		NONE = 0,
		TTF = 1,
		JSON = 2,
		BITMAP = 3
	};
	wstring FileName;
	wstring FontFamilyName;
	ResourceTypeEnum ResourceType;
	UnityDefaultFontResource() {}
	UnityDefaultFontResource(wstring fileName) {
		this->FileName = fileName;
		if (this->FileName.find(L".ttf") != wstring::npos) {
			size_t ttfFound = this->FileName.find(L".ttf");
			this->FontFamilyName = this->FileName.substr(0, ttfFound) + L" (TTF)";
			this->ResourceType = TTF;
		}
		else if (this->FileName.find(L".json") != wstring::npos) {
			size_t jsonFound = this->FileName.find(L".json");
			this->FontFamilyName = this->FileName.substr(0, jsonFound) + L" (json)";
			this->ResourceType = JSON;
		}
		else if (this->FileName.find(L".fnt") != wstring::npos) {
			size_t fntFound = this->FileName.find(L".fnt");
			this->FontFamilyName = this->FileName.substr(0, fntFound) + L" (Bitmap)";
			this->ResourceType = BITMAP;
		}
		else {
			this->ResourceType = NONE;
		}
	}
};

bool _cdecl SetProjectConfigJson(Json::Value pluginConfig) {
	ProjectConfig = pluginConfig;
	return true;
}

vector<FontAssetMap> GetFontAssetMapListFromUnityDefaultFont(UnityL10nToolAPI* unityL10nToolAPI, string assetsName) {
	vector<FontAssetMap> fontAssetMapList;
	const string FontClassName = "Font";
	map<string, UINT32>::const_iterator FontClassIndexItr = unityL10nToolAPI->FindBasicClassIndexFromClassName->find(FontClassName);
	if (FontClassIndexItr != unityL10nToolAPI->FindBasicClassIndexFromClassName->end()) {
		UINT32 FontClassIndex = FontClassIndexItr->second;
		ClassDatabaseType FontAssetCDT = unityL10nToolAPI->BasicClassDatabaseFile->classes[FontClassIndex];
		const int FontAssetClassId = FontAssetCDT.classId;

		map<string, AssetsFileTable*>::const_iterator assetsFileTableIterator = unityL10nToolAPI->FindAssetsFileTablesFromAssetsName->find(assetsName);
		if (assetsFileTableIterator != unityL10nToolAPI->FindAssetsFileTablesFromAssetsName->end()) {
			AssetsFileTable* assetsFileTable = assetsFileTableIterator->second;
			AssetsFile* assetsFile = assetsFileTable->getAssetsFile();
			for (unsigned int i = 0; i < assetsFileTable->assetFileInfoCount; i++) {
				AssetFileInfoEx* assetFileInfoEx = &assetsFileTable->pAssetFileInfo[i];
				int classId = 0;
				UINT16 monoClassId = 0;
				unityL10nToolAPI->GetClassIdFromAssetFileInfoEx(assetsFileTable, assetFileInfoEx, classId, monoClassId);
				if (classId == FontAssetClassId) {
					string assetName = unityL10nToolAPI->GetAssetNameA(assetsFile, assetFileInfoEx);
					INT64 PathId = assetFileInfoEx->index;
					string containerPath;
					map<string, INT32>::const_iterator FileIDIterator = unityL10nToolAPI->FindPathIDOfContainerPathFromAssetsName->find(assetsName);
					if (FileIDIterator != unityL10nToolAPI->FindPathIDOfContainerPathFromAssetsName->end()) {
						map<pair<INT32, INT64>, string>::const_iterator containerPathItr = unityL10nToolAPI->FindContainerPathFromFileIDPathID->find(pair<INT32, INT64>(FileIDIterator->second, PathId));
						if (containerPathItr != unityL10nToolAPI->FindContainerPathFromFileIDPathID->end()) {
							containerPath = containerPathItr->second;
						}
					}
					FontAssetMap fontAssetMap = FontAssetMap(
						L"",
						assetsName,
						assetName,
						containerPath,
						false,
						std::vector<AssetMapOption>()
					);
					fontAssetMap._m_PathID = assetFileInfoEx->index;
					fontAssetMapList.push_back(fontAssetMap);
				}
			}
		}
	}
	return fontAssetMapList;
}

map<wstring, UnityDefaultFontResource> UnityDefaultFontResources;

FontAssetMaps _cdecl GetPluginSupportAssetMap() {
	for (vector<string>::iterator assetsNameItr = UnityL10nToolAPIGlobal->AssetsFileNames->begin(); assetsNameItr != UnityL10nToolAPIGlobal->AssetsFileNames->end(); assetsNameItr++) {
		size_t globalgamemanagerFound = assetsNameItr->find("globalgamemanagers");
		size_t sharedFound = assetsNameItr->find("sharedassets");
		size_t resourcesFound = assetsNameItr->find("resources.assets");
		if (sharedFound != string::npos || resourcesFound != string::npos || globalgamemanagerFound != string::npos) {
			string assetsName = *assetsNameItr;
			vector<FontAssetMap> foundFontAssetMapList = GetFontAssetMapListFromUnityDefaultFont(UnityL10nToolAPIGlobal, assetsName);
			for (vector<FontAssetMap>::iterator FAMItr = foundFontAssetMapList.begin(); FAMItr != foundFontAssetMapList.end(); FAMItr++) {
				FAMItr->options = OptionsList;
				fontAssetMapsGlobal.News.push_back(*FAMItr);
			}
		}
	}
	if (ProjectConfig.isMember("Saveds")) {
		Json::Value savedJsonArray = ProjectConfig["Saveds"];
		if (savedJsonArray.isArray()) {
			for (Json::ArrayIndex i = 0; i < savedJsonArray.size(); i++) {
				FontAssetMap savedFAM((Json::Value)savedJsonArray[i]);
				for (vector<FontAssetMap>::iterator FAMItr = fontAssetMapsGlobal.News.begin(); FAMItr != fontAssetMapsGlobal.News.end(); FAMItr++) {
					if (FAMItr->LooseEquals(savedFAM)) {
						wstring savedFontFamilyName = *(wstring*)savedFAM.options[0].Value;
						if (UnityDefaultFontResources.find(savedFontFamilyName) != UnityDefaultFontResources.end()) {
							FAMItr->options[0].Value = new wstring(savedFontFamilyName);
							FAMItr->useContainerPath = savedFAM.useContainerPath;
							fontAssetMapsGlobal.Saveds.push_back(*FAMItr);
						}
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

	Json::Value SavedsJsonArray(Json::arrayValue);
	for (vector<FontAssetMap>::iterator FAMItr = fontAssetMapsGlobal.Saveds.begin(); FAMItr != fontAssetMapsGlobal.Saveds.end(); FAMItr++) {
		SavedsJsonArray.append(FAMItr->ToJson());
	}
	exportJson["Saveds"] = SavedsJsonArray;
	return exportJson;
}

Json::Value _cdecl GetPatcherConfigJson() {
	return GetProjectConfigJson();
}

Json::Value PatcherConfigGlobal;
bool _cdecl SetPatcherConfigJson(Json::Value patcherConfig) {
	PatcherConfigGlobal = patcherConfig;
	return true;
}

bool _cdecl SetPluginSupportAssetMap(FontAssetMaps supportAssetMaps) {
	fontAssetMapsGlobal = supportAssetMaps;
	return true;
}

bool _cdecl CopyBuildFileToBuildFolder(wstring FontPluginRelativePath, wstring targetPath) {
	set<wstring> usedFontResources;
	for (vector<FontAssetMap>::iterator FAMItr = fontAssetMapsGlobal.Saveds.begin(); FAMItr != fontAssetMapsGlobal.Saveds.end(); FAMItr++) {
		wstring usedFontFamilyName = *(wstring*)FAMItr->options[0].Value;
		if (UnityDefaultFontResources.find(usedFontFamilyName) != UnityDefaultFontResources.end()) {
			usedFontResources.insert(usedFontFamilyName);
		}
	}
	if (usedFontResources.size() != 0) {
		CreateDirectoryW((targetPath + L"UnityDefaultFont\\").c_str(), NULL);
		for (set<wstring>::iterator usedFontResourceItr = usedFontResources.begin(); usedFontResourceItr != usedFontResources.end(); usedFontResourceItr++) {
			map<wstring, UnityDefaultFontResource>::iterator UnityDefaultFontResourceItr = UnityDefaultFontResources.find(*usedFontResourceItr);
			if (UnityDefaultFontResourceItr != UnityDefaultFontResources.end()) {
				UnityDefaultFontResource nUnityDefaultFontResource = UnityDefaultFontResourceItr->second;
				switch (nUnityDefaultFontResource.ResourceType) {
				case UnityDefaultFontResource::TTF:
					CopyFileW((FontPluginRelativePath + L"UnityDefaultFont\\" + nUnityDefaultFontResource.FileName).c_str(), (targetPath + L"UnityDefaultFont\\" + nUnityDefaultFontResource.FileName).c_str(), false);
					break;
				case UnityDefaultFontResource::JSON:
				case UnityDefaultFontResource::BITMAP:
				case UnityDefaultFontResource::NONE:
					break;
				}
			}
		}
	}
	return true;
}


AssetsReplacer* GetAssetReplacerForTTF(UnityL10nToolAPI* unityL10nToolAPI, AssetsFileTable* assetsFileTable, AssetFileInfoEx* fontAFIEx, UnityDefaultFontResource unityDefaultFontResource) {
	AssetTypeTemplateField templateBase;
	int classId;
	WORD monoClassId;
	unityL10nToolAPI->GetClassIdFromAssetFileInfoEx(assetsFileTable, fontAFIEx, classId, monoClassId);
	templateBase.FromClassDatabase(unityL10nToolAPI->BasicClassDatabaseFile, &unityL10nToolAPI->BasicClassDatabaseFile->classes[unityL10nToolAPI->FindBasicClassIndexFromClassID->find(classId)->second], (DWORD)0, false);

	for (DWORD i = 0; i < templateBase.childrenCount; i++)
	{
		if (!strcmp(templateBase.children[i].name, "m_FontData"))
		{
			templateBase.children[i].type = "_vector";
			templateBase.children[i].valueType = ValueType_None;
			for (DWORD k = 0; k < templateBase.children[i].childrenCount; k++)
			{
				if (!strcmp(templateBase.children[i].children[k].name, "Array"))
				{
					templateBase.children[i].children[k].type = "TypelessData";
					break;
				}
			}
			break;
		}
	}
	AssetTypeTemplateField *pTemplateBase = &templateBase;
	AssetTypeInstance* baseAssetTypeInstance = new AssetTypeInstance(
		(DWORD)1,
		&pTemplateBase,
		fontAFIEx->curFileSize,
		assetsFileTable->getReader(),
		assetsFileTable->getAssetsFile()->header.endianness ? true : false,
		fontAFIEx->absolutePos);
	AssetTypeValueField *pBase = baseAssetTypeInstance->GetBaseField();
	if (pBase) {
		AssetTypeValueField* m_ConvertCaseATVF = pBase->Get("m_ConvertCase");
		m_ConvertCaseATVF->GetValue()->Set(new INT32(-2));

		for (unsigned int i = 0; i < pBase->GetChildrenCount(); i++) {
			AssetTypeValueField* ATVF = pBase->GetChildrenList()[i];
			if (strcmp(ATVF->GetName(), "m_CharacterRects") == 0) {
				AssetTypeValueField* arrayATVF = ATVF->GetChildrenList()[0];
				AssetTypeTemplateField* m_CharacterRectsATTF = arrayATVF->GetTemplateField();
				std::vector<AssetTypeValueField*>* m_CharacterRectsATVFs = new std::vector<AssetTypeValueField*>();
				AssetTypeArray* m_CharacterRectsATA = new AssetTypeArray();
				m_CharacterRectsATA->size = 0;
				AssetTypeValueField* m_CharacterRectsATVF = new AssetTypeValueField();
				m_CharacterRectsATVF->Read(new AssetTypeValue(ValueType_Array, m_CharacterRectsATA), m_CharacterRectsATTF, m_CharacterRectsATVFs->size(), m_CharacterRectsATVFs->data());
				ATVF->GetChildrenList()[0] = m_CharacterRectsATVF;
				break;
			}
		}
		
		AssetTypeValueField * m_FontDataATVF = pBase->Get("m_FontData");
		AssetTypeValueField* dataArrayField = m_FontDataATVF->Get("Array");
		if (dataArrayField && !dataArrayField->IsDummy()) {
			HANDLE fontFILE = CreateFileW(
				(FontPluginInfoGlobal->relativePluginPath + L"UnityDefaultFont\\" + unityDefaultFontResource.FileName).c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			DWORD fileSize = GetFileSize(fontFILE, NULL);
			LPVOID buffer = GlobalAlloc(GPTR, fileSize);
			DWORD readResult;
			ReadFile(fontFILE, buffer, fileSize, &readResult, NULL);
			AssetTypeByteArray byteArrayData;
			byteArrayData.size = fileSize;
			byteArrayData.data = (BYTE*)buffer;
			dataArrayField->GetValue()->Set(&byteArrayData);
			AssetsReplacer* assetsReplacer = unityL10nToolAPI->makeAssetsReplacer(assetsFileTable, fontAFIEx, baseAssetTypeInstance);
			return assetsReplacer;
		}
	}
	return NULL;
}

set<wstring> usedFontResourcesInPatcher;
map<string, vector<AssetsReplacer*>> _cdecl GetPatcherAssetReplacer() {
	map<string, vector<AssetsReplacer*>> replacers;
	if (PatcherConfigGlobal.isMember("Saveds")) {
		Json::Value savedFontArrayJson = PatcherConfigGlobal["Saveds"];
		for (Json::ArrayIndex i = 0; i < savedFontArrayJson.size(); i++) {
			FontAssetMap fontAssetMap = FontAssetMap((Json::Value)savedFontArrayJson[i]);
			wstring usedFontFamilyName = *(wstring*)fontAssetMap.options[0].Value;
			map<wstring, UnityDefaultFontResource>::iterator foundUnityDefaultFont = UnityDefaultFontResources.find(usedFontFamilyName);
			if (foundUnityDefaultFont != UnityDefaultFontResources.end()) {
				UnityDefaultFontResource unityDefaultFontResource = foundUnityDefaultFont->second;
				string assetsName;
				AssetsFileTable* assetsFileTable = NULL;
				AssetFileInfoEx* fontAFIEx = NULL;
				AssetTypeInstance* fontATI = NULL;
				AssetTypeValueField* fontBaseATVF = NULL;
				INT64 PathID = 0;

				if (fontAssetMap.useContainerPath && !fontAssetMap.containerPath.empty()) {
					map<string, pair<INT32, INT64>>::const_iterator FileIDPathIDiterator = UnityL10nToolAPIGlobal->FindFileIDPathIDFromContainerPath->find(fontAssetMap.containerPath);
					if (FileIDPathIDiterator != UnityL10nToolAPIGlobal->FindFileIDPathIDFromContainerPath->end()) {
						INT32 FileIDOfContainerPath = FileIDPathIDiterator->second.first;
						PathID = FileIDPathIDiterator->second.second;
						map<INT32, string>::const_iterator assetsNameIterator = UnityL10nToolAPIGlobal->FindAssetsNameFromPathIDOfContainerPath->find(FileIDOfContainerPath);
						if (assetsNameIterator != UnityL10nToolAPIGlobal->FindAssetsNameFromPathIDOfContainerPath->end()) {
							assetsName = assetsNameIterator->second;
							map<string, AssetsFileTable*>::const_iterator assetsFileTableIterator = UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->find(assetsName);
							if (assetsFileTableIterator != UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->end()) {
								assetsFileTable = assetsFileTableIterator->second;
								fontAFIEx = assetsFileTable->getAssetInfo(PathID);
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}
					else {
						break;
					}
				}
				else if (!fontAssetMap.assetName.empty() && !fontAssetMap.assetsName.empty()) {
					assetsName = fontAssetMap.assetsName;
					map<string, AssetsFileTable*>::const_iterator assetsFileTableIterator = UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->find(assetsName);
					if (assetsFileTableIterator != UnityL10nToolAPIGlobal->FindAssetsFileTablesFromAssetsName->end()) {
						assetsFileTable = assetsFileTableIterator->second;
						PathID = UnityL10nToolAPIGlobal->FindAssetIndexFromName(assetsFileTable, fontAssetMap.assetName);
						if (PathID == -1) {
							break;
						}
						fontAFIEx = assetsFileTable->getAssetInfo(PathID);
					}
					else {
						break;
					}
				}
				else {
					break;
				}
				
				AssetsReplacer* assetsReplacer = NULL;
				switch (unityDefaultFontResource.ResourceType)
				{
				case UnityDefaultFontResource::TTF:
					assetsReplacer = GetAssetReplacerForTTF(UnityL10nToolAPIGlobal, assetsFileTable, fontAFIEx, unityDefaultFontResource);
					replacers[assetsName].push_back(assetsReplacer);
					break;
				default:
					break;
				}
			}
		}
	}
	return replacers;
}

bool _cdecl CopyResourceFileToGameFolder(wstring FontPluginRelativePath, wstring targetPath) {
	if (usedFontResourcesInPatcher.size() != 0) {

	}
	return true;
}


bool _cdecl GetFontPluginInfo(UnityL10nToolAPI* unityL10nToolAPI, FontPluginInfo* fontPluginInfo) {
	UnityL10nToolAPIGlobal = unityL10nToolAPI;
	FontPluginInfoGlobal = fontPluginInfo;
	wcsncpy_s(fontPluginInfo->FontPluginName, L"UnityDefaultFont", 16);
	fontPluginInfo->SetProjectConfigJson = SetProjectConfigJson;
	fontPluginInfo->GetPluginSupportAssetMap = GetPluginSupportAssetMap;
	fontPluginInfo->SetPluginSupportAssetMap = SetPluginSupportAssetMap;
	fontPluginInfo->GetProjectConfigJson = GetProjectConfigJson;
	fontPluginInfo->GetPacherConfigJson = GetPatcherConfigJson;
	fontPluginInfo->CopyBuildFileToBuildFolder = CopyBuildFileToBuildFolder;

	fontPluginInfo->SetPacherConfigJson = SetPatcherConfigJson;
	fontPluginInfo->GetPatcherAssetReplacer = GetPatcherAssetReplacer;
	fontPluginInfo->CopyResourceFileToGameFolder = CopyResourceFileToGameFolder;

	AssetMapOption assetMapOption = AssetMapOption(
		L"Font Family",
		L"Select Font Family Name to use",
		NULL,
		NULL,
		AssetMapOption::Type::OPTION_TYPE_WSTRING,
		AssetMapOption::Type::OPTION_TYPE_NONE,
		vector<AssetMapOption>()
	);

	vector<wstring> ttfNameList = get_all_files_names_within_folder(fontPluginInfo->relativePluginPath + L"UnityDefaultFont\\*.ttf");
	for (vector<wstring>::iterator ttfFontNameItr = ttfNameList.begin(); ttfFontNameItr != ttfNameList.end(); ttfFontNameItr++) {
		UnityDefaultFontResource unityDefaultFontResource = UnityDefaultFontResource(*ttfFontNameItr);
		UnityDefaultFontResources.insert(pair<wstring, UnityDefaultFontResource>(unityDefaultFontResource.FontFamilyName, unityDefaultFontResource));
		AssetMapOption assetMapOptionFontFamilyEnum = AssetMapOption(
			L"",
			L"",
			NULL,
			new wstring(unityDefaultFontResource.FontFamilyName),
			AssetMapOption::Type::OPTION_TYPE_NONE,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			std::vector<AssetMapOption>()
		);
		assetMapOption.nestedOptions.push_back(assetMapOptionFontFamilyEnum);
	}
	if (assetMapOption.nestedOptions.size() == 0) {
		AssetMapOption assetMapOptionFontFamilyEnum = AssetMapOption(
			L"",
			L"",
			NULL,
			new wstring(L"(Cannot load Font Resources."),
			AssetMapOption::Type::OPTION_TYPE_NONE,
			AssetMapOption::Type::OPTION_TYPE_WSTRING,
			std::vector<AssetMapOption>()
		);
	}
	OptionsList.push_back(assetMapOption);
	return true;
}