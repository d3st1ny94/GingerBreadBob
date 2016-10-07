////////////////////////////////////////////////////////////////////////////////
// Filename: GridBitmapClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GridBitmapClass.h"


GridBitmapClass::GridBitmapClass()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
}


GridBitmapClass::GridBitmapClass(const GridBitmapClass& other)
{
}


GridBitmapClass::~GridBitmapClass()
{
}


bool GridBitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;


	// Store the screen size.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the size in pixels that this bitmap should be rendered at.
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// Initialize the previous rendering position to negative one.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this bitmap.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void GridBitmapClass::Shutdown()
{
	// Release the bitmap texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

int GridBitmapClass::GetVertexCount()
{
	return m_vertexCount;
}

int GridBitmapClass::GetInstanceCount()
{
	return m_instanceCount;
}

bool GridBitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	if (firstRender)
	{
		result = SetVBuffer(deviceContext);
		firstRender = false;
	}
	else
	{
		result = true;
	}
	if(!result)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int GridBitmapClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* GridBitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool GridBitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;
	int i;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	//// Create the index array.
	//indices = new unsigned long[m_indexCount];
	//if(!indices)
	//{
	//	return false;
	//}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	//// Load the index array with data.
	//for(i=0; i<m_indexCount; i++)
	//{
	//	indices[i] = i;
	//}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	//// Set up the description of the static index buffer.
 //   indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
 //   indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
 //   indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
 //   indexBufferDesc.CPUAccessFlags = 0;
 //   indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
 //   indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;
	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	// set instance data

	m_instanceCount = 56;
	instances = new InstanceType[m_instanceCount];
	if (!instances){
		return false;
	}
	int rows = 7;
	for (int i = 0; i < m_instanceCount; i++)
	{
		instances[i].position = D3DXVECTOR3(i % rows, -(i / rows), 0);
	}
	/*
	instances[0].position = D3DXVECTOR3(0, 0, 0);
	instances[1].position = D3DXVECTOR3(1, -1, 0);
	instances[2].position = D3DXVECTOR3(2, -2, 0);
	instances[3].position = D3DXVECTOR3(3, -3, 0);
	instances[4].position = D3DXVECTOR3(4, -4, 0);
	instances[5].position = D3DXVECTOR3(5, -5, 0);
	instances[6].position = D3DXVECTOR3(6, -6, 0);
	instances[7].position = D3DXVECTOR3(7, -7, 0);
	instances[8].position = D3DXVECTOR3(0, -1, 0);
	instances[9].position = D3DXVECTOR3(1, -2, 0);
	instances[10].position = D3DXVECTOR3(2, -3, 0);
	instances[11].position = D3DXVECTOR3(3, -4, 0);*/
	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;
	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	/*delete [] indices;
	indices = 0;*/

	delete[] instances;
	instances = 0;

	return true;
}


void GridBitmapClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


bool GridBitmapClass::SetVBuffer(ID3D11DeviceContext* deviceContext)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	//// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	//// currently has the correct parameters.
	//if((positionX == m_previousPosX) && (positionY == m_previousPosY))
	//{
	//	return true;
	//}
	//
	//// If it has changed then update the position it is being rendered to.
	//m_previousPosX = positionX;
	//m_previousPosY = positionY;

	//// Calculate the screen coordinates of the left side of the bitmap.
	//left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	//// Calculate the screen coordinates of the right side of the bitmap.
	//right = left + (float)m_bitmapWidth;

	//// Calculate the screen coordinates of the top of the bitmap.
	//top = (float)(m_screenHeight / 2) - (float)positionY;

	//// Calculate the screen coordinates of the bottom of the bitmap.
	//bottom = top - (float)m_bitmapHeight;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	//vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	//vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	//vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	//vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	//vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	//vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//// Second triangle.
	//vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	//vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	//vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	//vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	//vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	//vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//First triangle.
	vertices[0].position = D3DXVECTOR3(-0.5, 0.5, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(0.5, -0.5, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(-0.5, -0.5, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(-0.5, 0.5, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(0.5, 0.5, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(0.5, -0.5, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void GridBitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];
	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);
	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;
	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool GridBitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void GridBitmapClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}