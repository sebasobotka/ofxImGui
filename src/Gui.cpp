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
	void Gui::setup(BaseTheme* theme_) {
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

		if (theme_)	{
			setTheme(theme_);
		}
		else {
			setTheme(new BaseTheme());
		}
	}

	//--------------------------------------------------------------
	void Gui::SetDefaultFont(int indexAtlasFont) {
		ImGuiIO& io = ImGui::GetIO();
		if (indexAtlasFont < io.Fonts->Fonts.size()) {
			io.FontDefault = io.Fonts->Fonts[indexAtlasFont];
		}
		else {
			io.FontDefault = io.Fonts->Fonts[0];
		}
	}

	//--------------------------------------------------------------
	int Gui::addFont(const string & fontPath, float fontSize) {
			
		//ImFontConfig structure allows you to configure oversampling.
		//By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
		//than necessary, so you may reduce that to 1.

		static const ImWchar polishCharRanges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0100, 0x01FF, // Polish characters
			0,
		};
		
		ImGuiIO& io = ImGui::GetIO();
		string filePath = ofFilePath::getAbsolutePath(fontPath);

		char charFontPath[256];
		strcpy(charFontPath, filePath.c_str());
		//io.Fonts->AddFontFromFileTTF(fontPath, fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		ImFont* font = io.Fonts->AddFontFromFileTTF(charFontPath, fontSize, NULL, polishCharRanges);

		if (io.Fonts->Fonts.size() > 0) {
			return io.Fonts->Fonts.size() - 1;
		}
		else {
			return 0;
		}
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
	void Gui::addPrintableKeys(const string & input) {
	//ofLogNotice("Frame nr") << ofGetFrameNum() << " , " << __FUNCTION__ << " , " << input;
		virtKeyboardPrintableKeys.clear();

		std::vector<char> cvec(input.begin(), input.end());
		for (auto c : cvec) {
			virtKeyboardPrintableKeys.push_back(c);
		}
	}

	//--------------------------------------------------------------
	void Gui::passPrintableKeys() {
		ImGuiIO& io = ImGui::GetIO();

		if (virtKeyboardPrintableKeys.size() > 0) {
			//ofLogNotice("Buffer to inputtext") << ofToString(inputTextChars) << ", size: " << inputTextChars.size();
			// one char by one
			/*
			char c = bufToSend[0];
			bufToSend.erase(bufToSend.begin(), bufToSend.begin() + 1);
			gui.addAsciiChar(c);
			*/

			const int wcharsBufLen = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar) - 1;
			int sizeAllowedToAdd = wcharsBufLen;
			if (virtKeyboardPrintableKeys.size() >= wcharsBufLen) {
				sizeAllowedToAdd = wcharsBufLen;
			}
			else {
				sizeAllowedToAdd = virtKeyboardPrintableKeys.size();
			}

			for (int i = 0; i < sizeAllowedToAdd; i++) {
				char c = virtKeyboardPrintableKeys[i];
				io.AddInputCharacter(c);
			}
			virtKeyboardPrintableKeys.erase(virtKeyboardPrintableKeys.begin(), virtKeyboardPrintableKeys.begin() + sizeAllowedToAdd);
		}
	}

	//--------------------------------------------------------------
	void Gui::addFunctionKey(int key) {
		virtKeyboardFunctionKeys.emplace_back(virtFunctionKey{ key, true });
		virtKeyboardFunctionKeys.emplace_back(virtFunctionKey{ key, false });
	}

	//--------------------------------------------------------------
	void Gui::passFunctionKeys() {
		ImGuiIO& io = ImGui::GetIO();
		
		if (virtKeyboardFunctionKeys.size()) {
			
			ofLogNotice("Frame nr") << ofGetFrameNum() << " , mousePos: " << io.MousePos  << ", key: " << virtKeyboardFunctionKeys[0].key << ", state: " << virtKeyboardFunctionKeys[0].state;

			io.KeysDown[virtKeyboardFunctionKeys[0].key] = virtKeyboardFunctionKeys[0].state;
			virtKeyboardFunctionKeys.erase(virtKeyboardFunctionKeys.begin(), virtKeyboardFunctionKeys.begin() + 1);
		}
	}
	
	//--------------------------------------------------------------
	void Gui::setTheme(BaseTheme* theme_) {
		if (theme) {
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
	void Gui::openThemeColorWindow() {
		theme->themeColorsWindow(true);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(ofPixels& pixels) {
		return engine->loadTextureImage2D(pixels.getData(), pixels.getWidth(), pixels.getHeight());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(string imagePath) {
		if (!engine) return -1;
		ofPixels pixels;
		ofLoadImage(pixels, imagePath);
		return loadPixels(pixels);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(ofImage& image) {
		if (!engine) return -1;
		return loadPixels(image.getPixels());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(string imagePath) {
		return loadPixels(imagePath);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(string imagePath) {
		ofDisableArbTex();
		ofTexture* texture = new ofTexture();
		ofLoadImage(*texture, imagePath);
		ofEnableArbTex();
		loadedTextures.push_back(texture);
		return texture->getTextureData().textureID;
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(ofTexture& texture, string imagePath) {
		bool isUsingArb = ofGetUsingArbTex();
		if (isUsingArb)	{
			ofDisableArbTex();
		}
		ofLoadImage(texture, imagePath);
		if (isUsingArb)	{
			ofEnableArbTex();
		}
		return texture.getTextureData().textureID;
	}

	//--------------------------------------------------------------
	void Gui::begin() {
		if (!engine) {
			ofLogError(__FUNCTION__) << "setup() call required, calling it for you";
			setup();
		}

		ImGuiIO& io = ImGui::GetIO();

		float currentTime = ofGetElapsedTimef();
		if (lastTime > 0.f)	{
			io.DeltaTime = currentTime - lastTime;
		}
		else {
			io.DeltaTime = 1.0f / 60.f;
		}
		lastTime = currentTime;
		
		/*if (--engine->framesToClearMousePos == 0) {
			engine->setMousePos(-FLT_MAX, -FLT_MAX);
			ofLogNotice("Frame nr") << ofGetFrameNum() << " , " << __FUNCTION__ << "::clearing mouse pos";
		}*/
	
		if ((ofGetFrameNum() - engine->frameNr) == 2) {
			engine->setMousePos(-FLT_MAX, -FLT_MAX);
			//ofLogNotice("Frame nr") << ofGetFrameNum() << " , " << __FUNCTION__ << "::clearing mouse pos";
		}

		io.MousePos = ImVec2((float)engine->getMousePos().x, (float)engine->getMousePos().y);

		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine->mousePressed[i];
			// Update for next frame; set to false only if the mouse has been released
			engine->mousePressed[i] = !engine->mouseReleased;
		}

		// virtual keyboard
		passPrintableKeys();
		passFunctionKeys();

		ImGui::NewFrame();
	}

	//--------------------------------------------------------------
	void Gui::end()	{
		ImGui::Render();
	}

	//--------------------------------------------------------------
	void Gui::close() {
		if (engine) {
			delete engine;
			engine = nullptr;
		}
		//if(io)
		//{
		//    io->Fonts->TexID = 0;
		//    io = nullptr;
		//}
		if (theme) {
			delete theme;
			theme = nullptr;
		}
		for (size_t i = 0; i < loadedTextures.size(); i++) {
			delete loadedTextures[i];
		}
		loadedTextures.clear();
	}

	//--------------------------------------------------------------
	Gui::~Gui() {
		close();
		ImGui::Shutdown();
	}
}
