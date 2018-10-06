#pragma once
#define GENERALPURPOSEFUNCTIONSSTATIC
#include "..\UnityL10nToolCpp\\UnityL10nToolCpp.h"
#include <map>


using namespace System::Collections::Generic;
using namespace System;

namespace UnityL10nToolCppCLI {
	public ref struct FontAssetMapOptionCLI {
		enum class Type
		{
			OPTION_TYPE_NONE = 0,
			OPTION_TYPE_STRING = 1,
			OPTION_TYPE_INT = 2,
			OPTION_TYPE_DOUBLE = 3,
			OPTION_TYPE_BOOL = 4
		};
		property String^ OptionName;
		property String^ OptionDescription;
		property Object^ Value;
		property Object^ ValueAsChild;
		property Type type;
		property Type typeAsChild;
		property List<FontAssetMapOptionCLI^>^ nestedOptions;
	};

	public ref struct FontAssetMapCLI {
		property String^ assetsName;
		property String^ assetName;
		property String^ containerPath;
		property bool useContainerPath;
		property List<FontAssetMapOptionCLI^>^ options;
	};

	public ref struct FontAssetMapsCLI {
		property List<FontAssetMapCLI^>^ News;
		property List<FontAssetMapCLI^>^ Saveds;
	};

	

	

	public ref class UnityL10nToolCppManaged
	{
	public:
		static bool DetermineUnityGameFolder(String ^ path);
		static bool DetermineProjectGamePath(String^ path, String^ GameName, String^ MakerName);
		static String^ FindUnityGameFolderFromDataFolderName(String ^ dataFolderName, String ^ GameName, String ^ MakerName);
		
		UnityL10nToolCppManaged(String^ gameFolderPath);
		Dictionary<String^, FontAssetMapsCLI^>^ GetPluginsSupportAssetMap();
		bool LoadGlobalgamemanagersFile();
		bool LoadMonoClassDatabase();
		bool LoadUnityL10nToolAPI();
		List<String^>^ LoadFontPlugins();

		~UnityL10nToolCppManaged();

	protected:
		UnityL10nToolCpp *unityL10nToolCpp;	
	};
	FontAssetMapOptionCLI ^ ConvertMapOptionCLIFromUnmanaged(FontAssetMapOption fontAssetMapOption);
	FontAssetMapOption ConvertMapOptionUnmanagedFromCLI(UnityL10nToolCppCLI::FontAssetMapOptionCLI ^ fontAssetMapOptionCLI);
}
