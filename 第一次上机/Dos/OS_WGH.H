void f1_wgh()
{
    int i, j;
    printf("Hello ,This is f1()\n");

	for(i = 0; i < 500; i++)
	    for(j = 0; j < 500; j++);

}

void f2_wgh()
{
    int i, j;
    printf("Hello ,This is f2()\n");

	for(i = 0; i < 500; i++)
	    for(j = 0; j < 500; j++);

}

void f3_wgh()
{
    int i, j;
    printf("Hello ,This is f4()\n");

	for(i = 0; i < 500; i++)
	    for(j = 0; j < 500; j++);

}

void f4_wgh()
{
    int i, j;
    printf("Hello ,This is f4()\n");

	for(i = 0; i < 500; i++)
	    for(j = 0; j < 500; j++);

}
int wgh_show_time()
{
    printf("welcome to FCFS,we will create 4 threads and swtch them ,let's see what will happen:\n");
	initTCB();
	strcpy(tcb[0].name, "main");
	tcb[0].state = RUNNING;
	current = 0;
	create("f1", (codeptr)f1_wgh, 1024);
	create("f2", (codeptr)f2_wgh, 1024);
	create("f3", (codeptr)f3_wgh, 1024);
	create("f4", (codeptr)f4_wgh, 1024);
	getch();
	my_swtch();
	while(!finished());
	tcb[0].state=FINISHED;
    tcb[0].name[0]='\0';
	printf("\n\n  Thank you for using FCFS multi task system\n");
    return 0;
}