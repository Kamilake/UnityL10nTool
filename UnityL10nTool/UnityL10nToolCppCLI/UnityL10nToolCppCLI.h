#pragma once
#include "..\UnityL10nToolCpp\\UnityL10nToolCpp.h"
#include <map>

using namespace System::Collections::Generic;
using namespace System;

namespace UnityL10nToolCppCLI {
	public ref struct FontAssetMapCLI {
		String^ assetsName;
		String^ assetName;
		String^ containerPath;
		List<String^>^ options;
		String^ selectedOption;
		bool useContainerPath;
	};

	public ref class UnityL10nToolCppManaged
	{
	public:
		UnityL10nToolCppManaged(String^ gameFolderPath);
		Dictionary<String^, List<FontAssetMapCLI^>^>^ GetPluginsSupportAssetMap();
	protected:
		UnityL10nToolCpp *unityL10nToolCpp;	
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
	};
}
