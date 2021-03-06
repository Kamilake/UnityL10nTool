// GetTextPoULTTextPlugin.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <regex>

#include "IULTPluginCommonInterface.h"
#include "IULTTextPluginInterface.h"
#include "GeneralPurposeFunctions.h"
#include "json/json.h"

using namespace std;

TextPluginInfo* TextPluginInfoGlobal;

const wchar_t* whitespace = L" \t\n\r\f\v";

// trim from end of string (right)
inline std::wstring& rtrim(std::wstring& s, const wchar_t* t = whitespace)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from beginning of string (left)
inline std::wstring& ltrim(std::wstring& s, const wchar_t* t = whitespace)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from both ends of string (left & right)
inline std::wstring& trim(std::wstring& s, const wchar_t* t = whitespace)
{
	return ltrim(rtrim(s, t), t);
}

//LanguagePairDics _cdecl GetOriginalMapFromText(wstring OriginalText, LanguagePairDics languagePairDics) {
//	std::wregex regexp(L"msgctxt \"(.*)\"\\nmsgid ([\\s\\S]*?)\\nmsgstr ([\\s\\S]+?)\"\\n\\n");
//	const wsregex_iterator itEnd;
//	if (languagePairDics.size() == 0) {
//		LanguagePairDic languagePairDic;
//		for (wsregex_iterator it(OriginalText.begin(), OriginalText.end(), regexp); it != itEnd; it++) {
//			wstring key = (*it)[1];
//			key = ReplaceAll(key, L"\"", L"");
//			wstring OriginalText = (*it)[2];
//			wregex regex1(L"^\"|\"\n\"|\"$");
//			OriginalText = regex_replace(OriginalText, regex1, wstring(L""));
//			OriginalText = ReplaceAll(trim(OriginalText), L"\\n", L"\n");
//			OriginalText = ReplaceAll(trim(OriginalText), L"\\\"", L"\"");
//			wstring TranslatedText = (*it)[3];
//			TranslatedText = regex_replace(TranslatedText, regex1, wstring(L""));
//			TranslatedText = ReplaceAll(trim(TranslatedText), L"\\n", L"\n");
//			TranslatedText = ReplaceAll(trim(TranslatedText), L"\\\"", L"\"");
//			LanguagePair languagePair;
//			languagePair.Original = OriginalText;
//			languagePair.Translated = TranslatedText;
//			languagePairDic.Dic.insert(pair<wstring, LanguagePair>(key, languagePair));
//		}
//		languagePairDics[L"0"] = languagePairDic;
//	}
//	return languagePairDics;
//}
//
//wstring _cdecl GetTranslatedTextFromMap(wstring OriginalText, LanguagePairDics TranslatedMap) {
//	return L"";
//}

LanguagePairDics _cdecl GetUpdateFileTextFromMap(LanguagePairDics UpdateMap) {
	for (LanguagePairDics::iterator itr = UpdateMap.begin();
		itr != UpdateMap.end(); itr++) {
		itr->second.TranslatedFileText = L"";
		int i = 0;
		for (map<wstring, LanguagePair>::iterator langPairItr = itr->second.Dic.begin(); langPairItr != itr->second.Dic.end(); langPairItr++, i++) {
			wstring originalText = langPairItr->second.Original;
			originalText = ReplaceAll(originalText, L"\n", L"\\n");
			originalText = ReplaceAll(originalText, L"\"", L"\\\"");
			originalText = trim(originalText);
			wstring translatedText = langPairItr->second.Translated;
			translatedText = ReplaceAll(translatedText, L"\n", L"\\n");
			translatedText = ReplaceAll(translatedText, L"\"", L"\\\"");
			translatedText = trim(translatedText);
			wstring potString = L"#: " + to_wstring((long long)i) + L"\n" +
				L"msgctxt \"" + langPairItr->first +
				L"\"\nmsgid \"" +
				originalText +
				L"\"\n" +
				L"msgstr \"" +
				translatedText +
				L"\"\n\n";
			itr->second.TranslatedFileText += potString;
		}
		if (itr->second.InteractWithFileTextOptions.size() == 0) {
			AssetMapOption fileNameOption = AssetMapOption(
				L"File Name:",
				L"Set File Name saved.",
				new wstring(itr->first + L".po"),
				NULL,
				AssetMapOption::OPTION_TYPE_WSTRING,
				AssetMapOption::OPTION_TYPE_NONE,
				vector<AssetMapOption>()
			);
			itr->second.InteractWithFileTextOptions.push_back(fileNameOption);
		}
		else {
			itr->second.TranslatedFileName = *(wstring*)itr->second.InteractWithFileTextOptions[0].Value;
		}
	}
	return UpdateMap;
}

LanguagePairDics _cdecl GetTranslatedMapFromFileText(LanguagePairDics translatedMap) {
	for (LanguagePairDics::iterator itr = translatedMap.begin();
		itr != translatedMap.end(); itr++) {
		// https://stackoverflow.com/questions/28629306/regex-options-for-stdregex in C++ regex \s\S does not work.
		std::wregex regexp(L"msgctxt \"(.*)\"\\nmsgid ((?:.|\n)*?)\\nmsgstr ((?:.|\n)*?)\"\\n\\n");
		const wsregex_iterator itEnd;
		wsregex_iterator it(itr->second.TranslatedFileText.begin(), itr->second.TranslatedFileText.end(), regexp);
		LanguagePairDic languagePairDic;
		int i = 0;
		for (; it != itEnd; it++, i++) {
			wstring key = (*it)[1];
			key = ReplaceAll(key, L"\"", L"");
			wstring OriginalText = (*it)[2];
			wregex regex1(L"^\"|\"\n\"|\"$");
			OriginalText = regex_replace(OriginalText, regex1, wstring(L""));
			OriginalText = ReplaceAll(trim(OriginalText), L"\\n", L"\n");
			OriginalText = ReplaceAll(trim(OriginalText), L"\\\"", L"\"");
			wstring TranslatedText = (*it)[3];
			TranslatedText = regex_replace(TranslatedText, regex1, wstring(L""));
			TranslatedText = ReplaceAll(trim(TranslatedText), L"\\n", L"\n");
			TranslatedText = ReplaceAll(trim(TranslatedText), L"\\\"", L"\"");
			LanguagePair languagePair;
			languagePair.Original = OriginalText;
			languagePair.Translated = TranslatedText;
			languagePairDic.Dic.insert(pair<wstring, LanguagePair>(key, languagePair));
		}
		itr->second = languagePairDic;
	}
	return translatedMap;
}


bool _cdecl GetTextPluginInfo(TextPluginInfo* textPluginInfo) {
	TextPluginInfoGlobal = textPluginInfo;

	wcsncpy_s(textPluginInfo->TextPluginName, L"GetTextPo", 9);
	//textPluginInfo->GetOriginalMapFromText = GetOriginalMapFromText;
	//textPluginInfo->GetTranslatedTextFromMap = GetTranslatedTextFromMap;

	textPluginInfo->GetUpdateFileTextFromMap = GetUpdateFileTextFromMap;
	textPluginInfo->GetTranslatedMapFromFileText = GetTranslatedMapFromFileText;
	return true;
}