//Mosgöller Bernhard 4 Mai
//Programm könnte verbessert werden Ifs in die If's schreiben damit nicht jedes mal bei dem if abgefragt wird wenn das File schon zu ende ist   11Mai


//system call
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define ANALOG_CHANNEL_1 0
#define ANALOG_CHANNEL_2 1
#define ANALOG_CHANNEL_3 2
#define ANALOG_CHANNEL_4 3
#define ANALOG_CHANNEL_5 4
#define ANALOG_CHANNEL_6 5
#define ANALOG_CHANNEL_7 6
#define ANALOG_CHANNEL_8 7

#define DIGI_CHANNEL_1 8
#define DIGI_CHANNEL_2 9
#define DIGI_CHANNEL_3 10
#define DIGI_CHANNEL_4 11
#define DIGI_CHANNEL_5 12
#define DIGI_CHANNEL_6 13
#define DIGI_CHANNEL_7 14
#define DIGI_CHANNEL_8 15

#define NOT_SET 1888


void ausgabe(char* text);
void char2short( char* pchar,  short* pshort);
int channel_anz(char* channels);
int next_channel(char* channels, int active_channel);
int next_analog_channel(char* channels, int active_channel);

void ausgabe(char* text)
{
    printf("**********%s**********\n\n",text);

}

void char2short( char* pchar,  short* pshort)
{
  *pshort = (pchar[0] << 8) | pchar[1];
}

int channel_anz(char* channels) //überprüft und funktioniert
{
    int anz=0,i;

    for(i=0;i<16;i++)
    {
        if(channels[i] == '1')
        {
            anz++;
        }

    }
    return anz;
}

int next_channel(char* channels, int active_channel)    //funktioniert 21.6
{
    int next_ch = NOT_SET,i=0;
    //k wird auf 0 gestzt wenn wieder bei channel 1 angefangen werden muss


    if(channel_anz(channels)<=1)
    {
        //next_channel muss gleich bleiben, weil es nur 1 oder weniger active channels gibt
        //keine Änderung
        next_ch=active_channel;
    }
    else
    {
        if(active_channel>=15)
        {
            if(channels[ANALOG_CHANNEL_1]== '1')
            {
                next_ch = ANALOG_CHANNEL_1;
            }
            else    //es gibst mindestens 2 aktive Channels und weil wir beim ersten anfangen muss man sich keine Gedanken über die zurücksetzung machen
            {
                while(channels[i]!= '1')
                {
                    i++;
                }
                next_ch = i;
            }
        }
        else
        {
            i= active_channel+1;  //startet beim aktiven Channel+1, muss durch die vorhergehende Abfrage nicht zurückgesetzt werden im falle dass
            while(next_ch == NOT_SET)
            {
                if(channels[i]== '1')
                {
                        next_ch=i;
                }
                else
                {
                    i++;
                    if(i>=16) //Es gibt 16 Channels-> abfrage von vorn wieder
                    {
                        i=0;
                    }
                }
            }
        }
    }

    return next_ch;
}


int next_analog_channel(char* channels, int active_channel)    //funktioniert 21.6
{
    int next_ch = NOT_SET,i=0;


    if(channel_anz(channels)<=1)
    {
        //next_channel muss gleich bleiben, weil es nur 1 oder weniger active channels gibt
        //keine Änderung
        next_ch=active_channel;
    }
    else
    {
        if(active_channel>=7)
        {
            if(channels[ANALOG_CHANNEL_1]== '1')
            {
                next_ch = ANALOG_CHANNEL_1;
            }
            else    //es gibst mindestens 2 aktive Channels und weil wir beim ersten anfangen muss man sich keine Gedanken über die zurücksetzung machen
            {
                while(channels[i]!= '1')
                {
                    i++;
                }
                if(i<=8)
                {
                    next_ch = active_channel;
                }
                else
                {
                    next_ch = i;
                }
            }
        }
        else
        {
            i= active_channel+1;  //startet beim aktiven Channel+1, muss durch die vorhergehende Abfrage nicht zurückgesetzt werden im falle dass
            while(next_ch == NOT_SET)
            {
                if(channels[i]== '1')
                {
                        next_ch=i;
                }
                else
                {
                    i++;
                    if(i>=8) //Es gibt 16 Channels-> abfrage von vorn wieder
                    {
                        i=0;
                    }
                }
            }
        }
    }

    return next_ch;
}


