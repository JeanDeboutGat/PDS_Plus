
TOUS :=  pipe  pipes


tout: ${TOUS}


pipes : pipes.c
	gcc pipes.c -o pipes



pipe : pipe.c
	gcc pipe.c -o pipe

clean:

	rm -f core *.o



realclean: clean

	rm -f ${TOUS}



.PHONY: all clean realclean

