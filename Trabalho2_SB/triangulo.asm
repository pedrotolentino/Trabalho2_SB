SECTION TEXT
		c_input		B
		;input		H
		load		B
		;mult		H
		;div		DOIS
		store		R
		c_output	R
		stop

SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
