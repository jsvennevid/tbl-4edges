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

				.name ShaderFlatVu1
                .syntax new
                .vu
                .init_vf_all
                .init_vi_all

                --enter
				in_vi			vertexOffset (vi11)
				in_vi			inputStream (vi12)
				in_vi			outputStream (vi13)
				in_vi			vertexCount (vi14)
				in_vi			retAddress (vi15)
                --endenter

shader:

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				iaddiu 			vertexStream, outputStream, 0
				iaddiu 			verCount, vertexCount, 0 

				loi				0.0
nextVertex:
				lq				color, 1(vertexStream)
				add.w			color, color, i
				sq				color, 1(vertexStream)
				iadd			vertexStream, vertexStream, vertexOffset
				
				iaddi			verCount, verCount, -1
                ibne            verCount, vi00, nextVertex

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Barrier will not do as vcl removes more than one

				.vsm

				nop				nop
				nop				nop
				nop				nop
				nop				nop

				.endvsm


				--exit
				out_vi			vertexOffset (vi11)
				out_vi			inputStream (vi12)
				out_vi			outputStream (vi13)
				out_vi			vertexCount (vi14)
				out_vi			retAddress (vi15)
                --endexit




