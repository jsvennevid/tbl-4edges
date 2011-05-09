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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				.name DebugRendererVu1
                .syntax new
                .vu
                .init_vf_all
                .init_vi_all
                --enter
                --endenter

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClipMatrix        .assign 0
CameraMatrix      .assign 4
ClipToScreenShift .assign 8
ClipToScreenScale .assign 9
LocalMatrix       .assign 10
GifTag			  .assign 20
Coords			  .assign 24
Dest			  .assign 250
NextBuffer		  .assign 500
Temp			  .assign 22

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				matrixLoad	cameraMatrix, CameraMatrix, vi00
				matrixLoad	clipMatrix, ClipMatrix, vi00
				matrixLoad	localMatrix, LocalMatrix, vi00

				matrixMultiplyXYZ1	matrix, cameraMatrix, clipMatrix
				//matrixMultiplyXYZ1	matrix, localMatrix, cameraMatrix

				lq.xyz			clipToScreenShift, ClipToScreenShift(vi00)
				lq.xyz			clipToScreenScale, ClipToScreenScale(vi00)
				
				--cont
nextChunk:
				fcset			0

				//xtop			coords
				iaddiu			coords, vi00, 22
				iaddiu			coordsDest, coords, Dest+1
				iaddiu			coordsDestKick, coords, Dest

				iaddiu			numVertices, vi00, 3		// num polygons
				iaddiu			vertexCounter, vi00, 3

				iaddiu			gifTagOffset, vi00, GifTag

				lq				gifTag, 0(gifTagOffset)
				sq				gifTag, -1(coordsDest)

				////////////////////////////////////////////////////////////////////////////////////////////////////////
				// First pass, draw all strips and set ADC flag on them

				add.z			uv, vf00, vf00[w]
                lq				color, 0(coords)
				iaddiu			coords, coords, 1
nextVertex:
                lq		        vertex, 0(coords)
				iaddiu			coords, coords, 1

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

				iaddiu			numVerticesGifTag, numVertices, 0x7fff
				iaddiu			numVerticesGifTag, numVerticesGifTag, 1
				isw				numVerticesGifTag, 0(coordsDestKick)x:kick

				isw.x			coordsDestKick, 11(vi00)x


				--barrier
				xgkick          coordsDestKick
				--barrier

				--cont
				b				nextChunk

				--exit
                --endexit

.END
