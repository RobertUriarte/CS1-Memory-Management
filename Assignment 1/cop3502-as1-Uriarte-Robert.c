#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sp21_cop3502_as1.h"
#include "leak_detector_c.h"

//Credit Professor Gerber
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';
        t--;
    }
}

// Credit Professor Gerber
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

//Creates a new monster structure and fills the elements of the structure
monster *new_monster(int id, char *names, char *elements, int population)
{
    monster *p = malloc(sizeof(monster));

    (*p).id = id;
    (*p).name = strdup(names);
    (*p).element = strdup(elements);
    (*p).population = population;

    return p;
}


//Creates a double pointer array to filled with monster structures.
monster **new_monster_dparray(FILE *ifp, int total_monsters, char* buf)
{
    char namebuf[256];

    monster **array = calloc(total_monsters, sizeof(monster*));

    for(int i = 0; i < total_monsters; i++)
    {
        sprintf(namebuf, "Monster #%d", i+1);

        int id = i;

        get_next_nonblank_line(ifp,buf,255);
        char *names;
        names = malloc(sizeof(char)*256);
        strcpy(names,(strtok(buf," ")));    // Gets monster name from input file

        char* elements;
        elements = malloc(sizeof(char)*256);
        strcpy(elements ,(strtok(NULL," ")) );  //Gets element from input file


        int population;
        population = atoi(strtok(NULL," "));    //Gets Population from input file

        monster *p = new_monster(id, names, elements, population);
        array[i] = p;

        free(names);
        free(elements);
    }
    return array;
}

//Prints the elements of a monster structure
void print_monster(monster *p){
        printf("\nMonster id: %d", p->id);
        printf("\nName: %s", p->name);
        printf("\nElement: %s", p->element);
        printf("\nPopulation: %d\n", p->population);
}

//Prints The all the structures in the monster double pointer array
void print_monster_dparray(monster **array, int total_monsters)
{
    for(int i = 0; i < total_monsters; i++)
    {
        if(array[i] != NULL) {
            print_monster(array[i]);
        }
    }
}

//Credit Professor Gerber for function idea. Frees pointer elements in the monster structure
void dispose_monster(monster *p)
{
    free(p->name);
    free(p->element);
    free(p);
}

//Credit Professor Gerber for function idea. Frees the monster structures and the double pointer array.
void dispose_all_monster_data(monster **array, int total_monsters)
{
    for(int i = 0; i < total_monsters; i++)
    {
        if(array[i] != NULL) {
            dispose_monster(array[i]);
        }
    }
    free(array);
}

//Creates a new region structure and fills its elements
region *new_region(char* namebuffer, int nmonsters, int total_population, monster** monsters)
{
    region *ptr = malloc(sizeof(region));

    ptr->name = strdup(namebuffer);
    ptr->nmonsters = nmonsters ;
    ptr->total_population = total_population;
    ptr->monsters = monsters;

    return ptr;
}

//Creates a region double pointer array filled with region structures
region **new_region_dparray(FILE *ifp, char* buf, int total_regions, monster **monsters_index, int total_monsters) {

    char namebuf[256];

    region **rarray = calloc(total_regions, sizeof(region *));

    for(int i = 0; i < total_regions; i++){

        sprintf(namebuf, "Region: #%d", i+1);

        get_next_nonblank_line(ifp,buf,255);
        printf("\nName of region: %s\n",buf);
        char* namebuffer;
        namebuffer = malloc(sizeof(char)*256);
        strcpy(namebuffer,buf); //Gets region name

        get_next_nonblank_line(ifp,buf,255);
        int nmonsters;
        nmonsters = atoi(strtok(buf," "));  //Gets the number of monster in specific region
        printf("The number of monsters in the region of %s is %d", namebuffer, nmonsters);

        monster** monsters = calloc(nmonsters, sizeof(monster*));

        int total_population = 0;
        for(int j = 0; j < nmonsters; j++){
            get_next_nonblank_line(ifp,buf,255);
            for (int i = 0; i < total_monsters; i++) {
                if (strcmp(buf,monsters_index[i]->name)== 0) {  //Tests to see which monsters in this region
                    total_population = total_population + monsters_index[i]->population;    //Adds total population of each monster in that region
                    monsters[j] = monsters_index[i];
                }
            }
        }

        printf("\nTotal Pop: %d\n" , total_population);

        region *ptr = new_region(namebuffer,nmonsters,total_population,monsters);
        rarray[i] = ptr;

        free(namebuffer);

    }
    return rarray;
}

