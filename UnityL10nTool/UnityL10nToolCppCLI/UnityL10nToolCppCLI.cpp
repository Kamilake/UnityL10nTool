#include "stdafx.h"
#include <vcclr.h>
#include "UnityL10nToolCppCLI.h"
#include "../UnityL10nToolAPI/inc/IULTFontPluginInterface.h"

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::DetermineUnityGameFolder(String^ path) {
	pin_ptr<const wchar_t> ptr = PtrToStringChars(path);
	return UnityL10nToolCpp::DetermineUnityGameFolder(ptr);
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::DetermineProjectGamePath(String ^ path, String ^ GameName, String ^ MakerName)
{
	pin_ptr<const wchar_t> ptrpath = PtrToStringChars(path);
	pin_ptr<const wchar_t> ptrGameName = PtrToStringChars(GameName);
	pin_ptr<const wchar_t> ptrMakerName = PtrToStringChars(MakerName);
	return UnityL10nToolCpp::DetermineProjectGamePath(ptrpath, ptrGameName, ptrMakerName);
	return false;
}

String^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::FindUnityGameFolderFromDataFolderName(String^ dataFolderName, String^ GameName, String^ MakerName) {
	pin_ptr<const wchar_t> ptrdataFolderName = PtrToStringChars(dataFolderName);
	pin_ptr<const wchar_t> ptrGameName = PtrToStringChars(GameName);
	pin_ptr<const wchar_t> ptrMakerName = PtrToStringChars(MakerName);
	wstring result = UnityL10nToolCpp::FindUnityGameFolderFromDataFolderName(ptrdataFolderName, ptrGameName, ptrMakerName);
	return gcnew String(result.c_str());
}

UnityL10nToolCppCLI::UnityL10nToolCppManaged::UnityL10nToolCppManaged(String ^ gameFolderPath)
{
	pin_ptr<const wchar_t> ptr = PtrToStringChars(gameFolderPath);
	unityL10nToolCpp = new UnityL10nToolCpp(ptr);
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadGlobalgamemanagersFile() {
	return unityL10nToolCpp->LoadGlobalgamemanagersFile();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::ProcessResourceAndMonoManger() {
	return unityL10nToolCpp->ProcessResourceAndMonoManger();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadMonoClassDatabase() {
	return unityL10nToolCpp->LoadMonoClassDatabase();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadUnityL10nToolAPI()
{
	return unityL10nToolCpp->LoadUnityL10nToolAPI();
}

List<String^>^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadFontPlugins()
{
	vector<wstring> tempLoadedFontPlugins = unityL10nToolCpp->LoadFontPlugins();
	List<String^>^ loadedFontPlugins = gcnew List<String^>();
	for (vector<wstring>::iterator iterator = tempLoadedFontPlugins.begin();
		iterator != tempLoadedFontPlugins.end(); iterator++) {
		loadedFontPlugins->Add(gcnew String(iterator->c_str()));
	}
	return loadedFontPlugins;
}

Dictionary<String^, UnityL10nToolCppCLI::FontAssetMapsCLI^>^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetPluginsSupportAssetMap()
{
	map<wstring, FontAssetMaps> u_pluginsSupportAssetMap = unityL10nToolCpp->GetPluginsSupportAssetMap();
	Dictionary<String^, FontAssetMapsCLI^>^ result = gcnew Dictionary<String^, FontAssetMapsCLI^>(u_pluginsSupportAssetMap.size());
	for(map<wstring, FontAssetMaps>::iterator keyValueIterator = u_pluginsSupportAssetMap.begin();
		keyValueIterator != u_pluginsSupportAssetMap.end(); keyValueIterator++) {
		FontAssetMapsCLI^ tempFontAssetMaps = gcnew FontAssetMapsCLI();
		tempFontAssetMaps->News = gcnew ObservableCollection<FontAssetMapCLI^>();
		for (vector<FontAssetMap>::iterator fontAssetMapIterator = keyValueIterator->second.News.begin();
			fontAssetMapIterator != keyValueIterator->second.News.end(); fontAssetMapIterator++) {
			FontAssetMapCLI^ fontAssetMapCLI = gcnew FontAssetMapCLI();
			fontAssetMapCLI->assetsName = gcnew String(fontAssetMapIterator->assetsName.c_str());
			fontAssetMapCLI->assetName = gcnew String(fontAssetMapIterator->assetName.c_str());
			fontAssetMapCLI->containerPath = gcnew String(fontAssetMapIterator->containerPath.c_str());
			fontAssetMapCLI->useContainerPath = fontAssetMapIterator->useContainerPath;
			fontAssetMapCLI->options = gcnew List<AssetMapOptionCLI^>();
			for (vector<AssetMapOption>::iterator optionIterator = fontAssetMapIterator->options.begin();
				optionIterator != fontAssetMapIterator->options.end(); optionIterator++) {
				fontAssetMapCLI->options->Add(gcnew AssetMapOptionCLI(*optionIterator));
			}
			tempFontAssetMaps->News->Add(fontAssetMapCLI);
		}
		tempFontAssetMaps->Saveds = gcnew ObservableCollection<FontAssetMapCLI^>();
		for (vector<FontAssetMap>::iterator fontAssetMapIterator = keyValueIterator->second.Saveds.begin();
			fontAssetMapIterator != keyValueIterator->second.Saveds.end(); fontAssetMapIterator++) {
			FontAssetMapCLI^ fontAssetMapCLI = gcnew FontAssetMapCLI();
			fontAssetMapCLI->assetsName = gcnew String(fontAssetMapIterator->assetsName.c_str());
			fontAssetMapCLI->assetName = gcnew String(fontAssetMapIterator->assetName.c_str());
			fontAssetMapCLI->containerPath = gcnew String(fontAssetMapIterator->containerPath.c_str());
			fontAssetMapCLI->useContainerPath = fontAssetMapIterator->useContainerPath;
			fontAssetMapCLI->options = gcnew List<AssetMapOptionCLI^>();
			for (vector<AssetMapOption>::iterator optionIterator = fontAssetMapIterator->options.begin();
				optionIterator != fontAssetMapIterator->options.end(); optionIterator++) {
				fontAssetMapCLI->options->Add(gcnew AssetMapOptionCLI(*optionIterator));
			}
			tempFontAssetMaps->Saveds->Add(fontAssetMapCLI);
		}
		result->Add(gcnew String(keyValueIterator->first.c_str()), tempFontAssetMaps);
	}
	return result;
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::SetPluginsSupportAssetMap(Dictionary<String^, FontAssetMapsCLI^>^ pluginsSupportAssetMapsCLI)
{
	map<wstring, FontAssetMaps> u_pluginsSupportAssetMap;
	for each(KeyValuePair<String^, FontAssetMapsCLI^> item in pluginsSupportAssetMapsCLI) {
		pin_ptr<const wchar_t> ptrKey = PtrToStringChars(item.Key);
		wstring keyWS = wstring(ptrKey);
		u_pluginsSupportAssetMap[keyWS] = ConvertFontAssetMapsFromCLI(item.Value);
	}
	return unityL10nToolCpp->SetPluginsSupportAssetMap(u_pluginsSupportAssetMap);
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetProjectConfigJsonFromFontPlugin()
{
	return unityL10nToolCpp->GetProjectConfigJsonFromFontPlugin();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::SetTextPluginConfigToJsonValue()
{
	return unityL10nToolCpp->SetTextPluginConfigToJsonValue();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::SaveProjectConfigJson()
{
	return unityL10nToolCpp->SaveProjectConfigJson();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::BuildProject(String ^ buildTargetFolder)
{
	pin_ptr<const wchar_t> ptrbuildTargetFolder = PtrToStringChars(buildTargetFolder);
	return unityL10nToolCpp->BuildProject(ptrbuildTargetFolder);
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::SetPacherConfigJson()
{
	return unityL10nToolCpp->SetPacherConfigJson();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadAssetsReplacer()
{
	return unityL10nToolCpp->LoadAssetsReplacer();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::MakeModifiedAssetsFile()
{
	return unityL10nToolCpp->MakeModifiedAssetsFile();
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::CopyResourceFileToGameFolder()
{
	return unityL10nToolCpp->CopyResourceFileToGameFolder();
}

UnityL10nToolCppCLI::TextAssetMapsCLI^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetTextAssetMaps()
{
	return gcnew TextAssetMapsCLI(unityL10nToolCpp->GetTextAssetMaps());
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::SetTextAssetMaps(TextAssetMapCLI^ textAssetMapCLI, TextAssetMapCLI::ToWhere toWhereCLI)
{
	TextAssetMap::ToWhere toWhere;
	switch (toWhereCLI) {
	case TextAssetMapCLI::ToWhere::ToInteractWithAsset:
		toWhere = TextAssetMap::ToWhere::ToInteractWithAsset;
		break;
	case TextAssetMapCLI::ToWhere::ToInteractWithFileText:
		toWhere = TextAssetMap::ToWhere::ToInteractWithFileText;
		break;
	case TextAssetMapCLI::ToWhere::ToInteractWithMonoAsset:
		toWhere = TextAssetMap::ToWhere::ToInteractWithMonoAsset;
		break;
	case TextAssetMapCLI::ToWhere::ToDone:
		toWhere = TextAssetMap::ToWhere::ToDone;
		break;
	}
	return unityL10nToolCpp->SetTextAssetMaps(textAssetMapCLI->ToUnmanaged(), toWhere);
}

bool UnityL10nToolCppCLI::UnityL10nToolCppManaged::LoadTextPlugins()
{
	return unityL10nToolCpp->LoadTextPlugins();
}

List<String^>^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetInteractWithAssetPluginNames()
{
	List<String^>^ results = gcnew List<String^>();
	vector<wstring> resultsUnmanaged = unityL10nToolCpp->GetInteractWithAssetPluginNames();
	for (vector<wstring>::iterator iterator = resultsUnmanaged.begin();
		iterator != resultsUnmanaged.end(); iterator++) {
		results->Add(ConvertFromWstringToManagedString(*iterator));
	}
	return results;
}

List<String^>^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetInteractWithFileTextPluginNames()
{
	List<String^>^ results = gcnew List<String^>();
	vector<wstring> resultsUnmanaged = unityL10nToolCpp->GetInteractWithFileTextPluginNames();
	for (vector<wstring>::iterator iterator = resultsUnmanaged.begin();
		iterator != resultsUnmanaged.end(); iterator++) {
		results->Add(ConvertFromWstringToManagedString(*iterator));
	}
	return results;
}

//String ^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetOriginalText(TextAssetMapCLI ^ textAssetMap)
//{
//	return ConvertFromWstringToManagedString(unityL10nToolCpp->GetOriginalText(textAssetMap->ToUnmanaged()));
//	// TODO: 여기에 반환 구문을 삽입합니다.
//}

UnityL10nToolCppCLI::TextAssetMapCLI ^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetOriginalLanguagePairDics(TextAssetMapCLI^ textAssetMapCLI)
{
	return gcnew TextAssetMapCLI(unityL10nToolCpp->GetOriginalLanguagePairDics(textAssetMapCLI->ToUnmanaged()));
}

UnityL10nToolCppCLI::TextAssetMapCLI ^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetTranslatedText(TextAssetMapCLI ^ textAssetMapCLI)
{
	return gcnew TextAssetMapCLI(unityL10nToolCpp->GetTranslatedText(textAssetMapCLI->ToUnmanaged()));
}

UnityL10nToolCppCLI::TextAssetMapCLI ^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetUpdateFileText(TextAssetMapCLI ^ textAssetMapCLI)
{
	return gcnew TextAssetMapCLI(unityL10nToolCpp->GetUpdateFileText(textAssetMapCLI->ToUnmanaged()));
}

UnityL10nToolCppCLI::TextAssetMapCLI ^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetTranslatedLanguagePairDics(TextAssetMapCLI ^ textAssetMapCLI)
{
	return gcnew TextAssetMapCLI(unityL10nToolCpp->GetTranslatedLanguagePairDics(textAssetMapCLI->ToUnmanaged()));
}

UnityL10nToolCppCLI::UnityL10nToolCppManaged::~UnityL10nToolCppManaged() {
	unityL10nToolCpp->~UnityL10nToolCpp();
	unityL10nToolCpp = NULL;
}

FontAssetMaps UnityL10nToolCppCLI::ConvertFontAssetMapsFromCLI(UnityL10nToolCppCLI::FontAssetMapsCLI^ fontAssetMapsCLI) {
	FontAssetMaps fontAssetMaps;
	for each(FontAssetMapCLI^ fontAssetMapCLI in fontAssetMapsCLI->News) {
		fontAssetMaps.News.push_back(ConvertFontAssetMapFromCLI(fontAssetMapCLI));
	}
	for each(FontAssetMapCLI^ fontAssetMapCLI in fontAssetMapsCLI->Saveds) {
		fontAssetMaps.Saveds.push_back(ConvertFontAssetMapFromCLI(fontAssetMapCLI));
	}
	return fontAssetMaps;
}

FontAssetMap UnityL10nToolCppCLI::ConvertFontAssetMapFromCLI(UnityL10nToolCppCLI::FontAssetMapCLI^ fontAssetMapCLI) {
	FontAssetMap fontAssetMap;
	char* ptrAssetsName = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fontAssetMapCLI->assetsName).ToPointer();
	char* ptrAssetName = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fontAssetMapCLI->assetName).ToPointer();
	char* ptrContainerPath = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fontAssetMapCLI->containerPath).ToPointer();
	fontAssetMap.assetsName = ptrAssetsName;
	fontAssetMap.assetName = ptrAssetName;
	fontAssetMap.containerPath = ptrContainerPath;
	fontAssetMap.useContainerPath = fontAssetMapCLI->useContainerPath;
	for each(AssetMapOptionCLI^ fontAssetMapOptionCLI in fontAssetMapCLI->options) {
		fontAssetMap.options.push_back(fontAssetMapOptionCLI->ToUnmanaged());
	}
	return fontAssetMap;
}


UnityL10nToolCppCLI::FontAssetMapCLI^ UnityL10nToolCppCLI::FontAssetMapCLI::Clone() {
	FontAssetMapCLI^ newFontAssetMapCLI = gcnew FontAssetMapCLI();
	newFontAssetMapCLI->assetsName = (String^)this->assetsName->Clone();
	newFontAssetMapCLI->assetName = (String^)this->assetName->Clone();
	newFontAssetMapCLI->containerPath = (String^)this->containerPath->Clone();
	newFontAssetMapCLI->useContainerPath = this->useContainerPath;
	newFontAssetMapCLI->options = gcnew List<AssetMapOptionCLI^>();
	for each(AssetMapOptionCLI^ child in this->options) {
		newFontAssetMapCLI->options->Add((AssetMapOptionCLI^)child->Clone());
	}
	return newFontAssetMapCLI;
}

UnityL10nToolCppCLI::AssetMapOptionCLI^ UnityL10nToolCppCLI::AssetMapOptionCLI::Clone() {
	AssetMapOptionCLI^ newFontAssetMapOptionCLI = gcnew AssetMapOptionCLI();
	newFontAssetMapOptionCLI->OptionName = (String^)this->OptionName->Clone();
	newFontAssetMapOptionCLI->OptionDescription = (String^)this->OptionDescription->Clone();
	switch (this->type) {
	case Type::OPTION_TYPE_BOOL:
		newFontAssetMapOptionCLI->type = Type::OPTION_TYPE_BOOL;
		if (this->Value) {
			newFontAssetMapOptionCLI->Value = (bool)this->Value;
		}
		break;
	case Type::OPTION_TYPE_DOUBLE:
		newFontAssetMapOptionCLI->type = Type::OPTION_TYPE_DOUBLE;
		if (this->Value) {
			newFontAssetMapOptionCLI->Value = (double)this->Value;
		}
		break;
	case Type::OPTION_TYPE_INT:
		newFontAssetMapOptionCLI->type = Type::OPTION_TYPE_INT;
		if (this->Value) {
			newFontAssetMapOptionCLI->Value = (int)this->Value;
		}
		break;
	case Type::OPTION_TYPE_STRING:
		newFontAssetMapOptionCLI->type = Type::OPTION_TYPE_STRING;
		if (this->Value) {
			newFontAssetMapOptionCLI->Value = ((String^)this->Value)->Clone();
		}
		break;
	case Type::OPTION_TYPE_NONE:
		newFontAssetMapOptionCLI->type = Type::OPTION_TYPE_NONE;
		break;
	}
	switch (this->typeAsChild) {
	case Type::OPTION_TYPE_BOOL:
		newFontAssetMapOptionCLI->typeAsChild = Type::OPTION_TYPE_BOOL;
		if (this->ValueAsChild) {
			newFontAssetMapOptionCLI->ValueAsChild = (bool)this->ValueAsChild;
		}
		break;
	case Type::OPTION_TYPE_DOUBLE:
		newFontAssetMapOptionCLI->typeAsChild = Type::OPTION_TYPE_DOUBLE;
		if (this->ValueAsChild) {
			newFontAssetMapOptionCLI->ValueAsChild = (double)this->ValueAsChild;
		}
		break;
	case Type::OPTION_TYPE_INT:
		newFontAssetMapOptionCLI->typeAsChild = Type::OPTION_TYPE_INT;
		if (this->ValueAsChild) {
			newFontAssetMapOptionCLI->ValueAsChild = (int)this->ValueAsChild;
		}
		break;
	case Type::OPTION_TYPE_STRING:
		newFontAssetMapOptionCLI->typeAsChild = Type::OPTION_TYPE_STRING;
		if (this->ValueAsChild) {
			newFontAssetMapOptionCLI->ValueAsChild = ((String^)this->ValueAsChild)->Clone();
		}
		break;
	case Type::OPTION_TYPE_NONE:
		newFontAssetMapOptionCLI->typeAsChild = Type::OPTION_TYPE_NONE;
		break;
	}
	newFontAssetMapOptionCLI->nestedOptions = gcnew List<AssetMapOptionCLI^>();
	if (this->nestedOptions) {
		for each(AssetMapOptionCLI^ child in this->nestedOptions) {
			newFontAssetMapOptionCLI->nestedOptions->Add((AssetMapOptionCLI^)child->Clone());
		}
	}
	return newFontAssetMapOptionCLI;
}