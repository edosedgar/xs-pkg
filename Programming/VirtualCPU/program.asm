	NOP
	NOP	
Main1:	
	CALL Killer
	JP Main1
	END

Killer:
	OUT R23
	CALL Killer1
	INC R23
	RET

Killer1:
	MOV R30, 3000
	OUT R30
	RET
