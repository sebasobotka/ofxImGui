#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

#include "BaseEngine.h"
#include "BaseTheme.h"

namespace ofxImGui
{
	class Gui
	{
	public:
		Gui();
		~Gui();

		void setup(BaseTheme* theme = nullptr);
		void begin();
		void end();
		void close();
		
		void addFont(string font, float fontSize = 15.0f);
		void setImGuiWindowColor(ofColor & color, float alpha);
		void setInputTextFontColor(ofColor & color, float alpha);
		void setInputTextBackgroundColor(ofColor & color, float alpha);
		void pushInputTextDimension(float startCursorPosX, float fontSize, float inputTextWidth, float inputTextHeight);
		void popInputTextDimension();

		BaseEngine* engine;
		float lastTime;

		void setTheme(BaseTheme* theme);
		void setDefaultTheme();
		void openThemeColorWindow();

		BaseTheme* theme;

		GLuint loadImage(ofImage& image);
		GLuint loadImage(string imagePath);

		GLuint loadPixels(string imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(string imagePath);
		GLuint loadTexture(ofTexture& texture, string imagePath);

		vector<ofTexture*> loadedTextures;
	};
}
