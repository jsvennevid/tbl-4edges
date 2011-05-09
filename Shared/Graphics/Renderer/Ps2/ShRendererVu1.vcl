///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

.include 		"vcl_sml.i"
.include		"pow.i"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClipMatrix		.assign 0
CameraMatrix		.assign 4
ClipToScreenShift	.assign 8
ClipToScreenScale	.assign 9
LocalMatrix		.assign 10
Parameters		.assign 14
GifTag			.assign 20
DirectionalLight	.assign 22

StripCoordsA		.assign 32
StripCoordsB		.assign 544 ; 512+32

	// result: resulting 3D vector
	// thetaphi: theta in X, phi in Z
	// angles02: angle 0 in X, angle 2 in Z
	// angles46: angle 4 in X, angle 6 in Z (this could be dealt with using a shift?)
	// powers: power values 1,3,5,7 in XYZW
	.macro evalSH	result, thetaphi, angles02, angles46, powers
	mul.xz		temp_02, \angles02, \thetaphi[x] // multiply with phi
	AngleSinCos	temp_02, sincos_02		; result: sin4, cos4, sin6, cos6

	mul.xz		temp_46, \angles46, \thetaphi[z] // multiply with theta
	AngleSinCos	temp_46, sincos_46		; result: sin4, cos4, sin6, cos6

	sub		zero, vf00, vf00 

	add.x		inpow, zero, sincos_02[x]
	add.y		inpow, zero, sincos_02[w]
	add.z		inpow, zero, sincos_46[x]
	add.w		inpow, zero, sincos_46[w]	; inpow: sin0, cos2, sin4, cos6

	POW		outpow, inpow, \powers

	add.x		outpow, outpow, outpow[y]
	add.x		outpow, outpow, outpow[z]
	add.x		outpow, outpow, outpow[w]
	add		outpow, zero, outpow[x]		; distribute to all members

	AngleSinCos	\thetaphi, sincos_thetaphi	; result: sintheta, costheta, sinphi, cosphi

	mul.xz		\result, outpow, sincos_thetaphi[z]
	mul.y		\result, outpow, sincos_thetaphi[w]
	mul.x		\result, \result, sincos_thetaphi[y]
	mul.z		\result, \result, sincos_thetaphi[x]
	.endm


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		.name 	ShRendererVu1
                .syntax new
                .vu
                .init_vf_all
				.init_vi_all
                --enter
                --endenter

		matrixLoad	cameraMatrix, CameraMatrix, vi00
		matrixLoad	clipMatrix, ClipMatrix, vi00
		matrixLoad	localMatrix, LocalMatrix, vi00

		matrixMultiplyXYZ1	cameraMatrix, cameraMatrix, clipMatrix
		matrixMultiplyXYZ1	matrix, localMatrix, cameraMatrix

		lq.xyz		clipToScreenShift, ClipToScreenShift(vi00)
		lq.xyz		clipToScreenScale, ClipToScreenScale(vi00)

		// Use the parameters for something fancy
		lq		angles02, Parameters(vi00)
		lq		angles46, Parameters+1(vi00)
		lq		powers,	Parameters+2(vi00) 
		lq		dthetaphi, Parameters+3(vi00)

		sub		thetaphi1, vf00, vf00
		add		thetaphi2, thetaphi1, dthetaphi

		iaddiu		stripCounter, vi00, 128

		iaddiu		coordsKickA, vi00, StripCoordsA
		iaddiu		coordsKickB, vi00, StripCoordsB

		loi		0.03125					// uv step
		sub.x		uvStep, vf00, vf00
		add.x		uvStep, vf00, i			// step, 0, 0, 0

		sub		uvy, vf00, vf00

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

nextStrip:
		iadd		coordsDest, vi00, coordsKickA

		sub.x		thetaphi1, vf00, vf00
		add.x		thetaphi2, thetaphi1, dthetaphi

		lq		gifTag, GifTag(vi00)
		sqi		gifTag, (coordsDest++)

		; 128 + 2 points
		iaddiu		vertexCount, vi00, 128+2
		iaddiu		vertexCounter, vi00, 64
		fcset		0

		sub		color, vf00, vf00
		
		// Step UV

		add		uv1, vf00, uvy
		add		uv2, uv1, vf00
		add.y		uv2, uv2, uvStep[x]		// next row


		// Setup Vertices
	
		// Code Generates 128 points

		add.z		uv1, vf00, vf00[w]
		add.z		uv2, vf00, vf00[w]
