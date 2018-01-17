#pragma once

// For fast update ImGui version (only copy source to libs\imgui\src) here is some extra extension of library
// it should be done in imconfig.h but there were some issues


/*

	imgui.h

	struct ImFontAtlas
	{
		...
		IMGUI_API const ImWchar*    GetGlyphRangesPolish();     // Default + Polish characters
	}


	imgui_draw.cpp

	const ImWchar*  ImFontAtlas::GetGlyphRangesPolish()
	{
		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0100, 0x01FF, // Polish characters
			0,
			};
		return &ranges[0];
	}

*/

namespace ImGui {

	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}
}