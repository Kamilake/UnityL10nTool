#include "stdafx.h"
#include <vcclr.h>

#include "UnityL10nToolCppCLI.h"
#include "../UnityL10nToolAPI/inc/IULTFontPluginInterface.h"

UnityL10nToolCppCLI::UnityL10nToolCppManaged::UnityL10nToolCppManaged(String ^ gameFolderPath)
{
	pin_ptr<const wchar_t> ptr = PtrToStringChars(gameFolderPath);
	unityL10nToolCpp = new UnityL10nToolCpp(ptr);
}

Dictionary<String^, List<UnityL10nToolCppCLI::FontAssetMapCLI^>^>^ UnityL10nToolCppCLI::UnityL10nToolCppManaged::GetPluginsSupportAssetMap()
{
	map<wstring, vector<FontAssetMap>> u_pluginsSupportAssetMap = unityL10nToolCpp->GetPluginsSupportAssetMap();
	Dictionary<String^, List<FontAssetMapCLI^>^>^ result = gcnew Dictionary<String^, List<FontAssetMapCLI^>^>(u_pluginsSupportAssetMap.size());
	for(map<wstring, vector<FontAssetMap>>::iterator keyValueIterator = u_pluginsSupportAssetMap.begin();
		keyValueIterator != u_pluginsSupportAssetMap.end(); keyValueIterator++) {
		List<FontAssetMapCLI^>^ tempFontAssetMapCLIList = gcnew List<FontAssetMapCLI^>(keyValueIterator->second.size());
		for(vector<FontAssetMap>::iterator fontAssetMapIterator = keyValueIterator->second.begin();
			fontAssetMapIterator != keyValueIterator->second.end(); fontAssetMapIterator++) {
			FontAssetMapCLI^ fontAssetMapCLI = gcnew FontAssetMapCLI();
			fontAssetMapCLI->assetsName = gcnew String(fontAssetMapIterator->assetsName.c_str());
			fontAssetMapCLI->assetName = gcnew String(fontAssetMapIterator->assetName.c_str());
			fontAssetMapCLI->containerPath = gcnew String(fontAssetMapIterator->containerPath.c_str());
			fontAssetMapCLI->selectedOption = gcnew String(fontAssetMapIterator->selectedOption.c_str());
			fontAssetMapCLI->options = gcnew List<String^>(fontAssetMapIterator->options.size());
			for (vector<wstring>::iterator optionIterator = fontAssetMapIterator->options.begin();
				optionIterator != fontAssetMapIterator->options.end(); optionIterator++) {
				fontAssetMapCLI->options->Add(gcnew String(optionIterator->c_str()));
			}
			fontAssetMapCLI->useContainerPath = fontAssetMapIterator->useContainerPath;
			tempFontAssetMapCLIList->Add(fontAssetMapCLI);
		}
		result->Add(gcnew String(keyValueIterator->first.c_str()), tempFontAssetMapCLIList);
	}
	return result;
}