int main()
{
    FILE* d,*ch[15];
    char path[300];
/*  char path1_digi[300],path2_digi[300],path3_digi[300],path4_digi[300],path5_digi[300],path6_digi[300],path7_digi[300],path8_digi[300];
    char path1_analog[300],path2_analog[300],path3_analog[300],path4_analog[300],path5_analog[300],path6_analog[300],path7_analog[300],path8_analog[300];   */
    char path_channel[16][300]={"Analog_Channel1.txt","Analog_Channel2.txt","Analog_Channel3.txt","Analog_Channel4.txt","Analog_Channel5.txt","Analog_Channel6.txt","Analog_Channel7.txt","Analog_Channel8.txt","Digi_Channel1.txt","Digi_Channel2.txt","Digi_Channel3.txt","Digi_Channel4.txt","Digi_Channel5.txt","Digi_Channel6.txt","Digi_Channel7.txt","Digi_Channel8.txt"};
    //char path_digi[8][300]={"Digi_Channel1","Digi_Channel2","Digi_Channel3","Digi_Channel4","Digi_Channel5","Digi_Channel6","Digi_Channel7","Digi_Channel8"};

    //C:\Users\Bernhard\Schule\!DA\Programme\Vorarbeit\Channel_coding\Data_test\Data_test\Testdaten_Widerstand_4Mai.bin

    char buffer[40],buffer2[40],channels[24];
    int i,j,k,active_channel=1;
    double a_HP[9],a_LP[9],a_Sens[9],a_Sampl[9];
    double y_ko=1/256,x_ko;
    char polar[9];
    short value;

    strcpy(path,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\receivedData.bin");

   /* strcpy(path_digi[0],"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel1.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel2.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel3.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel4.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel5.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel6.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel7.txt");
    strcpy(path_digi,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Digi_Channel8.txt");

    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel1.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel2.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel3.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel4.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel5.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel6.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel7.txt");
    strcpy(path_analog,"C:\\Users\\Bernhard\\Schule\\!DA\\Programme\\Vorarbeit\\Channel_coding\\Data_test\\Data_test\\Analog_Channel8.txt");    */

    //Files neu erstellen
    for(i=0;i<16;i++)
    {
        ch[i]=fopen(path_channel[i],"w");
        fclose(ch[i]);
    }



    d=fopen(path,"r");
    if(d != NULL)
    {
        ausgabe("File wurde geoeffnet");

        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Hersteller:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende1");
        }



        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Produkt:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende2");
        }





        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Version:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende3");
        }




        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Abtastfrequenz:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende4");
        }




        //In Channels wird gespeichert ob channel benutzt wird oder nicht
        //wird nachher in einem unterprogramm modifiziert

        if(fgets(buffer,40,d)!=NULL)
        {
            //strcpy(buffer2,buffer);
            printf("ID Analog Channel(8-1):");
            for(i=0;i<8;i++)
            {
                printf("%c",buffer[i]);
                channels[i]=buffer[i];
            }
            printf("\n");

            printf("Digitl ID's(8-1):");
            for(i=8;i<16;i++)
            {
                printf("%c",buffer[i]);
                channels[i]=buffer[i];
            }
            printf("\n");

            printf("Direction of the digital channels(1 for Input 0 for Output): \n");
            for(i=16;i<24;i++)
            {
                printf("%c",buffer[i]);
                channels[i]=buffer[i];
            }
            printf("\n");

        }
        else
        {
            ausgabe("File zu Ende5");
        }

        /* Testabfrage
        printf("\n*******%d********\n\n",channel_anz(channels));//funktioniert
        printf("******%d***********\n\n",next_channel(channels, 4));    //funktioniert
        */


        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Displayed Channels:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende");
        }


        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Displayed Time(in Sekunden):%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende");
        }



        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Hardware version:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende5");
        }

        if(fgets(buffer,40,d)!=NULL)
        {
            printf("Seriennummer:%s",buffer);
        }
        else
        {
            ausgabe("File zu Ende5");
        }

//    double a_HP[9],a_LP[9],a_Sens[9],a_Sampl[9];
 //   char polar[9];

        for(i=ANALOG_CHANNEL_1;i<=ANALOG_CHANNEL_8;i++)    //8 Analoge Channel Konfig
        {
            if(fgets(buffer,40,d)!=NULL)
            {
                sscanf(buffer,"%lf/%lf/%lf/%lf/%c",&a_HP[i],&a_LP[i],&a_Sens[i],&a_Sampl[i],&polar[i]);
                //printf("%lf/%lf/%lf/%lf/%c\n\n",a_HP[i],a_LP[i],a_Sens[i],a_Sampl[i],polar[i]);//TEST
                printf("Analog Channel %d:  %s" ,i,buffer);
            }
            else
            {
                ausgabe("File zu Ende5");
            }

        }


        if(fgets(buffer,40,d)!=NULL)
        {

            if(strcmp(buffer,"EOH\n") == 0)
            {
                printf("END OF HEADER\n");
            }
        }
        else
        {
            ausgabe("File zu Ende5");
        }

        k=0;
        j=next_channel(channels,0);
        i=next_channel(channels,0);//in i steht der Start Channel
        while(fgets(buffer,3,d)!=NULL)//Nimmt immer 2 Zeichen weil char nur 1 Byte groß ist und shor 2 Byte groß ist
        {
            if(i>ANALOG_CHANNEL_8)
            {   //DIGI Channel
                char2short(buffer, &value);
                x_ko=value;   //weiß nicht genau was die digitalen Channels ausgeben

            }
            else
            {   //ANALOger Channel
                char2short(buffer, &value);
                x_ko=value*(5/131072)*a_Sens[i];   //statt 80 sollte die Sensity kommen
            }
            k++;
            printf("%.2f\n\t\t%d\n",x_ko,k/channel_anz(channels));

            ch[j]=fopen(path_channel[j],"a");   //Aus irgendeinen unbekannten Grund wird die Variable verändert
            fprintf(ch[j], "%f\n", x_ko);       //deswegen eigene Variable dafür und nicht i !!!!!!!
            //fclose(ch[j]);


            i= next_channel(channels,i);
            j=i;


        }

        ausgabe("File zu Ende");

        fclose(d);

        i=next_channel(channels,0);
        j=i;

        do
        {
            i=next_channel(channels,i);
            fclose(ch[i]);
        }while(j!=i);

    }
    else
    {
        ausgabe("Error beim File öffnen");
    }

    return 0;
}
