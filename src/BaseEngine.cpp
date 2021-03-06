#include "BaseEngine.h"
#include "ofAppRunner.h"
#include "imgui.h"

namespace ofxImGui
{
	int BaseEngine::g_ShaderHandle = 0;
	int BaseEngine::g_VertHandle = 0;
	int BaseEngine::g_FragHandle = 0;

	int BaseEngine::g_UniformLocationTex = 0;
	int BaseEngine::g_UniformLocationProjMtx = 0;
	int BaseEngine::g_AttribLocationUV = 0;

	int BaseEngine::g_AttribLocationPosition = 0;
	int BaseEngine::g_AttribLocationColor = 0;

	unsigned int BaseEngine::g_VboHandle = 0;
	unsigned int BaseEngine::g_ElementsHandle = 0;

	//--------------------------------------------------------------
	ofVec2f BaseEngine::getMousePos() {
		return mouseCursorPos;
	}

	//--------------------------------------------------------------
	void BaseEngine::setMousePos(float x, float y) {
		mouseCursorPos.set(x, y);
	}

	//--------------------------------------------------------------
	void BaseEngine::onKeyPressed(ofKeyEventArgs& event) {
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;
		//io->AddInputCharacter((unsigned short)event.codepoint);
	}

	//--------------------------------------------------------------
	void BaseEngine::onMousePressed(ofMouseEventArgs& event) {
		setMousePos(event.x, event.y);
		if (event.button >= 0 && event.button < 5) {
			mousePressed[event.button] = true;
			mouseReleased = false;
		}
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseReleased(ofMouseEventArgs& event) {
		setMousePos(event.x, event.y);
		mouseReleased = true;
	}

	//--------------------------------------------------------------
	void BaseEngine::mouseMoved(ofMouseEventArgs & event) {
		setMousePos((float)ofGetMouseX(), (float)ofGetMouseY());
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseDragged(ofMouseEventArgs& event) {
		setMousePos(event.x, event.y);
		mouseReleased = false;
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseScrolled(ofMouseEventArgs& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = event.scrollY;
	}

	//--------------------------------------------------------------
	void BaseEngine::touchDown(ofTouchEventArgs & touch) {
		setMousePos(touch.x, touch.y);
		mousePressed[0] = true;
		mouseReleased = false;
		ofLogNotice("Frame nr") << ofGetFrameNum() << " , " << __FUNCTION__ << " , " << ofGetTimestampString(); 
	}

	//--------------------------------------------------------------
	void BaseEngine::touchUp(ofTouchEventArgs & touch) {
		mouseReleased = true;
		//framesToClearMousePos = 3;
		//ofLogNotice("Frame nr") << ofGetFrameNum() << " , " << __FUNCTION__;
		
		// another approach, to set frameNr and then check ofGetFrameNum()-frameNr == 2
		frameNr = ofGetFrameNum();
	}

	//--------------------------------------------------------------
	void BaseEngine::touchMoved(ofTouchEventArgs & touch) {
		setMousePos(touch.x, touch.y);
		mouseReleased = false;
	}

	//--------------------------------------------------------------
	void BaseEngine::onWindowResized(ofResizeEventArgs& window)	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window.width, (float)window.height);
	}

	//--------------------------------------------------------------
	const char* BaseEngine::getClipboardString(void* user_data)	{
		return &ofGetWindowPtr()->getClipboardString()[0];
	}
	
	//--------------------------------------------------------------
	void BaseEngine::setClipboardString(void* user_data, const char * text)	{
		ofGetWindowPtr()->setClipboardString(text);
	}

	//--------------------------------------------------------------
	GLuint BaseEngine::loadTextureImage2D(unsigned char * pixels, int width, int height) {
		GLint last_texture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

		GLuint new_texture;
		glGenTextures(1, &new_texture);
		glBindTexture(GL_TEXTURE_2D, new_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width, height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels
		);

		glBindTexture(GL_TEXTURE_2D, last_texture);

		return new_texture;
	};
}
