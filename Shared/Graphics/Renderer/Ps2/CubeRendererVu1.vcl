//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

.include 		"vcl_sml.i"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClipMatrix        .assign 0
CameraMatrix      .assign 4
ClipToScreenShift .assign 8
ClipToScreenScale .assign 9
LocalMatrix       .assign 10
ProjectMatrix	  .assign 14
GifTag			  .assign 21
DirectionalLight  .assign 22
CubeCoords		  .assign 23

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				.name 	CubeRendererVu1
                .syntax new
                .vu
                .init_vf_all
				.init_vi_all
                --enter
                --endenter

                .syntax new
                .vu
                .init_vf_all
				.init_vi_all
                --enter
                --endenter

				matrixLoad		cameraMatrix, CameraMatrix, vi00
				matrixLoad		clipMatrix, ClipMatrix, vi00

				matrixMultiplyXYZ1	cameraMatrix, cameraMatrix, clipMatrix

				lq.xyz			clipToScreenShift, ClipToScreenShift(vi00)
				lq.xyz			clipToScreenScale, ClipToScreenScale(vi00)

				--cont
				
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


renderCube:
				xtop			coordsDest
				matrixLoad 			relMatrix, 0, coordsDest 
				matrixMultiplyXYZ1	matrix, relMatrix, cameraMatrix


				xtop			coordsDestKick
				iaddiu			coordsDestKick, coordsDestKick, 4
				iaddiu			coordsDest, coordsDest, 5

				iaddiu			inputStream, vi00, CubeCoords
				//lq				gifTag, GifTag(vi00)
				//sqi				gifTag, (coordsDest++)

				iaddiu			nextQuadCounter, vi00, 6

				// Transform light into clipspace

                lq				light, DirectionalLight(vi00)

				fcset			0

nextQuad:

                lq.xyz          vertex1, 0(inputStream)
				mul             acc, relMatrix[0], vertex1[x]
				madd            acc, relMatrix[1], vertex1[y]
				madd            acc, relMatrix[2], vertex1[z]
				madd            vertex1, relMatrix[3], vf00[w]

                lq.xyz          vertex2, 1(inputStream)
				mul             acc, relMatrix[0], vertex2[x]
				madd            acc, relMatrix[1], vertex2[y]
				madd            acc, relMatrix[2], vertex2[z]
				madd            vertex2, relMatrix[3], vf00[w]

                lq.xyz          vertex3, 2(inputStream)
				mul             acc, relMatrix[0], vertex3[x]
				madd            acc, relMatrix[1], vertex3[y]
				madd            acc, relMatrix[2], vertex3[z]
				madd            vertex3, relMatrix[3], vf00[w]

				// calculate normal
				
				sub.xyz         edge12, vertex2, vertex1
				sub.xyz         edge13, vertex3, vertex1
				
				opmula.xyz      ACC, edge12, edge13
				opmsub.xyz      polyNormal, edge13, edge12
				normal			polyNormal, polyNormal				

				// calculate direction to the light and normalize it

				sub.w			polyNormal, vf00, vf00

				dotACC			lightDot, polyNormal, light
			    max.x           lightDot, lightDot, vf00[x]
				loi				254
				max				color, vf00, vf00[w]
				mul				color, color, lightDot[x]
				mul				color, color, I
				ftoi0			color, color
				

				;--LoopCS 3,3

				// Vertex1

                lq.xyz          vertex1, 0(inputStream)
				mul             acc, matrix[0], vertex1[x]
				madd            acc, matrix[1], vertex1[y]
				madd            acc, matrix[2], vertex1[z]
				madd            vertex1, matrix[3], vf00[w]

				clipw.xyz		vertex1, vertex1
				fcand			vi01, 0x3ffff
				iaddiu			v1Clip, vi01, 0x7fff
		
				// Vertex 2

                lq.xyz          vertex2, 1(inputStream)
				mul             acc, matrix[0], vertex2[x]
				madd            acc, matrix[1], vertex2[y]
				madd            acc, matrix[2], vertex2[z]
				madd            vertex2, matrix[3], vf00[w]
				div				q, vf00[w], vertex2[w]
				mul				vertex2, vertex2, q

				clipw.xyz		vertex2, vertex2
				fcand			vi01, 0x3ffff
				iaddiu			v2Clip, vi01, 0x7fff

				// Vertex 3 
				
                lq.xyz          vertex3, 2(inputStream)
				mul             acc, matrix[0], vertex3[x]
				madd            acc, matrix[1], vertex3[y]
				madd            acc, matrix[2], vertex3[z]
				madd            vertex3, matrix[3], vf00[w]
				div				q, vf00[w], vertex3[w]
				mul				vertex3, vertex3, q

				clipw.xyz		vertex3, vertex3
				fcand			vi01, 0x3ffff
				iaddiu			v3Clip, vi01, 0x7fff

				// Vertex 4
				
                lq.xyz          vertex4, 3(inputStream)
				mul             acc, matrix[0], vertex4[x]
				madd            acc, matrix[1], vertex4[y]
				madd            acc, matrix[2], vertex4[z]
				madd            vertex4, matrix[3], vf00[w]
				div				q, vf00[w], vertex4[w]
				mul				vertex4, vertex4, q

				clipw.xyz		vertex4, vertex4
				fcand			vi01, 0x3ffff
				iaddiu			v4Clip, vi01, 0x7fff


				///////////////////////////////////////////////////////////////////////

				div				q, vf00[w], vertex1[w]
				mul				vertex1, vertex1, q

				div				q, vf00[w], vertex2[w]
				mul				vertex2, vertex2, q

				div				q, vf00[w], vertex3[w]
				mul				vertex3, vertex3, q

				div				q, vf00[w], vertex4[w]
				mul				vertex4, vertex4, q

				ior				v1Clip, v1Clip, v2Clip
				ior				v1Clip, v1Clip, v3Clip
				ior				v1Clip, v1Clip, v4Clip

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertex1, vertex1, clipToScreenScale
				mfir.w			vertex1, v1Clip
                ftoi4.xyz       vertex1, vertex1

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertex2, vertex2, clipToScreenScale
				mfir.w			vertex2, v1Clip
                ftoi4.xyz       vertex2, vertex2

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertex3, vertex3, clipToScreenScale
				mfir.w			vertex3, v1Clip
                ftoi4.xyz       vertex3, vertex3

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertex4, vertex4, clipToScreenScale
				mfir.w			vertex4, v1Clip
                ftoi4.xyz       vertex4, vertex4

				sq				color, 0(coordsDest):kick
                sq              vertex1, 1(coordsDest):kick
                sq              vertex2, 2(coordsDest):kick
                sq              vertex3, 3(coordsDest):kick

				sq				color, 4(coordsDest):kick
                sq              vertex1, 5(coordsDest):kick
                sq              vertex3, 6(coordsDest):kick
                sq              vertex4, 7(coordsDest):kick

				iaddiu			inputStream, inputStream, 4
				iaddiu			coordsdest, coordsdest, 8
		
                iaddi           nextQuadCounter, nextQuadCounter, -1
                ibne            nextQuadCounter, vi00, nextQuad

				--barrier
				xgkick          coordsDestKick
				--barrier

				--cont			
				b				renderCube

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				--exit
				--endexit

.END








