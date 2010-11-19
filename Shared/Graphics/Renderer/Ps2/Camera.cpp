///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <Shared/Hardware/Ps2/MasterChain.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Math/Vector3.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Camera::Camera()
{
	m_minZ = 115;
	m_maxZ = 65535;
	//m_maxZ = 0x2fffff;
	m_maxZ = 0x5fffff;

	m_width = 512; 
	m_height = 448;

	m_nearPlane = 1.0f; 
	m_farPlane = 15100.0f;
	m_viewWindowSize = 0.8f;

	m_aspectRatio = ((float)m_width / (float)m_height);

	init();

	m_matrix = Matrix4::identity();

	setPosition(Vector4(0.0f, 0.0f, 5.0f, 0.0f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Camera::init()
{
	m_clipMatrix = Matrix4::identity();

	const int iGuardDimX = 4094;	// yes, 4094 is correct (not 4096 due to float roading errors)
	const int iGuardDimY = 4094;

	float* matrix = m_clipMatrix.matrix();

	const float ax = m_width * m_viewWindowSize / iGuardDimX;
	const float ay = (m_height * m_aspectRatio) * m_viewWindowSize / iGuardDimY;	// here aspectRatio would be 4/3

	const float k1 = -(m_nearPlane + m_farPlane) / (m_farPlane - m_nearPlane);
	const float k2 = -(m_farPlane * m_nearPlane * 2) / (m_farPlane - m_nearPlane);

	matrix[(0<<2)+0] = ax;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = ay;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 0;
	matrix[(2<<2)+1] = 0;
	matrix[(2<<2)+2] = k1;
	matrix[(2<<2)+3] = -1;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = k2;
	matrix[(3<<2)+3] = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Camera::addToChain(uint vu1RamPos)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	init();

	m_viewMatrix = m_matrix.fastInverse();
	dmaChain.addSrcCntTag(0,VIF_UNPACK(vif::V4_32, 10, vu1RamPos));

	// TODO: Use refchains here?

	float* matrix = m_clipMatrix.matrix();

	for (uint i = 0; i < 16; i++)
		dmaChain.addFloat(*matrix++);

	matrix = m_viewMatrix.matrix();

	for (uint i = 0; i < 16; i++)
		dmaChain.addFloat(*matrix++);

	const int iGuardDimX = 4094;
	const int iGuardDimY = 4094;

	const float scaleX = 0.5f*iGuardDimX;
	const float scaleY = -0.5f*iGuardDimY;
	const float scaleZ = -0.5f*(m_maxZ - m_minZ);

	const float offsetX = 2048;
	const float offsetY = 2048;
	const float offsetZ = 0.5f*(m_maxZ + m_minZ);

	dmaChain.addFloat(offsetX); dmaChain.addFloat(offsetY);
	dmaChain.addFloat(offsetZ); dmaChain.addFloat(0.0f);

	dmaChain.addFloat(scaleX); dmaChain.addFloat(scaleY);
	dmaChain.addFloat(scaleZ); dmaChain.addFloat(0.0f);
/*
	dmaChain.add32(VIF_BASE(24));
	dmaChain.add32(VIF_OFFSET(524));
	dmaChain.add32(0);
	dmaChain.add32(0);
*/
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
