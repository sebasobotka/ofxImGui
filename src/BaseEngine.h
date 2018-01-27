#pragma once

#include "ofEvents.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace ofxImGui
{
	class BaseEngine
	{
	public:
		BaseEngine()
			: isSetup(false)
		{}

		virtual ~BaseEngine()
		{}

		virtual void setup() = 0;
		virtual void exit() = 0;
		virtual bool createDeviceObjects() = 0;
		virtual void invalidateDeviceObjects() = 0;

		virtual void onMouseDragged(ofMouseEventArgs& event);
		virtual void onMousePressed(ofMouseEventArgs& event);
		virtual void onMouseReleased(ofMouseEventArgs& event);
		virtual void onMouseScrolled(ofMouseEventArgs& event);
		virtual void mouseMoved(ofMouseEventArgs& event);
		virtual void touchDown(ofTouchEventArgs& touch);
		virtual void touchUp(ofTouchEventArgs& touch);
		virtual void touchMoved(ofTouchEventArgs& touch);
		virtual void onKeyPressed(ofKeyEventArgs& event);
		virtual void onKeyReleased(ofKeyEventArgs& event) = 0;
		virtual void onWindowResized(ofResizeEventArgs& window);
		
		virtual ofVec2f getMousePos();
		virtual void setMousePos(float x, float y);

		virtual GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

		static const char* getClipboardString(void* user_data);
		static void setClipboardString(void* user_data, const char * text);

		static int g_ShaderHandle;
		static int g_VertHandle;
		static int g_FragHandle;

		static int g_UniformLocationTex;
		static int g_UniformLocationProjMtx;
		static int g_AttribLocationPosition;
		static int g_AttribLocationUV;
		static int g_AttribLocationColor;

		static unsigned int g_VboHandle;
		static unsigned int g_ElementsHandle;

		bool mousePressed[5] = { false };
		bool mouseReleased = true;
		ofVec2f mouseCursorPos = ofVec2f(0, 0);
		int framesToClearMousePos = 0;
		int frameNr;

	protected:
		bool isSetup;
		
	};
}

