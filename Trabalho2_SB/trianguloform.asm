SECTION TEXT
		input	B
		input	H
		load	B
		mult	H
		div		DOIS
		store	R
		output	R
		stop

SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
