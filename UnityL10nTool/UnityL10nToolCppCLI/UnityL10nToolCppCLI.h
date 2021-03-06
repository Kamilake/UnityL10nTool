#pragma once
#define UnityL10nToolCppCLIDEFINE
#include "..\UnityL10nToolCpp\\UnityL10nToolCpp.h"
#include "../UnityL10nToolAPI/inc/IULTPluginCommonInterface.h"
#include "../UnityL10nToolAPI/inc/IULTTextPluginInterface.h"
#include "../UnityL10nToolAPI/inc/IULTFontPluginInterface.h"
#include <map>

using namespace System::Collections::Generic;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections::ObjectModel;

namespace UnityL10nToolCppCLI {

	String^ ConvertFromWstringToManagedString(wstring wstr) {
		return gcnew String(wstr.c_str());
	}

	wstring ConvertFromManagedStringToWstring(String^ m_str) {
		pin_ptr<const wchar_t> ptr = PtrToStringChars(m_str);
		return wstring(ptr);
	}

	String^ ConvertFromStringToManagedString(string str) {
		return gcnew String(str.c_str());
	}

	string ConvertFromManagedStringToString(String^ m_str) {
		char* ptr = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(m_str).ToPointer();
		return string(ptr);
	}

	public ref class OnlineUpdateCLI : public INotifyPropertyChanged {
	public:
		enum class SelectedEnum {
			None = 0,
			Manual = 1,
			GitHub = 2,
		};
		property SelectedEnum Selected {
			SelectedEnum get() { return this->_Selected; }
			void set(SelectedEnum Selected) {
				this->_Selected = Selected;
				OnPropertyChanged("Selected");
			}
		}
		property String^ currentVersion {
			String^ get() { return this->_currentVersion; }
			void set(String^ str) {
				this->_currentVersion = str;
				OnPropertyChanged("currentVersion");
			}
		}
		property String^ currentVersionURL {
			String^ get() { return this->_currentVersionURL; }
			void set(String^ str) {
				this->_currentVersionURL = str;
				OnPropertyChanged("currentVersionURL");
			}
		}
		property String^ manualZipURL {
			String^ get() { return this->_manualZipURL; }
			void set(String^ str) {
				this->_manualZipURL = str;
				OnPropertyChanged("manualZipURL");
			}
		}
		property String^ gitHubOwner {
			String^ get() { return this->_gitHubOwner; }
			void set(String^ str) {
				this->_gitHubOwner = str;
				OnPropertyChanged("gitHubOwner");
			}
		}
		property String^ gitHubRepo {
			String^ get() { return this->_gitHubRepo; }
			void set(String^ str) {
				this->_gitHubRepo = str;
				OnPropertyChanged("gitHubRepo");
			}
		}
		String^ _gitHubDownloadURL;
		OnlineUpdateCLI() {
			this->_Selected = OnlineUpdateCLI::SelectedEnum::None;
			this->_currentVersion = gcnew String("");
			this->_currentVersionURL = gcnew String("");
			this->_manualZipURL = gcnew String("");
			this->_gitHubOwner = gcnew String("");
			this->_gitHubRepo = gcnew String("");
		}
		OnlineUpdateCLI(OnlineUpdate onlineUpdate) {
			switch (onlineUpdate.Selected) {
			case OnlineUpdate::None:
				this->_Selected = OnlineUpdateCLI::SelectedEnum::None;
				break;
			case OnlineUpdate::Manual:
				this->_Selected = OnlineUpdateCLI::SelectedEnum::Manual;
				break;
			case OnlineUpdate::GitHub:
				this->_Selected = OnlineUpdateCLI::SelectedEnum::GitHub;
				break;
			default:
				this->_Selected = OnlineUpdateCLI::SelectedEnum::None;
			}
			this->currentVersion = ConvertFromWstringToManagedString(onlineUpdate.currentVersion);
			this->currentVersionURL = ConvertFromWstringToManagedString(onlineUpdate.currentVersionURL);
			this->manualZipURL = ConvertFromWstringToManagedString(onlineUpdate.manualZipURL);
			this->gitHubOwner = ConvertFromWstringToManagedString(onlineUpdate.gitHubOwner);
			this->gitHubRepo = ConvertFromWstringToManagedString(onlineUpdate.gitHubRepo);
		}
		OnlineUpdate toUnmanaged() {
			OnlineUpdate onlineUpdate;
			switch (this->_Selected) {
			case OnlineUpdateCLI::SelectedEnum::Manual:
				onlineUpdate.Selected = OnlineUpdate::Manual;
				break;
			case OnlineUpdateCLI::SelectedEnum::GitHub:
				onlineUpdate.Selected = OnlineUpdate::GitHub;
				break;
			default:
				onlineUpdate.Selected = OnlineUpdate::None;
			}
			onlineUpdate.currentVersion = ConvertFromManagedStringToWstring(this->_currentVersion);
			onlineUpdate.currentVersionURL = ConvertFromManagedStringToWstring(this->currentVersionURL);
			onlineUpdate.manualZipURL = ConvertFromManagedStringToWstring(this->_manualZipURL);
			onlineUpdate.gitHubOwner = ConvertFromManagedStringToWstring(this->_gitHubOwner);
			onlineUpdate.gitHubRepo = ConvertFromManagedStringToWstring(this->_gitHubRepo);
			return onlineUpdate;
		}
#pragma region INotifyPropertyChanged Members
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
#pragma endregion
	private:
		SelectedEnum _Selected;
		String^ _currentVersion;
		String^ _currentVersionURL;
		String^ _manualZipURL;
		String^ _gitHubOwner;
		String^ _gitHubRepo;
	};