nextVertex:
		;--LoopCS 3,3

		add		uv1, uv1, uvStep[x]				
		add		uv2, uv2, uvStep[x]				

		add.x		thetaphi1, thetaphi1, dthetaphi
		add.x		thetaphi2, thetaphi2, dthetaphi

		evalSH		vertex1, thetaphi1, angles02, angles46, powers
		evalSH		vertex2, thetaphi2, angles02, angles46, powers



		// Process Vertex 1

		mul             acc, matrix[0], vertex1[x]
		madd            acc, matrix[1], vertex1[y]
		madd            acc, matrix[2], vertex1[z]
		madd            vertexOut1, matrix[3], vf00[w]

		normal		normal1, vertexOut1
		loi		0.5
		add.xy		uv1, normal1, i
		
		clipw.xyz	vertexOut1, vertexOut1

		fcand		vi01, 0x3ffff
		iaddiu		vi01, vi01, 0x7fff

		div		q, vf00[w], vertexOut1[w]
		mul		vertexOut1, vertexOut1, q
		mul.xyz		stq, uv1, q
									
		mul.xyz		acc, clipToScreenShift, vf00[w]
		madd.xyz	vertexOut1, vertexOut1, clipToScreenScale
		mfir.w		vertexOut1, vi01

                ftoi4.xyz       vertexOut1, vertexOut1

		sq.xyz		stq, 0(coordsDest):kick
		sq		color, 1(coordsDest):kick
                sq              vertexOut1, 2(coordsDest):kick
		iaddiu		coordsdest, coordsDest, 3

		// Process Vertex 2

		mul             acc, matrix[0], vertex2[x]
		madd            acc, matrix[1], vertex2[y]
		madd            acc, matrix[2], vertex2[z]
		madd            vertexOut2, matrix[3], vf00[w]

		normal		normal2, vertexOut2
		loi		0.5
		add.xy		uv2, normal2, i

		clipw.xyz	vertexOut2, vertexOut2

		fcand		vi01, 0x3ffff
		iaddiu		vi01, vi01, 0x7fff

		div		q, vf00[w], vertexOut2[w]
		mul		vertexOut2, vertexOut2, q
		mul.xyz		stq, uv2, q
									
		mul.xyz		acc, clipToScreenShift, vf00[w]
		madd.xyz	vertexOut2, vertexOut2, clipToScreenScale
		mfir.w		vertexOut2, vi01

                ftoi4.xyz       vertexOut2, vertexOut2

		sq.xyz		stq, 0(coordsDest):kick
		sq		color, 1(coordsDest):kick
                sq              vertexOut2, 2(coordsDest):kick
		iaddiu		coordsdest, coordsDest, 3

                iaddi           vertexCounter, vertexCounter, -1
                ibne            vertexCounter, vi00, nextVertex

		lq		temp, 1(coordsKickA):kick
		sq		temp, 0(coordsDest):kick
		lq		temp, 2(coordsKickA):kick
		sq		temp, 1(coordsDest):kick
		lq		temp, 3(coordsKickA):kick
		sq		temp, 2(coordsDest):kick

		lq		temp, 4(coordsKickA):kick
		sq		temp, 3(coordsDest):kick
		lq		temp, 5(coordsKickA):kick
		sq		temp, 4(coordsDest):kick
		lq		temp, 6(coordsKickA):kick
		sq		temp, 5(coordsDest):kick

		iaddiu		numVerticesGifTag, vertexCount, 0x7fff
		iaddiu		numVerticesGifTag, numVerticesGifTag, 1
		isw		numVerticesGifTag, 0(coordsKickA)x:kick

		--barrier
		xgkick          coordsKickA
		--barrier

		iadd		coordsKickTemp, vi00, coordsKickA
		iadd		coordsKickA, vi00, coordsKickB
		iadd		coordsKickB, vi00, coordsKickTemp

		add.z		thetaphi1, thetaphi1, dthetaphi
		add.z		thetaphi2, thetaphi2, dthetaphi

		add.y		uvy, uvy, uvStep[x]

		iaddi		stripCounter, stripCounter, -1
		ibne		stripCounter, vi00, nextStrip

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		--exit
		--endexit

.END