//Credit Professor Gerber for function idea. Frees region elements in the region structure
void dispose_region(region *ptr)
{
    free(ptr->name);
    free(ptr->monsters);
    free(ptr);
}

//Credit Professor Gerber for function idea. Frees the region structures and the double pointer array.
void dispose_all_region_data(region **rarray, int total_regions)
{
    for(int i = 0; i < total_regions; i++)
    {
        if(rarray[i] != NULL) {
            dispose_region(rarray[i]);
        }
    }
    free(rarray);
}

//Creates a new itinerary structure and fills its elements
itinerary *new_itinerary(int nregions, region** regions, int captures)
{
    itinerary *ptr2 = malloc(sizeof(itinerary));

    ptr2->nregions = nregions;
    ptr2->regions = regions;
    ptr2->captures = captures ;

    return ptr2;
}

//Creates a double pointer array filled with itinerary structures
itinerary **new_itinerary_dparray(FILE *ifp, char*buf, int total_trainers, int total_regions, region** regions_index, char** trainer_names)
{
    char namebuf[256];

    itinerary **itarray = calloc(total_trainers, sizeof(itinerary *));

    for(int i = 0; i < total_trainers; i++) {

        sprintf(namebuf, "Itinerary #%d", i+1);

        get_next_nonblank_line(ifp,buf,255);
        strcpy(trainer_names[i],buf); //Gets the trainer name to be used later in trainer_index
        printf("\n\nTrainer Name: %s", trainer_names[i]);

        get_next_nonblank_line(ifp,buf,255);
        int captures;
        captures = atoi(strtok(buf," "));   //Gets number of captures for this trainer
        printf("\nNumber of captures: %d", captures);

        get_next_nonblank_line(ifp,buf,255);
        int nregions;
        nregions = atoi(strtok(buf," "));   //Gets number of regions for this trainers itinerary
        printf("\nNumber of regions: %d",nregions);

        region **regions = calloc(nregions, sizeof(region*));

        for(int j = 0; j < nregions; j++){
            get_next_nonblank_line(ifp,buf,255);
            for (int i = 0; i < total_regions; i++) {
                if (strcmp(buf,regions_index[i]->name)== 0) {   //Tests to see which regions are in this trainers itinerary utilized strcmp.
                    regions[j] = regions_index[i];
                    printf("\nThe region of: %s", regions_index[i]->name);
                }
            }
        }

        itinerary *ptr2 = new_itinerary(nregions, regions, captures);
        itarray[i] = ptr2;
    }

    return itarray;
}

//Credit Professor Gerber for function idea. Frees itinerary elements in the itinerary structure
void dispose_itinerary(itinerary *ptr2)
{
    free(ptr2->regions);
    free(ptr2);
}

//Credit Professor Gerber for function idea. Frees the itinerary structures and the double pointer array.
void dispose_all_itinerary_data(itinerary **itarray, int total_trainers)
{
    for(int i = 0; i < total_trainers; i++)
    {
        if(itarray[i] != NULL) {
            dispose_itinerary(itarray[i]);
        }
    }
    free(itarray);
}

//Creates a new trainer structure and fills its elements
trainer *new_trainer(char* namebuffer2, itinerary* visits)
{
    trainer *ptr3 = malloc(sizeof(trainer));

    ptr3->name = strdup(namebuffer2);
    ptr3->visits = visits;

    return ptr3;
}

//Creates a double pointer array and fills it with trainer structures.
trainer **new_trainer_dparray(FILE *ifp, char*buf, int total_trainers, itinerary** itinerary_index, char** trainer_names)
{
    char namebuf[256];

    trainer **tarray = calloc(total_trainers, sizeof(trainer *));

    for(int i = 0; i < total_trainers; i++)
    {
        sprintf(namebuf, "Trainer #%d", i+1);

        char* namebuffer2;
        namebuffer2 = malloc(sizeof(char)*25);
        strcpy(namebuffer2,trainer_names[i]);   //Gets the trainer's name

        itinerary *visits = calloc(1, sizeof(itinerary));
        *visits = *itinerary_index[i];

        trainer *ptr3 = new_trainer(namebuffer2, visits);
        tarray[i] = ptr3;

        free(namebuffer2);
    }
    return tarray;
}

//Credit Professor Gerber for function idea. Frees trainer elements in the trainer structure
void dispose_trainer(trainer *ptr3)
{
    free(ptr3->name);
    free(ptr3->visits);
    free(ptr3);
}

