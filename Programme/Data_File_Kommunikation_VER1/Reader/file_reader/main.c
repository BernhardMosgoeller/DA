#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUSY 48 //ASCII für 0
#define FREE 49 //ASCII für 1


int main()
{
    FILE *datafile,*waitfile;
    char datapath[100],waitpath[100];
    char free;
    char data_from_file[1000];

    strcpy(datapath,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Data_File_Kommunikation\\datafile.mos");
    strcpy(waitpath,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Data_File_Kommunikation\\waitfile.mos");

    waitfile=fopen(waitpath,"r");
    if(waitfile!=NULL)
    {
        printf("SUCCESSFUL OPENED WAITFILE\n");
        fscanf(waitfile,"%c",&free);

        if(free==BUSY)
        {
            printf("FILE IS READY TO READ\n");
            datafile=fopen(datapath,"r");
            if(datafile!=NULL)
            {
                fgets(data_from_file,1000,datafile);
                fclose(datafile);
                fclose(waitfile);
            }
            else
            {
                printf("FAILED TO OPEN DATAFILE r\n");
            }

            waitfile=fopen(waitpath,"w");
            if(waitfile!=NULL)
            {
                printf("SUCESSFUL OPENED WAITFILE w\n");
                fprintf(waitfile,"1");
                fclose(waitfile);


            }
            else
            {
                printf("ERROR FAIELD TOP OPEN WAITFILE w\n");
            }

        }
        else
        {
            printf("NO NEW DATA");
        }

    }
    else
    {
        printf("ERROR AT OPENING WAITFILE r\n");
    }

    puts(data_from_file);
    return 0;

}
