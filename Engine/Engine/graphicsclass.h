////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "gridbitmapclass.h"
#include "GameTimer.h"
#include "SimpleTextureShader.h"
#include "bitmapclass.h"
#include "textclass.h"
#include <vector>

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float deltatime, std::vector<std::vector<int>>& toDraw, int score, bool gameover);

private:
	bool Render(float deltatime, std::vector<std::vector<int>>& toDraw, int score, bool gameover);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextureShaderClass* m_TextureShader;
	GridBitmapClass* m_GridBitmap;
	SimpleTextureShader* m_SimpleTexShader;
	BitmapClass* m_simpleBitmap;
	BitmapClass* m_GameOver;
	TextClass* m_Text;
	float CurrTileY = 100;
	float CurrTileX = 100;
	int lastScore = 0;
};

#endif