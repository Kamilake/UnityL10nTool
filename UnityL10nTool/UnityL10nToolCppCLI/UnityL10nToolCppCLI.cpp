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
		tempFontAssetMaps->News = gcnew List<FontAssetMapCLI^>();
		for (vector<FontAssetMap>::iterator fontAssetMapIterator = keyValueIterator->second.News.begin();
			fontAssetMapIterator != keyValueIterator->second.News.end(); fontAssetMapIterator++) {
			FontAssetMapCLI^ fontAssetMapCLI = gcnew FontAssetMapCLI();
			fontAssetMapCLI->assetsName = gcnew String(fontAssetMapIterator->assetsName.c_str());
			fontAssetMapCLI->assetName = gcnew String(fontAssetMapIterator->assetName.c_str());
			fontAssetMapCLI->containerPath = gcnew String(fontAssetMapIterator->containerPath.c_str());
			fontAssetMapCLI->useContainerPath = fontAssetMapIterator->useContainerPath;
			fontAssetMapCLI->options = gcnew List<FontAssetMapOptionCLI^>();
			for (vector<FontAssetMapOption>::iterator optionIterator = fontAssetMapIterator->options.begin();
				optionIterator != fontAssetMapIterator->options.end(); optionIterator++) {
				fontAssetMapCLI->options->Add(ConvertMapOptionCLIFromUnmanaged(*optionIterator));
			}
			tempFontAssetMaps->News->Add(fontAssetMapCLI);
		}
		tempFontAssetMaps->Saved = gcnew List<FontAssetMapCLI^>();
		for (vector<FontAssetMap>::iterator fontAssetMapIterator = keyValueIterator->second.News.begin();
			fontAssetMapIterator != keyValueIterator->second.News.end(); fontAssetMapIterator++) {
			FontAssetMapCLI^ fontAssetMapCLI = gcnew FontAssetMapCLI();
			fontAssetMapCLI->assetsName = gcnew String(fontAssetMapIterator->assetsName.c_str());
			fontAssetMapCLI->assetName = gcnew String(fontAssetMapIterator->assetName.c_str());
			fontAssetMapCLI->containerPath = gcnew String(fontAssetMapIterator->containerPath.c_str());
			fontAssetMapCLI->useContainerPath = fontAssetMapIterator->useContainerPath;
			fontAssetMapCLI->options = gcnew List<FontAssetMapOptionCLI^>();
			for (vector<FontAssetMapOption>::iterator optionIterator = fontAssetMapIterator->options.begin();
				optionIterator != fontAssetMapIterator->options.end(); optionIterator++) {
				fontAssetMapCLI->options->Add(ConvertMapOptionCLIFromUnmanaged(*optionIterator));
			}
			tempFontAssetMaps->Saved->Add(fontAssetMapCLI);
		}
		result->Add(gcnew String(keyValueIterator->first.c_str()), tempFontAssetMaps);
	}
	return result;
}

UnityL10nToolCppCLI::UnityL10nToolCppManaged::~UnityL10nToolCppManaged() {
	unityL10nToolCpp->~UnityL10nToolCpp();
	unityL10nToolCpp = NULL;
}

