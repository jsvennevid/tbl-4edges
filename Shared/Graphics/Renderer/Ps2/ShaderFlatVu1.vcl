//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
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




