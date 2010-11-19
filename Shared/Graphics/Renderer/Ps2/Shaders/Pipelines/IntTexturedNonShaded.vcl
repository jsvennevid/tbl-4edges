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
GifTagFan		  .assign 20
GifTag			  .assign 20

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				.name 	IntTexturedNonShaded
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

				iadd			vertexCounter, vi00, vertexCount
				iaddiu			inputStream, settings, 1

				lq				gifTag, GifTag(vi00)
				sqi				gifTag, (coordsDest++)

				add.z			uv, vf00, vf00[w]
nextVertex:
				--LoopCS 3,3

				move			vertex, vf00
                lq.xyz          vertex, 0(inputStream)
                lq				color, 1(inputStream)
                lq.xy			uv, 2(inputStream)
				itof12.xy		uv, uv
				itof4.w			color, color
				ftoi12.w		color, color
				iaddiu			inputStream, inputStream, 3

				itof0.xyz		vertex, vertex

				mul             acc, matrix[0], vertex[x]
				madd            acc, matrix[1], vertex[y]
				madd            acc, matrix[2], vertex[z]
				madd            vertexOut, matrix[3], vf00[w]
		
				clipw.xyz		vertexOut, vertexOut

				fcand			vi01, 0x3ffff
				iaddiu			vi01, vi01, 0x7fff
				mtir			adc, color[w]
				ior				vi01, vi01, adc

				div				q, vf00[w], vertexOut[w]
				mul				vertexOut, vertexOut, q
				mul.xyz			stq, uv, q
									
				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertexOut, vertexOut, clipToScreenScale
				mfir.w			vertexOut, vi01

                ftoi4.xyz       vertexOut, vertexOut

				sq.xyz			stq, 0(coordsDest):kick
				sq				color, 1(coordsDest):kick
                sq              vertexOut, 2(coordsDest):kick
				iaddiu			coordsdest, coordsDest, 3

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






