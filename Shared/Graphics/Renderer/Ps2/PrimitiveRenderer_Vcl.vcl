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

                .syntax new
                .global Vu1PrimitiveRenderer             
                .global Vu1PrimitiveRendererEnd          
                .vu
                .init_vf_all
                .init_vi_all
                --enter
                --endenter
Vu1PrimitiveRenderer:
                iaddiu          vertexData, vi00, 7 //vertexOffset
                iaddiu          destAddress, vi00, 20 //destOffset
                iaddiu          kickAddress, vi00, 20 //kickOffset

                lq              worldToScreen[0], 0(vi00)
                lq              worldToScreen[1], 1(vi00)
                lq              worldToScreen[2], 2(vi00)
                lq              worldToScreen[3], 3(vi00)

                lq              gifTag, 5(vi00)
                sqi             gifTag, (destAddress++)

				lq				clipScaleAdd, 4(vi00)
                
                lq              color, 6(vi00)

                iaddiu          vertexCounter, vi00, 3 // vertexCount
vertexLoop:
                lqi             vertex, (vertexData++)

				mul             acc, worldToScreen[0], vertex[x]
				madd            acc, worldToScreen[1], vertex[y]
				madd            acc, worldToScreen[2], vertex[z]
				madd            vertexOut, worldToScreen[3], vertex[w]

                div             q, vf00[w], vertexOut[w]
                mul.xyz         vertexOut, vertexOut, q
									
				mula			acc, clipScaleAdd, vf00[w]
				madd			vertexOut, vertexOut, clipScaleAdd

                ftoi4           vertexOut, vertexOut

                sqi             color, (destAddress++)
                sqi             vertexOut, (destAddress++)

                iaddi           vertexCounter, vertexCounter, -1
                ibne            vertexCounter, vi00, vertexLoop
                
                xgkick          kickAddress
				--cont

Vu1PrimitiveRendererEnd:
                --exit
                --endexit
;.END