	public ref class OnlineResourcePairCLI : public ICloneable, INotifyPropertyChanged {
	private:
		String^ _filePath;
		String^ _URL;
	public:
		property String^ filePath{
			String^ get() {return _filePath; }
			void set(String^ value) {
				this->_filePath = value;
				this->OnPropertyChanged("filePath");
			}
		}
		property String^ URL {
			String^ get() { return this->_URL; }
			void set(String^ value) {
				this->_URL = value;
				this->OnPropertyChanged("URL");
			}
		}
		OnlineResourcePairCLI() {
			this->_filePath = gcnew String("");
			this->_URL = gcnew String("");
		}
		OnlineResourcePairCLI(OnlineResourcePair onlineResourcePair) {
			this->_filePath = ConvertFromWstringToManagedString(onlineResourcePair.filePath);
			this->_URL = ConvertFromWstringToManagedString(onlineResourcePair.URL);
		}
		OnlineResourcePair toUnmanaged() {
			OnlineResourcePair onlineResourcePair;
			onlineResourcePair.filePath = ConvertFromManagedStringToWstring(this->_filePath);
			onlineResourcePair.URL = ConvertFromManagedStringToWstring(this->_URL);
			return onlineResourcePair;
		}
		virtual Object^ Clone() override {
			OnlineResourcePairCLI^ newOnlineResourcePairCLI = gcnew OnlineResourcePairCLI();
			newOnlineResourcePairCLI->URL = (String^)this->URL->Clone();
			newOnlineResourcePairCLI->filePath = (String^)this->filePath->Clone();
			return newOnlineResourcePairCLI;
		}
#pragma region INotifyPropertyChanged Members
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
#pragma endregion
	};