UnityL10nToolCppCLI::FontAssetMapOptionCLI^ UnityL10nToolCppCLI::ConvertMapOptionCLIFromUnmanaged(FontAssetMapOption fontAssetMapOption) {
	FontAssetMapOptionCLI^ fontAssetMapOptionCLI = gcnew FontAssetMapOptionCLI();
	fontAssetMapOptionCLI->OptionName = gcnew String(fontAssetMapOption.OptionName.c_str());
	fontAssetMapOptionCLI->OptionDescription = gcnew String(fontAssetMapOption.OptionDescription.c_str());

	switch (fontAssetMapOption.type) {
	case FontAssetMapOption::OPTION_TYPE_WSTRING:
		if (fontAssetMapOption.Value != NULL) {
			fontAssetMapOptionCLI->Value = gcnew String(((std::wstring*)fontAssetMapOption.Value)->c_str());
		}
		else {
			fontAssetMapOptionCLI->Value = gcnew String(L"");
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_INT:
		if (fontAssetMapOption.Value != NULL) {
			fontAssetMapOptionCLI->Value = *((int*)fontAssetMapOption.Value);
		}
		else {
			fontAssetMapOptionCLI->Value = (int)0;
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_DOUBLE:
		if (fontAssetMapOption.Value != NULL) {
			fontAssetMapOptionCLI->Value = *((double*)fontAssetMapOption.Value);
		}
		else {
			fontAssetMapOptionCLI->Value = (double)0.0;
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_BOOL:
		if (fontAssetMapOption.Value != NULL) {
			fontAssetMapOptionCLI->Value = *((bool*)fontAssetMapOption.Value);
		}
		else {
			fontAssetMapOptionCLI->Value = (bool)false;
		}
		break;
	}
	switch (fontAssetMapOption.typeAsChild) {
	case FontAssetMapOption::OPTION_TYPE_WSTRING:
		if (fontAssetMapOption.ValueAsChild != NULL) {
			fontAssetMapOptionCLI->ValueAsChild = gcnew String(((std::wstring*)fontAssetMapOption.ValueAsChild)->c_str());
		}
		else {
			fontAssetMapOptionCLI->ValueAsChild = gcnew String(L"");
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_INT:
		if (fontAssetMapOption.ValueAsChild != NULL) {
			fontAssetMapOptionCLI->ValueAsChild = *((int*)fontAssetMapOption.ValueAsChild);
		}
		else {
			fontAssetMapOptionCLI->ValueAsChild = (int)0;
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_DOUBLE:
		if (fontAssetMapOption.ValueAsChild != NULL) {
			fontAssetMapOptionCLI->ValueAsChild = *((double*)fontAssetMapOption.ValueAsChild);
		}
		else {
			fontAssetMapOptionCLI->ValueAsChild = (double)0.0;
		}
		break;
	case FontAssetMapOption::OPTION_TYPE_BOOL:
		if (fontAssetMapOption.ValueAsChild != NULL) {
			fontAssetMapOptionCLI->ValueAsChild = *((bool*)fontAssetMapOption.ValueAsChild);
		}
		else {
			fontAssetMapOptionCLI->ValueAsChild = (bool)false;
		}
		break;
	}
	fontAssetMapOptionCLI->nestedOptions = gcnew List<FontAssetMapOptionCLI^>();
	for (std::vector<FontAssetMapOption>::iterator iterator = fontAssetMapOption.nestedOptions.begin();
		iterator != fontAssetMapOption.nestedOptions.end(); iterator++) {
		fontAssetMapOptionCLI->nestedOptions->Add(ConvertMapOptionCLIFromUnmanaged(*iterator));
	}
	return fontAssetMapOptionCLI;
}

FontAssetMapOption UnityL10nToolCppCLI::ConvertMapOptionUnmanagedFromCLI(UnityL10nToolCppCLI::FontAssetMapOptionCLI^ fontAssetMapOptionCLI) {
	FontAssetMapOption fontAssetMapOption;
	pin_ptr<const wchar_t> ptrOptionName = PtrToStringChars(fontAssetMapOptionCLI->OptionName);
	pin_ptr<const wchar_t> ptrOptionDescription = PtrToStringChars(fontAssetMapOptionCLI->OptionDescription);
	fontAssetMapOption.OptionName = ptrOptionName;
	fontAssetMapOption.OptionDescription = ptrOptionDescription;
	pin_ptr<const wchar_t> ptrValue;
	pin_ptr<const wchar_t> ptrValueAsChild;
	switch (fontAssetMapOptionCLI->type) {
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_STRING:
		ptrValue = PtrToStringChars((String^)fontAssetMapOptionCLI->Value);
		fontAssetMapOption.Value = (void*)(new wstring(ptrValue));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_INT:
		fontAssetMapOption.Value = (void*)(new int((int)fontAssetMapOptionCLI->Value));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE:
		fontAssetMapOption.Value = (void*)(new double((double)fontAssetMapOptionCLI->Value));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_BOOL:
		fontAssetMapOption.Value = (void*)(new bool((bool)fontAssetMapOptionCLI->Value));
		break;
	}
	switch (fontAssetMapOptionCLI->typeAsChild) {
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_STRING:
		ptrValueAsChild = PtrToStringChars((String^)fontAssetMapOptionCLI->ValueAsChild);
		fontAssetMapOption.ValueAsChild = (void*)(new wstring(ptrValueAsChild));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_INT:
		fontAssetMapOption.ValueAsChild = (void*)(new int((int)fontAssetMapOptionCLI->ValueAsChild));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE:
		fontAssetMapOption.ValueAsChild = (void*)(new double((double)fontAssetMapOptionCLI->ValueAsChild));
		break;
	case FontAssetMapOptionCLI::Type::OPTION_TYPE_BOOL:
		fontAssetMapOption.ValueAsChild = (void*)(new bool((bool)fontAssetMapOptionCLI->ValueAsChild));
		break;
	}
	for each(FontAssetMapOptionCLI^ iterator in fontAssetMapOptionCLI->nestedOptions) {
		fontAssetMapOption.nestedOptions.push_back(ConvertMapOptionUnmanagedFromCLI(iterator));
	}
	return fontAssetMapOption;
}

