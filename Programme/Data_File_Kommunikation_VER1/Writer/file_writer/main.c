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
    char data_to_file[1000];

    strcpy(data_to_file,"TEST1");

    strcpy(datapath,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Data_File_Kommunikation\\datafile.mos");
    strcpy(waitpath,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Data_File_Kommunikation\\waitfile.mos");

    waitfile=fopen(waitpath,"r");
    if(waitfile!=NULL)
    {
        //printf("Waitfile successful opened");

        fscanf(waitfile,"%c",&free);

        if(free == FREE)
        {
            printf("LINE IS FREE\n");
            datafile=fopen(datapath,"w");
            if(datafile!=NULL)
            {
                printf("DATAFILE SUCCESSFUL OPENED\n");
                fputs(data_to_file,datafile);
                fclose(datafile);
                fclose(waitfile);

                waitfile=fopen(waitpath,"w");
                if(waitfile!=NULL)
                {
                    fprintf(waitfile,"0");
                    fclose(waitfile);
                }
                else
                {
                    printf("ERROR AT OPENING WAITFILE w\n");
                }

            }
            else
            {
                printf("ERROR OPEN DATAFILE");
            }
        }
        else
        {
            printf("LINE BUSY");
        }

    }
    else
    {
        printf("ERROR OPEN WAITFILE");
    }

    return 0;
}
