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

.include 		"../../vcl_sml.i"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClipMatrix        .assign 0
CameraMatrix      .assign 4
ClipToScreenShift .assign 8
ClipToScreenScale .assign 9
LocalMatrix       .assign 10
LocalMatrixNoScale .assign 14
RenderType		  .assign 18
GifTagFan		  .assign 20
GifTag			  .assign 20

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				.name 	AlphaDissolveEnvmapped
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

				// Check if we should render the first pass with alpha texturemap or the envmap pass

				ilw.x			renderType, RenderType(vi00)
				ibne			renderType, vi00, envPipe

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
				;--LoopCS 3,3

				move			vertex, vf00
                lq.xyz          vertex, 0(inputStream)
                lq				color, 1(inputStream)
                lq.xy			uv, 2(inputStream)
				itof12.xy		uv, uv
				itof4.w			color, color
				ftoi12.w		color, color
				iaddiu			inputStream, inputStream, 4	// Skip normals

				//itof0.xyz		vertex, vertex

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

envPipe:
				matrixLoad		localMatrixNoScale, LocalMatrixNoScale, vi00
				--cont
envPipeLoop:
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
nextVertexEnv:
				;--LoopCS 3,3

				move			vertex, vf00
                lq.xyz          vertex, 0(inputStream)
                lq				color, 1(inputStream)
                lq.xyz			normal, 3(inputStream)
				itof15.xyz		normal, normal
				itof4.w			color, color
				ftoi12.w		color, color
				iaddiu			inputStream, inputStream, 4

				//itof0.xyz		vertex, vertex

				// Rotate normal

				mul             acc, matrix[0], vertex[x]
				madd            acc, matrix[1], vertex[y]
				madd            acc, matrix[2], vertex[z]
				madd            vertexOut, matrix[3], vf00[w]

				clipw.xyz		vertexOut, vertexOut

				// Rotate Project vertex
		
				mul             acc, localMatrixNoScale[0], normal[x]
				madd            acc, localMatrixNoScale[1], normal[y]
				madd            acc, localMatrixNoScale[2], normal[z]
				madd            normal, localMatrixNoScale[3], vf00[w]
		
				fcand			vi01, 0x3ffff
				iaddiu			vi01, vi01, 0x7fff
				mtir			adc, color[w]
				ior				vi01, vi01, adc

				div				q, vf00[w], vertexOut[w]
				mul				vertexOut, vertexOut, q

				// Do fake envmap calculation

				loi				0.5
				mul				normal, normal, I
				add				normal, normal, I
				mul				normal, normal, q
				add.z			normal, vf00, q		

				mul.xyz			acc, clipToScreenShift, vf00[w]
				madd.xyz		vertexOut, vertexOut, clipToScreenScale
				mfir.w			vertexOut, vi01

                ftoi4.xyz       vertexOut, vertexOut

				sq.xyz			normal, 0(coordsDest):kick
				sq				color, 1(coordsDest):kick
                sq              vertexOut, 2(coordsDest):kick
				iaddiu			coordsdest, coordsDest, 3

                iaddi           vertexCounter, vertexCounter, -1
                ibne            vertexCounter, vi00, nextVertexEnv

				iaddiu			numVerticesGifTag, vertexCount, 0x7fff
				iaddiu			numVerticesGifTag, numVerticesGifTag, 1
				isw				numVerticesGifTag, 0(coordsDestKick)x:kick

				--barrier
				xgkick          coordsDestKick
				--barrier

				--cont			
				b				envPipeLoop


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				--exit
				--endexit


.END







