//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

.include "../vcl_sml.i"

				.name Render_Branch
                 .syntax new
                .vu
                .init_vf_all
                .init_vi_all
                --enter
                --endenter

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Memory layout
//
//  00-03   Clip Matrix
//  04-07   WorldToClip Matrix
//  00008   ClipToScreenScale
//  00009   ClipToScreenAdd
//  10-13   Local Matrix
//  00018   TriFan GifTag
//  00019   TriStrip GifTag
//  00020   Coords

ClipMatrix        .assign 0
CameraMatrix      .assign 4
ClipToScreenShift .assign 8
ClipToScreenScale .assign 9
LocalMatrix       .assign 10
DummyTag		  .assign 16
GifTagFan		  .assign 22
GifTag			  .assign 23
Coords			  .assign 24
Dest			  .assign 250
NextBuffer		  .assign 500
CurrentOutBuffer  .assign 14(vi00)x
StartOutput		  .assign 14

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				matrixLoad	cameraMatrix, CameraMatrix, vi00
				matrixLoad	clipMatrix, ClipMatrix, vi00
				matrixLoad	localMatrix, LocalMatrix, vi00

				matrixMultiplyXYZ1	cameraMatrix, cameraMatrix, clipMatrix
				matrixMultiplyXYZ1	matrix, localMatrix, cameraMatrix

				lq.xyz			clipToScreenShift, ClipToScreenShift(vi00)
				lq.xyz			clipToScreenScale, ClipToScreenScale(vi00)

				// make a dummy giftag for debugging

				lq				dummyTag, GifTag(vi00)
				sq				dummyTag, DummyTag(vi00)	
				iaddiu			tempCount, vi00, 0x7fff
				iaddiu			tempCount, tempCount, 1
				isw				tempCount, DummyTag(vi00)x

				--cont

				xitop			sectionCount
				//isw.x			sectionCount, 13(vi00)x

				iaddiu			numVertices, vi00, 10
				iaddiu			numVerticesGifTag, numVertices, 0x7fff
				iaddiu			numVerticesGifTag, numVerticesGifTag, 1
nextChunk:
				xitop			sectionCount

				xtop			coords
				iaddiu			coordsDest, coords, Dest+1
				iaddiu			coordsDestKick, coords, Dest
nextSection:
				fcset		0

				iaddiu			vertexCounter, vi00, 10
				iadd			vertexCounter, vi00, numVertices


				lq				gifTag, GifTag(vi00)
				sq				gifTag, -1(coordsDest)

				////////////////////////////////////////////////////////////////////////////////////////////////////////
				// First pass, draw all strips and set ADC flag on them

nextVertex:
				;--LoopCS 3,3			
	

				lqi.xyz         vertex, (coords++)

				mul             acc, matrix[0], vertex[x]
				madd            acc, matrix[1], vertex[y]
				madd            acc, matrix[2], vertex[z]
				madd            vertex, matrix[3], vf00[w]

				clipw.xyz		vertex, vertex
				div				q, vf00[w], vertex[w]
				mul.xyz			vertex, vertex, q
				fcand			vi01, 0x3ffff
				iaddiu			vi01, vi01, 0x7fff
				mfir.w			vertex, vi01
									
				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertex, vertex, clipToScreenScale

                ftoi4.xyz       vertex, vertex

                sq              vertex, 0(coordsDest):kick
				iaddiu			coordsDest, coordsDest, 1

                iaddi           vertexCounter, vertexCounter, -1
                ibne            vertexCounter, vi00, nextVertex

				isw				numVerticesGifTag, 0(coordsDestKick)x:kick
				isw.w			numVerticesGifTag, 1(coordsDestKick)w:kick	
				isw.w			numVerticesGifTag, 2(coordsDestKick)w:kick	

				--barrier
				xgkick          coordsDestKick
				--barrier

				//isw.x			coordsDestKick, 14(vi00)x

				iadd			coordsDestKick, vi00, coordsDest				
				iaddiu			coordsDest, coordsDest, 1
				
                iaddi           sectionCount, sectionCount, -1
                ibne            sectionCount, vi00, nextSection

				--cont
				b				nextChunk

				--exit
                --endexit
.END


