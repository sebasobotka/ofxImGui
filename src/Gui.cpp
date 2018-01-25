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
	void Gui::clearTouchState() {
		engine->mouseCursorPos.set(-FLT_MAX, -FLT_MAX);
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

		static const ImWchar ranges[] =
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
		ImFont* font = io.Fonts->AddFontFromFileTTF(charFontPath, fontSize, NULL, ranges);

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
	void Gui::addAsciiChar(int key) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(key);
	}

	//--------------------------------------------------------------
	void Gui::addSpecialKey(int key) {
		virtKeyboardSpecialKeys.emplace_back(virtSpecialKey{ key, true });
		virtKeyboardSpecialKeys.emplace_back(virtSpecialKey{ key, false });
	}

	//--------------------------------------------------------------
	void Gui::checkSpecialKeys() {
		if (virtKeyboardSpecialKeys.size()) {
			ofLogNotice("special key") << ofGetTimestampString() << ", " << virtKeyboardSpecialKeys[0].key << ", state: " << virtKeyboardSpecialKeys[0].state;
			sendSpecialKey(virtKeyboardSpecialKeys[0].key, virtKeyboardSpecialKeys[0].state);
			virtKeyboardSpecialKeys.erase(virtKeyboardSpecialKeys.begin(), virtKeyboardSpecialKeys.begin() + 1);
		}
	}
	
	//--------------------------------------------------------------
	void Gui::sendSpecialKey(int key, int state) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = state;
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
		
		//io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
		io.MousePos = ImVec2((float)engine->getMousePos().x, (float)engine->getMousePos().y);
	
		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine->mousePressed[i];
			// Update for next frame; set to false only if the mouse has been released
			engine->mousePressed[i] = !engine->mouseReleased;
		}

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
