void f1_jwt()
{
    int i, j, k;
    for(i = 0; i < 50; i++)
    {
	putchar('a');

	for(j = 0; j < 500; j++)
	    for(k = 0; k < 500; k++);
    }
}


void f2_jwt()
{
    long i, j, k;
    for(i = 0; i < 30; i++)
    {
	putchar('b');

	for(j = 0; j < 500; j++)
	    for(k = 0; k < 500; k++);
    }
}

int jwt_show_time()
{
    InitDos();
    initTCB();
    old_int8 = getvect(8);

    strcpy(tcb[0].name, "main");
    tcb[0].state = RUNNING;
    current = 0;

		printf("\n The realization of thread concurrent execution, can set the time sheet size.\n");
		printf(" Thread F1 continuous output of the letters a, a total of 50 times\n");
		printf(" Thread F2 continuous output of the letters b, a total of 30 times\n");
		printf(" Please set the time slice size:");
		scanf("%d", &TL);
		printf(" Press any key to continue. . . ");
		getch();
		create("f1", (codeptr)f1_jwt, 1024);
		create("f2", (codeptr)f2_jwt, 1024);
		tcb_state();
		setvect(8, new_int8);
		my_swtch();
		while(!finished());


	tcb_state();
	setvect(8, old_int8);

    tcb[0].state=FINISHED;
    tcb[0].name[0]='\0';
    printf("\n\n  Thank you for using multi task system\n");

    return 0;

}