#include "Gui.h"
#include "ofAppRunner.h"

#if defined(TARGET_OPENGLES)
#include "EngineOpenGLES.h"
#elif defined (OF_TARGET_API_VULKAN)
#include "EngineVk.h"
#else
#include "EngineGLFW.h"
#endif

namespace ofxImGui
{
	//--------------------------------------------------------------
	Gui::Gui()
		: lastTime(0.0f)
		, engine(nullptr)
		, theme(nullptr)
	{}

	//--------------------------------------------------------------
	void Gui::setup(BaseTheme* theme_)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
		io.MouseDrawCursor = false;

#if defined(TARGET_OPENGLES)
		engine = new EngineOpenGLES();
#elif defined (OF_TARGET_API_VULKAN) 
		engine = new EngineVk();
#else 
	engine = new EngineGLFW();
#endif

		engine->setup();

		if (theme_)
		{
			setTheme(theme_);
		}
		else
		{
			setTheme(new BaseTheme());
		}
	}

	//--------------------------------------------------------------
	void Gui::addFont(string font, float fontSize) 
	{
		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0100, 0x01FF, // Polish characters
			0,
		};
		
		ImGuiIO& io = ImGui::GetIO();
		string filePath = ofFilePath::getAbsolutePath(font);
		char fontPath[256];
		strcpy(fontPath, filePath.c_str());
		//io.Fonts->AddFontFromFileTTF(fontPath, fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		io.Fonts->AddFontFromFileTTF(fontPath, fontSize, NULL, ranges);

		
		// HOW TO GET FONT, set default...
		//ImGuiIO& io = ImGui::GetIO();
		//ImFont* font_current = ImGui::GetFont();
		//font_current->GetDebugName()))
		//ImGuiIO& io = ImGui::GetIO();
		//ImFont* font_current = ImGui::GetFont();
		//if (ImGui::BeginCombo(label, font_current->GetDebugName()))
		//{
			//for (int n = 0; n < io.Fonts->Fonts.Size; n++)
				//if (ImGui::Selectable(io.Fonts->Fonts[n]->GetDebugName(), io.Fonts->Fonts[n] == font_current))
					//io.FontDefault = io.Fonts->Fonts[n];
			//ImGui::EndCombo();
		//}
	}

	//--------------------------------------------------------------
	void Gui::setImGuiWindowColor(ofColor & color, float alpha) {
		// alpha range 0.0 - 1.0
		// it can be global and called once for all windows
		// or called every frame for different windows

		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_WindowBg] = ImVec4(color, alpha);
	}

	//--------------------------------------------------------------
	void Gui::setInputTextFontColor(ofColor & color, float alpha) {
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(color, alpha);
		
		//text selected color
		//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(ofColor(0,0,255), 0.43f);
	}

	//--------------------------------------------------------------
	void Gui::setInputTextBackgroundColor(ofColor & color, float alpha) {
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_FrameBg] = ImVec4(color, alpha);
	}

	//--------------------------------------------------------------
	void Gui::pushInputTextDimension(float startCursorPosX, float fontSize, float inputTextWidth, float inputTextHeight) {
		
		float padY = (inputTextHeight - fontSize) / 2;
		ImGui::PushItemWidth(inputTextWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(startCursorPosX, padY));
	}

	//--------------------------------------------------------------
	void Gui::popInputTextDimension() {
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
	}

	//--------------------------------------------------------------
	void Gui::setTheme(BaseTheme* theme_)
	{
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}
		theme = theme_;
		theme->updateColors();
		theme->setup();
	}

	//--------------------------------------------------------------
	void Gui::setDefaultTheme()	{
		setTheme(new BaseTheme());
	}

	//--------------------------------------------------------------
	void Gui::openThemeColorWindow()
	{
		theme->themeColorsWindow(true);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(ofPixels& pixels)
	{
		return engine->loadTextureImage2D(pixels.getData(), pixels.getWidth(), pixels.getHeight());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(string imagePath)
	{
		if (!engine) return -1;
		ofPixels pixels;
		ofLoadImage(pixels, imagePath);
		return loadPixels(pixels);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(ofImage& image)
	{
		if (!engine) return -1;
		return loadPixels(image.getPixels());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(string imagePath)
	{
		return loadPixels(imagePath);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(string imagePath)
	{
		ofDisableArbTex();
		ofTexture* texture = new ofTexture();
		ofLoadImage(*texture, imagePath);
		ofEnableArbTex();
		loadedTextures.push_back(texture);
		return texture->getTextureData().textureID;
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(ofTexture& texture, string imagePath)
	{
		bool isUsingArb = ofGetUsingArbTex();
		if (isUsingArb)
		{
			ofDisableArbTex();
		}
		ofLoadImage(texture, imagePath);
		if (isUsingArb)
		{
			ofEnableArbTex();
		}
		return texture.getTextureData().textureID;
	}

	//--------------------------------------------------------------
	void Gui::begin()
	{
		if (!engine)
		{
			ofLogError(__FUNCTION__) << "setup() call required, calling it for you";
			setup();
		}

		ImGuiIO& io = ImGui::GetIO();

		float currentTime = ofGetElapsedTimef();
		if (lastTime > 0.f)
		{
			io.DeltaTime = currentTime - lastTime;
		}
		else
		{
			io.DeltaTime = 1.0f / 60.f;
		}
		lastTime = currentTime;

		// Update settings
		io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine->mousePressed[i];

			// Update for next frame; set to false only if the mouse has been released
			engine->mousePressed[i] = !engine->mouseReleased;
		}
		ImGui::NewFrame();
	}

	//--------------------------------------------------------------
	void Gui::end()
	{
		ImGui::Render();
	}

	//--------------------------------------------------------------
	void Gui::close()
	{
		if (engine)
		{
			delete engine;
			engine = nullptr;
		}
		//if(io)
		//{
		//    io->Fonts->TexID = 0;
		//    io = nullptr;
		//}
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}
		for (size_t i = 0; i < loadedTextures.size(); i++)
		{
			delete loadedTextures[i];
		}
		loadedTextures.clear();
	}

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		close();
		ImGui::Shutdown();
	}
}
