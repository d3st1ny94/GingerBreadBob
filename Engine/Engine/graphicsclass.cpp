////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_GridBitmap = 0;
	m_Text = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	// Initialize the text object.
	D3DXMATRIX viewmatrix;
	m_Camera->GetViewMatrix(viewmatrix);
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, viewmatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}
	m_SimpleTexShader = new SimpleTextureShader;
		if (!m_SimpleTexShader)
	{
		return false;
	}

	// Initialize the texture shader objects.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	result = m_SimpleTexShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap objects.
	m_GridBitmap = new GridBitmapClass;
	if (!m_GridBitmap)
	{
		return false;
	}
	m_simpleBitmap = new BitmapClass;
	if (!m_simpleBitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_simpleBitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/background.dds", screenWidth / 4, screenHeight / 4);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	result = m_GridBitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/spritesheet.dds", screenWidth / 4, screenHeight / 4);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the bitmap objects.
	if (m_GridBitmap)
	{
		m_GridBitmap->Shutdown();
		delete m_GridBitmap;
		m_GridBitmap = 0;
	}
	if (m_simpleBitmap)
	{
		m_simpleBitmap->Shutdown();
		delete m_simpleBitmap;
		m_simpleBitmap = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	if (m_SimpleTexShader)
	{
		m_SimpleTexShader->Shutdown();
		delete m_SimpleTexShader;
		m_SimpleTexShader = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame(float deltatime, std::vector<std::vector<int>>& toDraw, int score)
{
	bool result;

	CurrTileX += 40 * deltatime;
	// Render the graphics scene.
	result = Render(deltatime, toDraw, score);
	if(!result)
	{
		return false;
	}

	return true;
}

#include <cmath>

static float rad = 3.14 / 2.0f;
int lastScore = 0;
bool GraphicsClass::Render(float deltatime, std::vector<std::vector<int>>& toDraw, int score)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, rotationZ;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 0.0f);
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();


	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	//DrawingStuff
	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	D3DXMATRIX S, R;
	D3DXMatrixScaling(&S, m_GridBitmap->GetMaxWidth(), m_GridBitmap->GetMaxHeight(), 0);
	//prepare background bitmap for drawing
	result = m_simpleBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}
	//draw background 

	result = m_SimpleTexShader->Render(m_D3D->GetDeviceContext(), 6, S, viewMatrix, orthoMatrix, m_simpleBitmap->GetTexture());

	//Transform World Matrix with a rotation matrix
	/*const float piOverTwo = 3.14f / 2.0f;
	if (rad < 3.1 * 2.0f){
		rad += (piOverTwo * deltatime);
	}
	else{
		rad += fmod((piOverTwo * deltatime), (3.14f * 2.0f));
	}
	D3DXMatrixRotationZ(&R, rad);*/
	D3DXMatrixScaling(&S, 128, 128, 0);

	worldMatrix = S;
	worldMatrix._41 = 32;
	worldMatrix._42 = m_GridBitmap->GetMaxHeight() / 2 - 96;
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_GridBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if(!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	// draw instances
	for (int i = 0; i < toDraw.size(); i++){
		for (int k = 0; k < toDraw[k].size(); k++){
			int mInst = ((i*toDraw[k].size()) + k);
			if (toDraw.at(i).at(k) == 1)
			{
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), 6, mInst, worldMatrix, viewMatrix, orthoMatrix, m_GridBitmap->GetTexture());
				if (!result)
				{
					return false;
				}
			}
		}
	}
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), 6, 1, worldMatrix, viewMatrix, orthoMatrix, m_GridBitmap->GetTexture());			
	


	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();
	//update the score
	if (lastScore != score)
	{
		m_Text->UpdateScore(score, m_D3D->GetDeviceContext());
		lastScore = score;
	}
	// Render the text strings.
	m_D3D->GetWorldMatrix(worldMatrix);
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}