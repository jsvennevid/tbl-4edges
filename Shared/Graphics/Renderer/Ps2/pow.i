;(c) Ian Stephenson
;
;ian@dctsystems.freeserve.co.uk
;
; Fast pow() VU implementation

	--enter
	--endenter

	.macro FRAC	result,val
	loi	10000
	add	ftmp,\val,I
	ftoi0	itmp,ftmp
	itof0	itmp,itmp
	sub	\result,ftmp,itmp
	.endm

	.macro LOG2 result,val
	itof0	\val,\val
	loi	1.0/(1<<23)
	mul	\val,\val,I
	loi	-127
	add	\result,\val,I

	FRAC	y,\result
	sub	t1,y,vf0[w]
	loi	-0.346607
	mul	t2,y,I
	mul	correction,t1,t2
	add	\result,\result,correction
	.endm

	.macro POW2 result,val
	FRAC	y,\val
	sub	t1,y,vf0[w]
	loi	-0.33971
	mul	t2,y,I
	mul	correction,t1,t2
	sub	\val,\val,correction

	loi	127
	add	\val,\val,I
	loi	(1<<23)
	mul	\val,\val,I
	ftoi0	\result,\val
	.endm

	.macro FAST_LOG2 result,val
	itof0	\val,\val
	loi	1.0/(1<<23)
	mul	\val,\val,I
	loi	-127
	add	\result,\val,I
	.endm

	.macro FAST_POW2 result,val
	loi	127
	add	\val,\val,I
	loi	(1<<23)
	mul	\val,\val,I
	ftoi0	\result,\val
	.endm

	.macro FAST_POW result,a,b
	FAST_LOG2	tmp,\a
	mul	tmp,tmp,\b
	FAST_POW2	\result, tmp
	.endm

	.macro POW result,a,b
	LOG2	tmp,\a
	mul	tmp,tmp,\b
	POW2	\result, tmp
	.endm
