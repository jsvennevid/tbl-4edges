///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
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
