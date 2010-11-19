//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

.include 		"../../vcl_sml.i"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClipMatrix        .assign 0
CameraMatrix      .assign 4
ClipToScreenShift .assign 8
ClipToScreenScale .assign 9
LocalMatrix       .assign 10
ProjectMatrix	  .assign 14
GifTag			  .assign 20
GifTagFlat		  .assign 21

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				.name 	FloatFlatNonShaded
                .syntax new
                .vu
                .init_vf_all
				.init_vi_all
                --enter
                --endenter

				matrixLoad		cameraMatrix, CameraMatrix, vi00
				matrixLoad		clipMatrix, ClipMatrix, vi00
				matrixLoad		localMatrix, LocalMatrix, vi00

				matrixMultiplyXYZ1	cameraMatrix, cameraMatrix, clipMatrix
				matrixMultiplyXYZ1	matrix, localMatrix, cameraMatrix

				lq.xyz			clipToScreenShift, ClipToScreenShift(vi00)
				lq.xyz			clipToScreenScale, ClipToScreenScale(vi00)

				--cont
				
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

clipPipe:
				fcset			0

				xitop			vertexCount
				xtop			settings
				ilw.z			kickBufferOffset, 0(settings)
				iadd			coordsDest, kickBufferOffset, settings
				iadd			coordsDestKick, coordsDest, vi00
				iaddiu			inputStream, settings, 1

				iadd			vertexCounter, vi00, vertexCount

				lq				gifTag, GifTagFlat(vi00)
				sqi				gifTag, (coordsDest++)
nextVertex:
				;--LoopCS 3,3

                lq.xyz          vertex, 0(inputStream)
                lq				color, 1(inputStream)
				iaddiu			inputStream, inputStream, 2

				mul             acc, matrix[0], vertex[x]
				madd            acc, matrix[1], vertex[y]
				madd            acc, matrix[2], vertex[z]
				madd            vertexOut, matrix[3], vf00[w]
		
				clipw.xyz		vertexOut, vertexOut

				fcand			vi01, 0x3ffff
				iaddiu			vi01, vi01, 0x7fff

				div				q, vf00[w], vertexOut[w]
				mul				vertexOut, vertexOut, q

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertexOut, vertexOut, clipToScreenScale
				mfir.w			vertexOut, vi01

                ftoi4.xyz       vertexOut, vertexOut

				sq				color, 0(coordsDest):kick
                sq              vertexOut, 1(coordsDest):kick
				iaddiu			coordsdest, coordsDest, 2

                iaddi           vertexCounter, vertexCounter, -1
                ibne            vertexCounter, vi00, nextVertex

				iaddiu			numVerticesGifTag, vertexCount, 0x7fff
				iaddiu			numVerticesGifTag, numVerticesGifTag, 1
				isw				numVerticesGifTag, 0(coordsDestKick)x:kick

				--barrier
				xgkick          coordsDestKick
				--barrier

				--cont			
				b				clipPipe

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				--exit
				--endexit

.END





