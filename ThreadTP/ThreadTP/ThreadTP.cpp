#include <iostream>
#include <windows.h>

// Fonction que le Thread execute
DWORD WINAPI Static_ThreadProc(void* pParam)
{
    // Code du Thread
    std::cout << "Le thread est lancer!" << std::endl;

    int* startValue = (int*)pParam;
    int test = *startValue;

    while (test >= 0)
    {
        std::cout << "Compteur:" << test << std::endl;
        test--;
    }

    return 0;
}

int main()
{
    // Tableau de ThreadId static
    const int numberOfThreads = 3;
    HANDLE ThreadHandles[numberOfThreads];
    DWORD threadId[numberOfThreads];


    // Creation de Threads 
    // (Attribut de securite, taille de pile/memoire alloue(0 taille par defaut), parametre de la fonction qui est executer avec le thread, indicateur de creation,id du thread une fois creer) 
    for (int i = 0; i < 3; i++) 
    {
        int valueCompteur = 5;
        ThreadHandles[i] = CreateThread(nullptr, 0, Static_ThreadProc, &valueCompteur, 0, &threadId[i]);
        
        if (ThreadHandles[i] == nullptr)
        {
            std::cerr << "Erreur pendant la creation du thread!" << std::endl;
            return 1;
        }

    }

    // Verifie si les threads on finis de sexecuter
    WaitForMultipleObjects(numberOfThreads,ThreadHandles, true, INFINITE);

    // Ferme les Threads apres execution de celles-ci
    for (int i = 0; i < numberOfThreads; i++) 
    {
        CloseHandle(ThreadHandles[i]);
    }

    return 0;
}