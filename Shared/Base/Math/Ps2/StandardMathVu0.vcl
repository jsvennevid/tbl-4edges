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

				.name StandardMathVu0
				.global MatirxMultiply1
                .syntax new
                .vu
                .init_vf_all
                .init_vi_all

.macro matrixMultiply   matResult, matLeft, matRight
				mul            acc,           \matright[0], \matleft[0][x]
				madd           acc,           \matright[1], \matleft[0][y]
				madd           acc,           \matright[2], \matleft[0][z]
				madd           \matresult[0], \matright[3], \matleft[0][w]
				
				mul            acc,           \matright[0], \matleft[1][x]
				madd           acc,           \matright[1], \matleft[1][y]
				madd           acc,           \matright[2], \matleft[1][z]
				madd           \matresult[1], \matright[3], \matleft[1][w]
				
				mul            acc,           \matright[0], \matleft[2][x]
				madd           acc,           \matright[1], \matleft[2][y]
				madd           acc,           \matright[2], \matleft[2][z]
				madd           \matresult[2], \matright[3], \matleft[2][w]
				
				mul            acc,           \matright[0], \matleft[3][x]
				madd           acc,           \matright[1], \matleft[3][y]
				madd           acc,           \matright[2], \matleft[3][z]
				madd           \matresult[3], \matright[3], \matleft[3][w]
.endm

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				
MatrixMultiply1:

                --enter
				in_vf			matright[0] (vf01)
				in_vf			matright[1] (vf02)
				in_vf			matright[2] (vf03)
				in_vf			matright[3] (vf04)

				in_vf			matleft[0] (vf05)
				in_vf			matleft[1] (vf06)
				in_vf			matleft[2] (vf07)
				in_vf			matleft[3] (vf08)
                --endenter

				mul            acc,          matright[0], matleft[0][x]
				madd           acc,          matright[1], matleft[0][y]
				madd           acc,          matright[2], matleft[0][z]
				madd           matresult[0], matright[3], matleft[0][w]
				
				mul            acc,          matright[0], matleft[1][x]
				madd           acc,          matright[1], matleft[1][y]
				madd           acc,          matright[2], matleft[1][z]
				madd           matresult[1], matright[3], matleft[1][w]
				
				mul            acc,          matright[0], matleft[2][x]
				madd           acc,          matright[1], matleft[2][y]
				madd           acc,          matright[2], matleft[2][z]
				madd           matresult[2], matright[3], matleft[2][w]
				
				mul            acc,          matright[0], matleft[3][x]
				madd           acc,          matright[1], matleft[3][y]
				madd	       acc,          matright[2], matleft[3][z]
				madd           matresult[3], matright[3], matleft[3][w]
				--cont

				--exit
				out_vf		   matresult[0] (vf09)
				out_vf		   matresult[1] (vf10)
				out_vf		   matresult[2] (vf11)
				out_vf		   matresult[3] (vf12)
                --endexit