	public ref class AssetMapOptionCLI : public ICloneable {
	public:
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
		property List<AssetMapOptionCLI^>^ nestedOptions;
		AssetMapOptionCLI^ Clone();
		virtual Object^ Clone2() sealed = ICloneable::Clone;
		AssetMapOptionCLI() {}
		AssetMapOptionCLI(AssetMapOption assetMapOption) {
			this->OptionName = gcnew String(assetMapOption.OptionName.c_str());
			this->OptionDescription = gcnew String(assetMapOption.OptionDescription.c_str());

			switch (assetMapOption.type) {
			case AssetMapOption::OPTION_TYPE_WSTRING:
				this->type = AssetMapOptionCLI::Type::OPTION_TYPE_STRING;
				if (assetMapOption.Value != NULL) {
					this->Value = gcnew String(((std::wstring*)assetMapOption.Value)->c_str());
				}
				break;
			case AssetMapOption::OPTION_TYPE_INT:
				this->type = AssetMapOptionCLI::Type::OPTION_TYPE_INT;
				if (assetMapOption.Value != NULL) {
					this->Value = *((int*)assetMapOption.Value);
				}
				break;
			case AssetMapOption::OPTION_TYPE_DOUBLE:
				this->type = AssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE;
				if (assetMapOption.Value != NULL) {
					this->Value = *((double*)assetMapOption.Value);
				}
				break;
			case AssetMapOption::OPTION_TYPE_BOOL:
				this->type = AssetMapOptionCLI::Type::OPTION_TYPE_BOOL;
				if (assetMapOption.Value != NULL) {
					this->Value = *((bool*)assetMapOption.Value);
				}
				break;
			}
			switch (assetMapOption.typeAsChild) {
			case AssetMapOption::OPTION_TYPE_WSTRING:
				this->typeAsChild = AssetMapOptionCLI::Type::OPTION_TYPE_STRING;
				if (assetMapOption.ValueAsChild != NULL) {
					this->ValueAsChild = gcnew String(((std::wstring*)assetMapOption.ValueAsChild)->c_str());
				}
				break;
			case AssetMapOption::OPTION_TYPE_INT:
				this->typeAsChild = AssetMapOptionCLI::Type::OPTION_TYPE_INT;
				if (assetMapOption.ValueAsChild != NULL) {
					this->ValueAsChild = *((int*)assetMapOption.ValueAsChild);
				}
				break;
			case AssetMapOption::OPTION_TYPE_DOUBLE:
				this->typeAsChild = AssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE;
				if (assetMapOption.ValueAsChild != NULL) {
					this->ValueAsChild = *((double*)assetMapOption.ValueAsChild);
				}
				break;
			case AssetMapOption::OPTION_TYPE_BOOL:
				this->typeAsChild = AssetMapOptionCLI::Type::OPTION_TYPE_BOOL;
				if (assetMapOption.ValueAsChild != NULL) {
					this->ValueAsChild = *((bool*)assetMapOption.ValueAsChild);
				}
				break;
			}
			if (assetMapOption.nestedOptions.size() != 0) {
				this->nestedOptions = gcnew List<AssetMapOptionCLI^>();
				for (std::vector<AssetMapOption>::iterator iterator = assetMapOption.nestedOptions.begin();
					iterator != assetMapOption.nestedOptions.end(); iterator++) {
					this->nestedOptions->Add(gcnew AssetMapOptionCLI(*iterator));
				}
			}
		}
		AssetMapOption ToUnmanaged() {
			pin_ptr<const wchar_t> ptrOptionName = PtrToStringChars(this->OptionName);
			pin_ptr<const wchar_t> ptrOptionDescription = PtrToStringChars(this->OptionDescription);
			pin_ptr<const wchar_t> ptrValue = nullptr;
			pin_ptr<const wchar_t> ptrValueAsChild = nullptr;
			AssetMapOption fontAssetMapOption = AssetMapOption(
				ptrOptionName,
				ptrOptionDescription,
				NULL,
				NULL,
				AssetMapOption::OPTION_TYPE_NONE,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			switch (this->type) {
			case AssetMapOptionCLI::Type::OPTION_TYPE_STRING:
				fontAssetMapOption.type = AssetMapOption::OPTION_TYPE_WSTRING;
				if (this->Value != nullptr) {
					ptrValue = PtrToStringChars((String^)this->Value);
					fontAssetMapOption.Value = (void*)(new wstring(ptrValue));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_INT:
				fontAssetMapOption.type = AssetMapOption::OPTION_TYPE_INT;
				if (this->Value != nullptr) {
					fontAssetMapOption.Value = (void*)(new int((int)this->Value));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE:
				fontAssetMapOption.type = AssetMapOption::OPTION_TYPE_DOUBLE;
				if (this->Value != nullptr) {
					fontAssetMapOption.Value = (void*)(new double((double)this->Value));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_BOOL:
				fontAssetMapOption.type = AssetMapOption::OPTION_TYPE_BOOL;
				if (this->Value != nullptr) {
					fontAssetMapOption.Value = (void*)(new bool((bool)this->Value));
				}
				break;
			}
			switch (this->typeAsChild) {
			case AssetMapOptionCLI::Type::OPTION_TYPE_STRING:
				fontAssetMapOption.typeAsChild = AssetMapOption::OPTION_TYPE_WSTRING;
				if (this->ValueAsChild != nullptr) {
					ptrValueAsChild = PtrToStringChars((String^)this->ValueAsChild);
					fontAssetMapOption.ValueAsChild = (void*)(new wstring(ptrValueAsChild));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_INT:
				fontAssetMapOption.typeAsChild = AssetMapOption::OPTION_TYPE_INT;
				if (this->ValueAsChild != nullptr) {
					fontAssetMapOption.ValueAsChild = (void*)(new int((int)this->ValueAsChild));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_DOUBLE:
				fontAssetMapOption.typeAsChild = AssetMapOption::OPTION_TYPE_DOUBLE;
				if (this->ValueAsChild != nullptr) {
					fontAssetMapOption.ValueAsChild = (void*)(new double((double)this->ValueAsChild));
				}
				break;
			case AssetMapOptionCLI::Type::OPTION_TYPE_BOOL:
				fontAssetMapOption.typeAsChild = AssetMapOption::OPTION_TYPE_BOOL;
				if (this->ValueAsChild != nullptr) {
					fontAssetMapOption.ValueAsChild = (void*)(new bool((bool)this->ValueAsChild));
				}
				break;
			}
			if (this->nestedOptions != nullptr) {
				for each(AssetMapOptionCLI^ iterator in this->nestedOptions) {
					fontAssetMapOption.nestedOptions.push_back(iterator->ToUnmanaged());
				}
			}
			return fontAssetMapOption;
		}
	};

	Object^ AssetMapOptionCLI::Clone2() {
		return this->Clone();
	}

	public ref class FontAssetMapCLI : public ICloneable {
	public:
		property String^ Id;
		property String^ assetsName;
		property String^ assetName;
		property String^ containerPath;
		property bool useContainerPath;
		property List<AssetMapOptionCLI^>^ options;
		FontAssetMapCLI^ Clone();
		virtual Object^ Clone2() sealed = ICloneable::Clone;
		FontAssetMapCLI(){}
		FontAssetMapCLI(FontAssetMap fontAssetMap) {
			this->Id = ConvertFromWstringToManagedString(fontAssetMap.Id);
			this->assetsName = gcnew String(fontAssetMap.assetsName.c_str());
			this->assetName = gcnew String(fontAssetMap.assetName.c_str());
			this->containerPath = gcnew String(fontAssetMap.containerPath.c_str());
			this->useContainerPath = fontAssetMap.useContainerPath;
			this->options = gcnew List<AssetMapOptionCLI^>();
			for (vector<AssetMapOption>::iterator optionIterator = fontAssetMap.options.begin();
				optionIterator != fontAssetMap.options.end(); optionIterator++) {
				this->options->Add(gcnew AssetMapOptionCLI(*optionIterator));
			}
		}
		bool LooseEquals(FontAssetMapCLI^ fontAssetMapCLI) {
			if (this->Id == fontAssetMapCLI->Id &&
				this->assetName == fontAssetMapCLI->assetName &&
				this->assetsName == fontAssetMapCLI->assetsName &&
				this->containerPath == fontAssetMapCLI->containerPath) {
				return true;
			}
			else {
				return false;
			}
		}
		FontAssetMap toUnmanaged() {
			FontAssetMap fontAssetMap;
			fontAssetMap.Id = ConvertFromManagedStringToWstring(this->Id);
			fontAssetMap.assetsName = ConvertFromManagedStringToString(this->assetsName);
			fontAssetMap.assetName = ConvertFromManagedStringToString(this->assetName);
			fontAssetMap.containerPath = ConvertFromManagedStringToString(this->containerPath);
			fontAssetMap.useContainerPath = this->useContainerPath;
			for each(AssetMapOptionCLI^ fontAssetMapOptionCLI in this->options) {
				fontAssetMap.options.push_back(fontAssetMapOptionCLI->ToUnmanaged());
			}
			return fontAssetMap;
		}
	};
	Object^ FontAssetMapCLI::Clone2() {
		return this->Clone();
	}

	public ref class FontAssetMapsCLI {
	public:
		property ObservableCollection<FontAssetMapCLI^>^ News ;
		property ObservableCollection<FontAssetMapCLI^>^ Saveds;
		FontAssetMapsCLI(FontAssetMaps fontAssetMaps) {
			this->News = gcnew ObservableCollection<FontAssetMapCLI^>();
			for (vector<FontAssetMap>::iterator fontAssetMapIterator = fontAssetMaps.News.begin();
				fontAssetMapIterator != fontAssetMaps.News.end(); fontAssetMapIterator++) {
				this->News->Add(gcnew FontAssetMapCLI(*fontAssetMapIterator));
			}
			this->Saveds = gcnew ObservableCollection<FontAssetMapCLI^>();
			for (vector<FontAssetMap>::iterator fontAssetMapIterator = fontAssetMaps.Saveds.begin();
				fontAssetMapIterator != fontAssetMaps.Saveds.end(); fontAssetMapIterator++) {
				this->Saveds->Add(gcnew FontAssetMapCLI(*fontAssetMapIterator));
			}
		}
		FontAssetMaps toUnmanaged() {
			FontAssetMaps fontAssetMaps;
			for each(FontAssetMapCLI^ fontAssetMapCLI in this->News) {
				fontAssetMaps.News.push_back(fontAssetMapCLI->toUnmanaged());
			}
			for each(FontAssetMapCLI^ fontAssetMapCLI in this->Saveds) {
				fontAssetMaps.Saveds.push_back(fontAssetMapCLI->toUnmanaged());
			}
			return fontAssetMaps;
		}
	};

	public ref class LanguagePairCLI : public ICloneable {
	public:
		property String^ Original;
		property String^ Translated;
		virtual Object^ Clone() override {
			LanguagePairCLI^ languagePair = gcnew LanguagePairCLI();
			languagePair->Original = (String^)this->Original->Clone();
			languagePair->Translated = (String^)this->Translated->Clone();
			return languagePair;
		}
		LanguagePairCLI(){}
		LanguagePairCLI(LanguagePair languagePair) {
			Original = ConvertFromWstringToManagedString(languagePair.Original);
			Translated = ConvertFromWstringToManagedString(languagePair.Translated);
		}
		LanguagePair ToUnmanaged() {
			LanguagePair languagePair;
			languagePair.Original = ConvertFromManagedStringToWstring(this->Original);
			languagePair.Translated = ConvertFromManagedStringToWstring(this->Translated);
			return languagePair;
		}
	};

	public ref class LanguagePairDicCLI {
	public:
		property Dictionary<String^, LanguagePairCLI^>^ Dic;
		property String^ TranslatedFileText;
		property String^ TranslatedFileName;
		property List<AssetMapOptionCLI^>^ InteractWithAssetOptions;
		property List<AssetMapOptionCLI^>^ InteractWithFileTextOptions;
		LanguagePairDicCLI() {
			Dic = gcnew Dictionary<String^, LanguagePairCLI^>();
			InteractWithAssetOptions = gcnew List<AssetMapOptionCLI^>();
			InteractWithFileTextOptions = gcnew List<AssetMapOptionCLI^>();
			TranslatedFileText = gcnew String("");
			TranslatedFileName = gcnew String("");
		}
		LanguagePairDicCLI(LanguagePairDic languagePairDic) {
			Dic = gcnew Dictionary<String^, LanguagePairCLI^>();
			for (map<wstring, LanguagePair>::iterator iterator = languagePairDic.Dic.begin();
				iterator != languagePairDic.Dic.end(); iterator++) {
				Dic->Add(ConvertFromWstringToManagedString(iterator->first),
					gcnew LanguagePairCLI(iterator->second));
			}
			this->TranslatedFileText = gcnew String(ConvertFromWstringToManagedString(languagePairDic.TranslatedFileText));
			this->TranslatedFileName = gcnew String(ConvertFromWstringToManagedString(languagePairDic.TranslatedFileName));
			InteractWithAssetOptions = gcnew List<AssetMapOptionCLI^>();
			InteractWithFileTextOptions = gcnew List<AssetMapOptionCLI^>();
			for (vector<AssetMapOption>::iterator iterator = languagePairDic.InteractWithAssetOptions.begin();
				iterator != languagePairDic.InteractWithAssetOptions.end(); iterator++) {
				InteractWithAssetOptions->Add(gcnew AssetMapOptionCLI(*iterator));
			}
			for (vector<AssetMapOption>::iterator iterator = languagePairDic.InteractWithFileTextOptions.begin();
				iterator != languagePairDic.InteractWithFileTextOptions.end(); iterator++) {
				InteractWithFileTextOptions->Add(gcnew AssetMapOptionCLI(*iterator));
			}
		}
		LanguagePairDic ToUnmanaged() {
			LanguagePairDic languagepairDic;
			for each(KeyValuePair<String^, LanguagePairCLI^> keyValuePair in this->Dic) {
				languagepairDic.Dic[ConvertFromManagedStringToWstring(keyValuePair.Key)]
					= keyValuePair.Value->ToUnmanaged();
			}
			languagepairDic.TranslatedFileText = ConvertFromManagedStringToWstring(this->TranslatedFileText);
			languagepairDic.TranslatedFileName = ConvertFromManagedStringToWstring(this->TranslatedFileName);
			if (this->InteractWithAssetOptions) {
				for each(AssetMapOptionCLI^ assetMapOptionCLI in this->InteractWithAssetOptions) {
					languagepairDic.InteractWithAssetOptions.push_back(assetMapOptionCLI->ToUnmanaged());
				}
			}
			if (this->InteractWithFileTextOptions) {
				for each(AssetMapOptionCLI^ assetMapOptionCLI in this->InteractWithFileTextOptions) {
					languagepairDic.InteractWithFileTextOptions.push_back(assetMapOptionCLI->ToUnmanaged());
				}
			}
			return languagepairDic;
		}
	};
	typedef Dictionary<String^, LanguagePairDicCLI^> LanguagePairDicCLIs;

	public ref class TextAssetMapCLI : public System::ComponentModel::INotifyPropertyChanged {
	private:
		String^ _assetsName;
		String^ _assetName;
		String^ _containerPath;
		String^ _ExternalRelativeFilePath;
		String^ _InteractWithAssetPluginName;
		String^ _InteractWithFileTextPluginName;
		String^ _InteractWithMonoAssetPluginName;
		bool _useContainerPath;
		String^ _OriginalText;
		String^ _TranslatedText;
	public:
		enum class ToWhere {
			None = 0,
			ToInteractWithAsset = 1,
			ToInteractWithFileText = 2,
			ToInteractWithMonoAsset = 3,
			ToDone = 4,
		};
		property String^ assetsName {
			String^ get() {return this->_assetsName;}
			void set(String^ value) {
				this->_assetsName = value;
				OnPropertyChanged("assetsName");
			}
		}
		property String^ assetName {
			String^ get() { return this->_assetName; }
			void set(String^ value) {
				this->_assetName = value;
				OnPropertyChanged("assetName");
			}
		}
		property String^ containerPath {
			String^ get() { return this->_containerPath; }
			void set(String^ value) {
				this->_containerPath = value;
				OnPropertyChanged("containerPath");
			}
		}
		property String^ ExternalRelativeFilePath {
			String^ get() { return this->_ExternalRelativeFilePath; }
			void set(String^ value) {
				this->_ExternalRelativeFilePath = value;
				OnPropertyChanged("ExternalRelativeFilePath");
			}
		}
		property String^ InteractWithAssetPluginName {
			String^ get() { return this->_InteractWithAssetPluginName; }
			void set(String^ value) {
				this->_InteractWithAssetPluginName = value;
				OnPropertyChanged("InteractWithAssetPluginName");
			}
		}
		property String^ InteractWithFileTextPluginName {
			String^ get() { return this->_InteractWithFileTextPluginName; }
			void set(String^ value) {
				this->_InteractWithFileTextPluginName = value;
				OnPropertyChanged("InteractWithFileTextPluginName");
			}
		}
		property String^ InteractWithMonoAssetPluginName {
			String^ get() { return this->_InteractWithMonoAssetPluginName; }
			void set(String^ value) {
				this->_InteractWithMonoAssetPluginName = value;
				OnPropertyChanged("InteractWithMonoAssetPluginName");
			}
		}
		property String^ OriginalText {
			String^ get() { return this->_OriginalText; }
			void set(String^ value) {
				this->_OriginalText = value;
				OnPropertyChanged("OriginalText");
			}
		}
		property String^ TranslatedText {
			String^ get() { return this->_TranslatedText; }
			void set(String^ value) {
				this->_TranslatedText = value;
				OnPropertyChanged("TranslatedText");
			}
		}
		property bool useContainerPath {
			bool get() { return this->_useContainerPath; }
			void set(bool value) {
				this->_useContainerPath = value;
				OnPropertyChanged("useContainerPath");
			}
		}
		property LanguagePairDicCLIs^ languagePairDics;
#pragma region INotifyPropertyChanged Members
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
#pragma endregion
		TextAssetMapCLI(TextAssetMap textAssetMap) {
			assetsName = ConvertFromWstringToManagedString(textAssetMap.assetsName);
			assetName = ConvertFromWstringToManagedString(textAssetMap.assetName);
			containerPath = ConvertFromWstringToManagedString(textAssetMap.containerPath);
			ExternalRelativeFilePath = ConvertFromWstringToManagedString(textAssetMap.ExternalRelativeFilePath);
			useContainerPath = textAssetMap.useContainerPath;
			InteractWithAssetPluginName = ConvertFromWstringToManagedString(textAssetMap.InteractWithAssetPluginName);
			InteractWithFileTextPluginName = ConvertFromWstringToManagedString(textAssetMap.InteractWithFileTextPluginName);
			InteractWithMonoAssetPluginName = ConvertFromWstringToManagedString(textAssetMap.InteractWithMonoAssetPluginName);
			OriginalText = ConvertFromWstringToManagedString(textAssetMap.OriginalText);
			TranslatedText = ConvertFromWstringToManagedString(textAssetMap.TranslatedText);
			languagePairDics = gcnew LanguagePairDicCLIs();
			for (map<wstring, LanguagePairDic>::iterator iterator = textAssetMap.languagePairDics.begin();
				iterator != textAssetMap.languagePairDics.end(); iterator++) {
				languagePairDics[ConvertFromWstringToManagedString(iterator->first)]
					= gcnew LanguagePairDicCLI(iterator->second);
			}
		}
		TextAssetMap ToUnmanaged() {
			TextAssetMap textAssetMap;
			textAssetMap.assetsName = ConvertFromManagedStringToWstring(this->assetsName);
			textAssetMap.assetName = ConvertFromManagedStringToWstring(this->assetName);
			textAssetMap.containerPath = ConvertFromManagedStringToWstring(this->containerPath);
			textAssetMap.ExternalRelativeFilePath = ConvertFromManagedStringToWstring(this->_ExternalRelativeFilePath);
			textAssetMap.useContainerPath = (this->useContainerPath);
			textAssetMap.InteractWithAssetPluginName = ConvertFromManagedStringToWstring(this->InteractWithAssetPluginName);
			textAssetMap.InteractWithFileTextPluginName = ConvertFromManagedStringToWstring(this->InteractWithFileTextPluginName);
			textAssetMap.InteractWithMonoAssetPluginName = ConvertFromManagedStringToWstring(this->InteractWithMonoAssetPluginName);
			textAssetMap.OriginalText = ConvertFromManagedStringToWstring(this->OriginalText);
			textAssetMap.TranslatedText = ConvertFromManagedStringToWstring(this->TranslatedText);
			for each(KeyValuePair<String^, LanguagePairDicCLI^> keyValuePair in this->languagePairDics) {
				textAssetMap.languagePairDics.insert(pair<wstring, LanguagePairDic>(
					ConvertFromManagedStringToWstring(keyValuePair.Key),
					keyValuePair.Value->ToUnmanaged()));
			}			
			return textAssetMap;
		}
	};

	public ref class TextAssetMapsCLI {
	public:
		property ObservableCollection<TextAssetMapCLI^>^ InteractWithAssetNews;
		property ObservableCollection<TextAssetMapCLI^>^ InteractWithFileTextNews;
		property ObservableCollection<TextAssetMapCLI^>^ Done;
		TextAssetMapsCLI() {
			InteractWithAssetNews = gcnew ObservableCollection<TextAssetMapCLI^>();
			InteractWithFileTextNews = gcnew ObservableCollection<TextAssetMapCLI^>();
			Done = gcnew ObservableCollection<TextAssetMapCLI^>();
		}
		TextAssetMapsCLI(TextAssetMaps textAssetMaps) {
			InteractWithAssetNews = gcnew ObservableCollection<TextAssetMapCLI^>();
			InteractWithFileTextNews = gcnew ObservableCollection<TextAssetMapCLI^>();
			Done = gcnew ObservableCollection<TextAssetMapCLI^>();
			for (vector<TextAssetMap>::iterator iterator = textAssetMaps.InteractWithAssetNews.begin();
				iterator != textAssetMaps.InteractWithAssetNews.end(); iterator++) {
				InteractWithAssetNews->Add(gcnew TextAssetMapCLI(*iterator));
			}
			for (vector<TextAssetMap>::iterator iterator = textAssetMaps.InteractWithFileTextNews.begin();
				iterator != textAssetMaps.InteractWithFileTextNews.end(); iterator++) {
				InteractWithFileTextNews->Add(gcnew TextAssetMapCLI(*iterator));
			}
			for (vector<TextAssetMap>::iterator iterator = textAssetMaps.Done.begin();
				iterator != textAssetMaps.Done.end(); iterator++) {
				Done->Add(gcnew TextAssetMapCLI(*iterator));
			}
		}
	};

	public ref class CopyResourcesManualPairCLI : public ICloneable, INotifyPropertyChanged {
	private:
		String^ _ResourcesFileFromProjectFolder;
		String^ _ResourcesFileTargetRelativePath;
	public:
		property String^ ResourcesFileFromProjectFolder {
			String^ get() { return this->_ResourcesFileFromProjectFolder; }
			void set(String^ value) {
				this->_ResourcesFileFromProjectFolder = value;
				OnPropertyChanged("ResourcesFileFromProjectFolder");
			}
		}
		property String^ ResourcesFileTargetRelativePath {
			String^ get() { return this->_ResourcesFileTargetRelativePath; }
			void set(String^ value) {
				this->_ResourcesFileTargetRelativePath = value;
				OnPropertyChanged("ResourcesFileTargetRelativePath");
			}
		}
		CopyResourcesManualPairCLI(){}
		CopyResourcesManualPairCLI(CopyResourcesManualPair copyResourcesManualPair) {
			this->_ResourcesFileFromProjectFolder = ConvertFromWstringToManagedString(copyResourcesManualPair.ResourcesFileFromProjectFolder);
			this->_ResourcesFileTargetRelativePath = ConvertFromWstringToManagedString(copyResourcesManualPair.ResourcesFileTargetRelativePath);
		}
		CopyResourcesManualPair toUnmanaged() {
			CopyResourcesManualPair copyResourcesManualPair;
			copyResourcesManualPair.ResourcesFileFromProjectFolder = ConvertFromManagedStringToWstring(this->_ResourcesFileFromProjectFolder);
			copyResourcesManualPair.ResourcesFileTargetRelativePath = ConvertFromManagedStringToWstring(this->_ResourcesFileTargetRelativePath);
			return copyResourcesManualPair;
		}
		virtual Object^ Clone() override {
			CopyResourcesManualPairCLI^ clone = gcnew CopyResourcesManualPairCLI();
			clone->_ResourcesFileFromProjectFolder = (String^)this->_ResourcesFileFromProjectFolder->Clone();
			clone->_ResourcesFileTargetRelativePath = (String^)this->_ResourcesFileTargetRelativePath->Clone();
			return clone;
		}
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
	};

	public ref class ProjectSettingsCLI : public INotifyPropertyChanged {
	private:
		bool _DownloadOnlineResourcesWhenBuild;
		bool _ZipBuildFolderAfterBuild;
		bool _CreateCurrentVersionTxtFileAfterBuild;
		bool _RemoveBuildFolderBeforeBuild;
		String^ _ZipFileName;
		bool _IsZipFileNameConatinVersion;
	public:
		property bool DownloadOnlineResourcesWhenBuild {
			bool get() { return this->_DownloadOnlineResourcesWhenBuild; }
			void set(bool value) {
				this->_DownloadOnlineResourcesWhenBuild = value;
				OnPropertyChanged("DownloadOnlineResourcesWhenBuild");
			}
		}
		property bool ZipBuildFolderAfterBuild {
			bool get() { return this->_ZipBuildFolderAfterBuild; }
			void set(bool value) {
				this->_ZipBuildFolderAfterBuild = value;
				OnPropertyChanged("ZipBuildFolderAfterBuild");
			}
		}
		property bool CreateCurrentVersionTxtFileAfterBuild {
			bool get() { return this->_CreateCurrentVersionTxtFileAfterBuild; }
			void set(bool value) {
				this->_CreateCurrentVersionTxtFileAfterBuild = value;
				OnPropertyChanged("CreateCurrentVersionTxtFileAfterBuild");
			}
		}
		property bool RemoveBuildFolderBeforeBuild {
			bool get() { return this->_RemoveBuildFolderBeforeBuild; }
			void set(bool value) {
				this->_RemoveBuildFolderBeforeBuild = value;
				OnPropertyChanged("RemoveBuildFolderBeforeBuild");
			}
		}
		property String^ ZipFileName {
			String^ get() { return this->_ZipFileName; }
			void set(String^ value) {
				this->_ZipFileName = value;
				OnPropertyChanged("ZipFileName");
			}
		}
		property bool IsZipFileNameConatinVersion {
			bool get() { return this->_IsZipFileNameConatinVersion; }
			void set(bool value) {
				this->_IsZipFileNameConatinVersion = value;
				OnPropertyChanged("IsZipFileNameConatinVersion");
			}
		}
		ProjectSettingsCLI() {
			this->_DownloadOnlineResourcesWhenBuild = false;
			this->_ZipBuildFolderAfterBuild = false;
			this->_CreateCurrentVersionTxtFileAfterBuild = false;
			this->_RemoveBuildFolderBeforeBuild = false;
			this->_ZipFileName = gcnew String("");
			this->IsZipFileNameConatinVersion = false;
		}
		ProjectSettingsCLI(ProjectSettings projectSettings) {
			this->_DownloadOnlineResourcesWhenBuild = projectSettings.DownloadOnlineResourcesWhenBuild;
			this->_ZipBuildFolderAfterBuild = projectSettings.ZipBuildFolderAfterBuild;
			this->_CreateCurrentVersionTxtFileAfterBuild = projectSettings.CreateCurrentVersionTxtFileAfterBuild;
			this->_RemoveBuildFolderBeforeBuild = projectSettings.RemoveBuildFolderBeforeBuild;
			this->_ZipFileName = ConvertFromWstringToManagedString(projectSettings.ZipFileName);
			this->_IsZipFileNameConatinVersion = projectSettings.IsZipFileNameConatinVersion;
		}
		ProjectSettings toUnmanaged() {
			ProjectSettings projectSettings;
			projectSettings.DownloadOnlineResourcesWhenBuild = this->_DownloadOnlineResourcesWhenBuild;
			projectSettings.ZipBuildFolderAfterBuild = this->_ZipBuildFolderAfterBuild;
			projectSettings.CreateCurrentVersionTxtFileAfterBuild = this->_CreateCurrentVersionTxtFileAfterBuild;
			projectSettings.RemoveBuildFolderBeforeBuild = this->_RemoveBuildFolderBeforeBuild;
			projectSettings.ZipFileName = ConvertFromManagedStringToWstring(this->_ZipFileName);
			projectSettings.IsZipFileNameConatinVersion = this->_IsZipFileNameConatinVersion;
			return projectSettings;
		}
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
	};

	public ref class UnityL10nToolCppManaged
	{
	public:
		static String^ DetermineUnityGameFolder(String ^ path);
		static bool DetermineProjectGamePath(String^ path, String^ GameName, String^ MakerName);
		static String^ FindUnityGameFolderFromDataFolderName(String ^ dataFolderName, String ^ GameName, String ^ MakerName);
		static String^ NewGameProjectFromFolder(String^ folder);
		
		UnityL10nToolCppManaged(String^ projectJsonFolderPath, String^ gameFolderPath);
		UnityL10nToolCppManaged(String^ projectJsonFolderPath);
		Dictionary<String^, FontAssetMapsCLI^>^ GetPluginsSupportAssetMap();
		bool LoadGlobalgamemanagersFile();
		bool ProcessResourceAndMonoManger();
		bool LoadMonoClassDatabase();
		bool LoadUnityL10nToolAPI();
		List<String^>^ LoadFontPlugins();

		bool SetPluginsSupportAssetMap(Dictionary<String^, FontAssetMapsCLI^>^ pluginsSupportAssetMapsCLI);

		bool GetProjectConfigJsonFromFontPlugin();
		bool SetTextPluginConfigToJsonValue();
		void LoadProjectSettingsFromJson();
		ProjectSettingsCLI^ GetProjectSettings();
		void SetProjectSettings(ProjectSettingsCLI^ projectSettingsCLI);
		bool SaveProjectConfigJson();
		bool BuildProject(String^ buildTargetFolder);

		bool SetPacherConfigJson();
		bool LoadAssetsReplacer();
		bool MakeModifiedAssetsFile();
		bool CopyResourceFileToGameFolder();

#pragma region TextAssetPluginProject
		TextAssetMapsCLI^ GetTextAssetMaps();
		TextAssetMapCLI^ GetTextAssetMapFromExternalFile(String^ ExternalRelativeFilePath);
		bool SetTextAssetMaps(TextAssetMapCLI^ textAssetMapCLI, TextAssetMapCLI::ToWhere toWhereCLI);
		bool LoadTextPlugins();
		List<String^>^ GetInteractWithAssetPluginNames();
		List<String^>^ GetInteractWithFileTextPluginNames();
		//String^ GetOriginalText(TextAssetMapCLI^ textAssetMap);
		TextAssetMapCLI^ GetOriginalLanguagePairDics(TextAssetMapCLI^ textAssetMapCLI, bool IsFinal);
		TextAssetMapCLI^ GetTranslatedText(TextAssetMapCLI^ textAssetMapCLI, bool IsFinal);
		TextAssetMapCLI^ GetUpdateFileText(TextAssetMapCLI^ textAssetMapCLI, bool IsFinal);
		bool SaveUpdateFileToTempFolder(TextAssetMapCLI^ textAssetMapCLI);
		TextAssetMapCLI^ LoadTranslatedFileTextFromTempAndResourceFolder(TextAssetMapCLI^ textAssetMapCLI);
		TextAssetMapCLI^ GetTranslatedLanguagePairDics(TextAssetMapCLI^ textAssetMapCLI, bool IsFinal);

		TextAssetMapCLI^ GetOriginalLanguagePairDics(TextAssetMapCLI^ textAssetMapCLI);
		TextAssetMapCLI^ GetTranslatedText(TextAssetMapCLI^ textAssetMapCLI);
		TextAssetMapCLI^ GetUpdateFileText(TextAssetMapCLI^ textAssetMapCLI);
		TextAssetMapCLI^ GetTranslatedLanguagePairDics(TextAssetMapCLI^ textAssetMapCLI);

		bool GetTranslatedTextAssetsFromFile();
		bool GetAssetReplacerFromTextAssets();

		ObservableCollection<OnlineResourcePairCLI^>^ GetOnlineResourcePairs();
		void AddOnlineResoucesPair(OnlineResourcePairCLI^ onlineResourcePair);
		void SetOnlineResourcePairs(ObservableCollection<OnlineResourcePairCLI^>^ onlineResourcePairs);
		void DownloadResourcesFromInternetToTempFolder();
		void DownloadResourcesFromInternetToBuildResourceFolder();
#pragma endregion

#pragma region OnlineUpdate
		OnlineUpdateCLI^ GetOnlineUpdate();
		void SetOnlineUpdate(OnlineUpdateCLI^ onlineUpdate);
#pragma endregion

#pragma region CopyResourcesManual member
		ObservableCollection<CopyResourcesManualPairCLI^>^ GetCopyResourcesManualPairs();
		void AddCopyResourcesManualPair(CopyResourcesManualPairCLI^ copyResourcesManualPair);
		void SetCopyResourcesManualPairs(ObservableCollection<CopyResourcesManualPairCLI^>^ copyResourcesManualPairs);
		void CopyResourcesManualPairsForPatcher();
#pragma endregion
		~UnityL10nToolCppManaged();

	protected:
		UnityL10nToolCpp *unityL10nToolCpp;	
	};
}