//Credit Professor Gerber for function idea. Frees the trainer structures and the double pointer array.
void dispose_all_trainer_data(trainer **tarray, int total_trainers)
{
    for(int i = 0; i < total_trainers; i++)
    {
        if(tarray[i] != NULL) {
            dispose_trainer(tarray[i]);
        }
    }
    free(tarray);
}

//Gets the actual captures of a trainer and file prints all the data
void print_output(FILE* ofp, int total_trainers, trainer** trainers_index) {
    for (int i=0; i < total_trainers; i++) {
        fprintf(ofp,"%s\n",trainers_index[i]->name);

        for (int j=0; j < trainers_index[i]->visits->nregions; j++) {
           fprintf(ofp,"%s\n",trainers_index[i]->visits->regions[j]->name);

           for(int k=0; k < trainers_index[i]->visits->regions[j]->nmonsters; k++) {

                //Converts int values to double so actual values can be calculated.
                double monster_pop = (trainers_index[i]->visits->regions[j]->monsters[k]->population);
                double region_pop = (trainers_index[i]->visits->regions[j]->total_population);
                double intended_captures = (trainers_index[i]->visits->captures);
                double actual_captures = round ((monster_pop/region_pop)*(intended_captures));
                if (actual_captures != 0) {
                    fprintf(ofp,"%.0lf %s\n", actual_captures, trainers_index[i]->visits->regions[j]->monsters[k]->name);
                }
           }
        }
        fprintf(ofp,"\n");
    }
}

//Credit Professor Gerber for function idea. Frees the trainer name double pointer array.
void dispose_trainer_names(char **trainer_names, int total_trainers)
{
    for(int i = 0; i < total_trainers; i++)
    {
        if(trainer_names[i] != NULL) {
            free(trainer_names[i]);
        }
    }
    free(trainer_names);
}

int main()
{
    atexit(report_mem_leak);

    char buf[256];

    int total_monsters, total_regions, total_trainers;

    //Opens output file and checks if it successfully opened
    FILE *ofp;
    ofp = fopen("cop3502-as1-output-Uriarte-Robert.txt","w");
    if(ofp != NULL) {
        printf("Output file successfully opened\n");
    } else {
        printf("Error output file not opened");
        return 0;
    }

    //Opens input file and checks if it successfully opened
    FILE *ifp;
    ifp = fopen("cop3502-as1-input.txt","r");
    if(ifp != NULL) {
        printf("Input file successfully opened\n");
    } else {
        printf("Error, input file not opened\n");
        return 0;
    }

    //Gets the total numbers of monsters from input file
    (get_next_nonblank_line(ifp,buf,255));
    total_monsters = atoi(strtok(buf," "));
    printf("\nTotal Monsters: %d\n", total_monsters);

    //Declares, initializes, and outputs monster data
    monster **monsters_index;
    monsters_index = new_monster_dparray(ifp,total_monsters,buf);
    print_monster_dparray(monsters_index, total_monsters);

    //Declares, initializes, and outputs region data
    (get_next_nonblank_line(ifp,buf,255));
    total_regions = atoi(strtok(buf," "));
    printf("\ntotal Regions: %d\n", total_regions);
    region **regions_index;
    regions_index = new_region_dparray(ifp, buf, total_regions, monsters_index, total_monsters);

    //Declares, initializes, and outputs itinerary data
    (get_next_nonblank_line(ifp,buf,255));
    total_trainers = atoi(strtok(buf," "));
    char **trainer_names = malloc(total_trainers * sizeof(char*)); //Used to hold the trainer names
    for (int i=0; i<total_trainers; i++) {
        trainer_names[i] = (char*)malloc(25);
    }
    itinerary **itinerary_index;
    itinerary_index = new_itinerary_dparray(ifp,buf,total_trainers,total_regions,regions_index,trainer_names);

    //Declares, initializes, and outputs trainer data
    trainer **trainers_index;
    trainers_index = new_trainer_dparray(ifp,buf,total_trainers,itinerary_index,trainer_names);

    print_output(ofp,total_trainers,trainers_index); //File prints the output data

    //Frees all pointers
    dispose_all_monster_data(monsters_index, total_monsters);
    dispose_all_region_data(regions_index, total_regions);
    dispose_all_itinerary_data(itinerary_index, total_trainers);
    dispose_all_trainer_data(trainers_index, total_trainers);
    dispose_trainer_names(trainer_names,total_trainers);

    //Closes both the input and output files
    fclose(ifp);
    fclose(ofp);

    printf("\n\nYou made it to the end =D\n");
    return 0;
}
