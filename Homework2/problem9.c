
//Jeremy Kritz
//This code won;t compile, but i dont think its meant to


sem_t Tchrs; // gonna hold the number of teachers
sem_t Kids; // holds number of children
//sem_t childMutex; // to lock ratio checks, so no one leaves or arrives while they happen
// it'll be binary, bc of mutexes to for teacher/kid counts not sure i need this
float ratio;
float curRatio; // current ratio
int big = 99999999; // arbitrarily big number to be larger than ratio

void teacher_enter()
{
    sem_post(&Tchrs); // increment the number of teachers, if a teacher is waititng to leave, they'll go
}

void teacher_exit()
{
    int numTchrs; // current number of teachers
    int numKids; // current number of kids
    sem_getvalue(&Tchrs, &numTchrs);
    sem_getvalue(&Kids, &numKids);
    float ratioIfGone;
    if ((numTchrs - 1) != 0)
    {
        ratioIfGone = numKids / (numTchrs - 1); // if this teacher leaves is it ok
    }
    else
    {
        if (numKids == 0)
        {
            ratioIfGone = 0; // if no kids, ratio doesnt matter, teacher can leave
        }
        else
        {
            ratioIfGone = big; // if no teachers but kids, cant leave
        }
    }

    if (ratioIfGone <= ratio) // if ratio still be ok, the teacher can leave
    {
        sem_wait(&Tchrs); // decrement num teachers
    }

    else // cant leave
    {
       // printf("Teacher %u waiting to leave...\n", (int)pthread_self());
        teacher_exit(); // teacher attempts to leave again
    }
}
void verify_compliance() // this doesnt do anything, but it'll print
{


    int numTchrs; // number of teachers
    int numKids; // number of kids

    sem_getvalue(&Kids, &numKids); // the semaphore holds the current number
    sem_getvalue(&Tchrs, &numTchrs);
    if (numTchrs != 0)
    {
        curRatio = numKids / numTchrs;
    }

    else
    {
        if (numKids == 0)
        {
            curRatio = 0;
        }

        else
        {
            curRatio = big;
        }

    }


    if (curRatio <= ratio)
    { // if the compliance is verified
        printf("Parent %u has verified compliance.\n", (int)pthread_self());
        // printing bc not specified whhat func does
        // it could also be a bool i guess
    }

    else
    {
        printf("Parent %u found non-compliance.\n", (int)pthread_self());
    }

}

void child_enter() // no barriers here
{
    sem_post(&Kids); // increment kids
}

void child_exit() // no barriers
{
    sem_wait(&Kids); // decrement kids
}

void parent_enter() //  nothing happens, we dont rely on parents
{
   // printf("Parent %u entered.\n", (int)pthread_self()); // maybe print bc otherwise it does nothing
}

void parent_exit() // no issue
{
    //printf("Parent %u left.\n", (int)pthread_self()); // maybe print bc otherwise it does nothing
}




void Teacher()
{
    for (;;)
    {
        teacher_enter();
        // ... critical section ... //
        teach(); // not written
        teacher_exit(); 
        go_home(); // not specified
    }
}

void Child()
{
    for (;;)
    {
        child_enter();
        // ... critical section ... //
        learn(); // this does nothing, we weren't told to write this or teach
        child_exit(); 
        go_home();
    }
}

void Parent()
{
    for (;;)
    {
        parent_enter();
        // ... critical section ... //
        verify_compliance();
        parent_exit();
        go_home();

    }
}
