#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define CLASS_SIZE 30
#define BUFF_SIZE 200
#define NN 5
#define NN_LET 5



typedef struct ListItemType{
    char label[CLASS_SIZE];
    void  *next;
    int count;
}list_item;



void bubble_sort(float *distance,  char **neighbors_class);
void bubble_sort2(float *distance,  char *neighbors_class);
void get_columns_labels(FILE *iris_p, FILE *let_p, char *buffer, list_item **iris_list, list_item **let_list, int *iris_cols, int *let_cols);



int main(int argc, char **argv){

   int i,j;
   int instance_ind, tmp_ind;
   int iris_cols , let_cols;
   int iris_enn_instances, let_enn_instances;
   int max_count;
   FILE *iris_ptr;
   FILE *iris_ptr_iterate;
   FILE *let_ptr, *let_ptr_iterate;
   FILE *iris_enn_ptr, *let_enn_ptr;
   float *iris_attribute, *let_attribute;
   float *tmp_iris_attribute, *tmp_let_attribute;
   float *iris_distance, *let_distance;
   float tmp_dist;
   char   **iris_nn_class, *let_nn_class;
   char let_class, tmp_let_class;
   char *iris_class, *tmp_iris_class;
   char *buffer;   
   int distances_comp;
   char *max_iris_class, max_let_class;
   char *token;

   list_item *iris_list = NULL; list_item *let_list = NULL;
   list_item *iris_item, *let_item;

   //open files
   iris_ptr = fopen("mycsvfiles/irisNormalized.csv", "r");                           if(iris_ptr==NULL){printf("Can't open file!\n"); exit(1);}
   iris_ptr_iterate = fopen("mycsvfiles/irisNormalized.csv", "r");                   if(iris_ptr_iterate==NULL){printf("Can't open file!\n"); exit(1);}
   let_ptr= fopen("mycsvfiles/letter-recognitionNormalized.csv", "r");               if(let_ptr==NULL){printf("Can't open file!\n"); exit(1);}
   let_ptr_iterate = fopen("mycsvfiles/letter-recognitionNormalized.csv", "r");      if(let_ptr_iterate==NULL){printf("Can't open file!\n"); exit(1);}
   iris_enn_ptr = fopen("mycsvfiles/irisENN.csv", "w+");                             if(iris_enn_ptr==NULL){printf("Can't open file!\n"); exit(1);}
   let_enn_ptr = fopen("mycsvfiles/letter-recognitionENN.csv", "w+");                if(let_enn_ptr==NULL){printf("Can't open file!\n"); exit(1);}


   //allocate memory for the buffer
   buffer = (char*)malloc (BUFF_SIZE* sizeof(char));
   if (buffer == NULL) {printf("Can't allocate memory. The program will exit\n"); exit(1);}  

   //find the number of columns and class labels of  each dataset
   get_columns_labels(iris_ptr, let_ptr, buffer, &iris_list, &let_list, &iris_cols, &let_cols);
   if(iris_list==NULL || let_list==NULL){printf("Pointer to the start of list should not be null\n"); exit(1);}  
   //printf("Iris cols:%d, Let_cols:%d\n", iris_cols, let_cols);
   
   
   //ALLOCATE MEMORY FOR ARRAYS
   iris_attribute = (float*)malloc((iris_cols-1)*sizeof(float));     if(iris_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}
   tmp_iris_attribute = (float*)malloc((iris_cols-1)*sizeof(float)); if(tmp_iris_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}
   
   let_attribute = (float*)malloc((let_cols-1)*sizeof(float));     if(let_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}
   tmp_let_attribute = (float*)malloc((let_cols-1)*sizeof(float)); if(tmp_let_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

   iris_class = (char*)malloc(CLASS_SIZE*sizeof(char)); if(iris_class==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
   tmp_iris_class = (char*)malloc(CLASS_SIZE*sizeof(char)); if(tmp_iris_class==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
   iris_nn_class = (char**)malloc(NN*sizeof(char*)); if(iris_nn_class==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
   for(i=0 ;i<NN; i++){
       iris_nn_class[i] = (char*)malloc(CLASS_SIZE*sizeof(char)); 
       if(iris_nn_class[i]==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
   }

   let_nn_class = (char*)malloc(NN_LET*sizeof(char));    

   iris_distance = (float*)malloc(NN*sizeof(float));    if(iris_distance==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
   let_distance = (float*)malloc(NN_LET*sizeof(float)); if(let_distance==NULL){printf("Can't allocate memory.The program will exit"); exit(1);}
    
   
   max_iris_class = (char*)malloc(CLASS_SIZE*sizeof(char));  

   //---------------------------------------------------------------------------------------------------------------------
  
    fseek(iris_ptr, 0, SEEK_SET);
    fscanf(iris_ptr, "%s", buffer);
    fprintf(iris_enn_ptr, "%s\n", buffer);

    //APPLY ENN ALGORITHM ON IRIS DATASET
    instance_ind=0; 
   
    while(fscanf(iris_ptr,"%s", buffer)==1){

        instance_ind++;

        //tokenize the line just read and get each one of the tokens-attributes except class label
        for(i=0, token=strtok(buffer, ","); token!=NULL && i<iris_cols; i++){
             if(i<iris_cols-1){
                sscanf(token, "%f", iris_attribute+i);
             }else{
               sscanf(token, "%s", iris_class);
             }
             token = strtok(NULL, ",");
        }
     

        tmp_ind=0;
        distances_comp=0;
   
        fscanf(iris_ptr_iterate, "%*s");

        while(fscanf(iris_ptr_iterate, "%s", buffer)==1){
            
            tmp_ind++;
            if(tmp_ind!=instance_ind){
                distances_comp++;

                for(i=0, token=strtok(buffer, ","); token!=NULL && i<iris_cols; i++){
                   if(i<iris_cols-1){
                        sscanf(token, "%f", tmp_iris_attribute+i);
                    }else{
                       sscanf(token, "%s", tmp_iris_class);
                    }
                   token = strtok(NULL, ",");
                }

                //compute Euclideian distance
                for(i=0, tmp_dist=0.0; i<iris_cols-1; i++){
                   tmp_dist+= pow((iris_attribute[i]-tmp_iris_attribute[i]),2);
                }
                tmp_dist = sqrt(tmp_dist);

                //check if this this distance just computed is among the N smallest distances
                if(distances_comp<NN){
                    iris_distance[distances_comp-1] = tmp_dist;
                    sscanf(tmp_iris_class, "%s", iris_nn_class[distances_comp-1]);

                }
                else if(distances_comp==NN){
                    iris_distance[distances_comp-1] = tmp_dist;
                    sscanf(tmp_iris_class, "%s", iris_nn_class[distances_comp-1]);
                    bubble_sort(iris_distance, iris_nn_class);
                }
                else{
                   for(i=0; i<NN && tmp_dist>=iris_distance[i]; i++){}
                   if(i<NN){
                      for(j=NN-1; j>i; j--){
                          iris_distance[j] = iris_distance[j-1];
                          sscanf(iris_nn_class[j-1], "%s", iris_nn_class[j]);
                          
                       }
                       iris_distance[i]=tmp_dist;
                       sscanf(tmp_iris_class, "%s", iris_nn_class[i]);
                       
                   }                   
                 }

            }//end if

            
         }// nearest neighbors have been found
         
         fseek(iris_ptr_iterate, 0, SEEK_SET);

         //debug-print the nearest neighbors
         printf("Nearest neighbors' classes of iris item %d: ", instance_ind);
         for(i=0; i<NN; i++){
            printf("%s ", iris_nn_class[i]);
         }
         printf("\n");


         iris_item = iris_list;
         while(iris_item!=NULL){
               (*iris_item).count=0;   iris_item = (*iris_item).next;  
         }

         //count the times each class occurs among the nearest neighbors
         for(i=0; i<NN; i++){
             iris_item = iris_list;
             while(iris_item!=NULL){
                if(strcmp( (*iris_item).label, iris_nn_class[i])==0){(*iris_item).count++;    break;}
                iris_item = (*iris_item).next;
             }
             if(iris_item==NULL){printf("There is not such a class label. The program will exit\n"); exit(1);}
         }

         //set as max the first class and search for the actual max 
         max_count = (*iris_list).count;
         strcpy(max_iris_class, (*iris_list).label);
         iris_item = iris_list;
         while(iris_item!=NULL){
             if((*iris_item).count> max_count){
                max_count = (*iris_item).count;
                strcpy(max_iris_class, (*iris_item).label);
             }
             iris_item = iris_item->next;
         }
      
         
         if(strcmp(iris_class, max_iris_class)==0){
            
            for(i=0, token=strtok(buffer, ","); token!=NULL && i<iris_cols; i++){

                if(i<iris_cols-1){
                    fprintf(iris_enn_ptr, "%f,", iris_attribute[i]); 
                }
                else{
                    fprintf(iris_enn_ptr, "%s\n", iris_class);
                }
            }
         }
         

         
       
     }

     //check how many data items of the initial dataset remain in new Edited dataset
     fseek(iris_enn_ptr, 0, SEEK_SET);
     
     fscanf(iris_enn_ptr, "%*s");
     iris_enn_instances=0;

     while(fscanf(iris_enn_ptr, "%s", buffer)==1){
           iris_enn_instances++; 
     }

     printf("Number of items in the initial iris dataset:%d\n", instance_ind);
     printf("Number of items in the edited  iris dataset:%d\n", iris_enn_instances);

     //CLOSE FILE STREAMS AND FREE ALLOCATED MEMORY
     fclose(iris_ptr); 
     fclose(iris_ptr_iterate); fclose(iris_enn_ptr);  free(iris_distance); free(iris_attribute); 
     free(tmp_iris_attribute);  free(iris_class); free(tmp_iris_class);  free(iris_nn_class); free(max_iris_class);
      
     printf("Done with Iris dataset\n");  
     
     //IRIS DATASET DONE

     //------------------------------------------------------------------------------------------------

     //APPLY ENN ALGORITHM ON LETTER-RECOGNITION DATASET
     fseek(let_ptr, 0, SEEK_SET);
     fscanf(let_ptr, "%s", buffer);
     fprintf(let_enn_ptr, "%s\n", buffer);

     instance_ind=0; 
     while(fscanf(let_ptr,"%s", buffer)==1){
      
        instance_ind++;
        for(i=0, token=strtok(buffer,","); i<let_cols && token!=NULL; i++){
            if(i<let_cols-1){ sscanf(token, "%f", &let_attribute[i]); }
            else{ sscanf(token, "%c", &let_class);}
            token = strtok(NULL, ",");
        }
        
        fscanf(let_ptr_iterate, "%*s");
        tmp_ind=0;
        distances_comp=0;

        fscanf(let_ptr_iterate, "%*s");

        //find the nearest neighbors
        while(fscanf(let_ptr_iterate, "%s", buffer)==1){
              
              tmp_ind++;

              if(tmp_ind!=instance_ind){

                   distances_comp++;
                   for(i=0, token=strtok(buffer,","); i<let_cols && token!=NULL; i++){
                          if(i<let_cols-1){ sscanf(token, "%f", &tmp_let_attribute[i]); }
                          else{ sscanf(token, "%c", &tmp_let_class);}
                          token = strtok(NULL, ",");
                   }

                   //compute Euclideian distance
                   tmp_dist=0.0;
                   for(i=0; i<let_cols; i++){
                       tmp_dist+= pow( (let_attribute[i] - tmp_let_attribute[i]), 2);

                   }
                   tmp_dist = sqrt(tmp_dist);

              
                   //check if the distance just computed is among the smallest distances
                   if(distances_comp<NN_LET){
                       let_distance[distances_comp-1] = tmp_dist;
                       sscanf(&tmp_let_class,"%c", &let_nn_class[distances_comp-1]);
                   }
                   else if(distances_comp==NN_LET){
                       let_distance[distances_comp-1] = tmp_dist;
                       sscanf(&tmp_let_class,"%c", &let_nn_class[distances_comp-1]);
                       bubble_sort2(let_distance, let_nn_class);
                      
                   }
                   else{
                      for(i=0; i<NN_LET && tmp_dist>=let_distance[i]; i++){}
                      if(i<NN_LET){
                          for(j=NN_LET-1; j>i; j--){
                             let_distance[j] = let_distance[j-1];
                             sscanf(&let_nn_class[j-1], "%c", &let_nn_class[j]);
                          
                           }
                           let_distance[i]=tmp_dist;
                           sscanf(&tmp_let_class, "%c", &let_nn_class[i]);
                       
                       }                   

                    }
              
                }
            }//Nearest Neighbors and their classes found 
      
            printf("Nearest neighbors of letter item %d: ", instance_ind);
            for(i=0; i<NN_LET; i++){
                printf("%c ", let_nn_class[i]);
            }
            printf("\n");


            fseek(let_ptr_iterate, 0, SEEK_SET);

            //find how many times each class label occurs - first set count =0 for each class
            let_item =let_list;
            while(let_item!=NULL){ 
               let_item->count=0;
               let_item = let_item->next;
            }

            for(i=0; i<NN_LET; i++){
                 let_item=let_list;
                 while(let_item!=NULL){
                      if(let_nn_class[i]==*(let_item->label) ){ let_item->count++; break;}
                      let_item  = let_item->next;
                 }
                 if(let_item==NULL){
                     printf("Not valid let class label has been assigned to a neighbor. The program will exit\n"); exit(1);
                 }
                 
            }
            

            //find class with most occurences among the nearest neighbors
            max_let_class = *(let_list->label);
            max_count = let_list->count;
            let_item = let_list->next;
            while(let_item!=NULL){
               if(let_item->count> max_count){
                   max_count=let_item->count;
                   max_let_class = *(let_item->label);
               }
               let_item = let_item->next;
                
            }

            if(let_class==max_let_class){
                 for(i=0; i<let_cols-1; i++){
                    fprintf(let_enn_ptr, "%.3f,", let_attribute[i]);
                 }
                 fprintf(let_enn_ptr, "%c\n", let_class);
            }
            //printf("OKK with item %d of let dataset\n", instance_ind);
            
       }//end outer while
       
       fseek(let_enn_ptr,0, SEEK_SET);
       let_enn_instances=0;
       
       fscanf(let_enn_ptr, "%*s");
       while(fscanf(let_enn_ptr, "%s", buffer)==1){
           let_enn_instances++; 
        }

       printf("Number of items in the initial letter-recognition dataset:%d\n", instance_ind);
       printf("Number of items in the edited  letter-recognition dataset:%d\n", let_enn_instances);

       fclose(let_ptr); fclose(let_ptr_iterate); fclose(let_enn_ptr);   
       free(let_attribute); free(let_nn_class); free(tmp_let_attribute); free(let_distance); 
       free(buffer);

       printf("Done with letter-recognition dataset\n");
   


}/// end main

//------------------------------------------------------------------------------------------------------

void bubble_sort(float *distance,  char **neighbors_class){

        int i, j;
        float tmp;
        char *tmp_class;
        tmp_class = (char*)malloc(CLASS_SIZE*sizeof(char));
        if(tmp_class==NULL){printf("Error in bubble_sort(). Can't allocate memmory\n"); exit(1);}
        bool swapped;
        
        for(i=0; i<NN-1; i++){
           swapped=false;
           for(j=NN-1; j>i; j--){
               if(distance[j] < distance[j-1]){
                  swapped = true;
                  tmp = distance[j];
                  strcpy(tmp_class, neighbors_class[j]);
                  distance[j] = distance[j-1];
                  strcpy(neighbors_class[j], neighbors_class[j-1]);
                  distance[j-1] = tmp;
                  strcpy(neighbors_class[j-1], tmp_class);
               }
           }
           if(swapped==false){break;}
           
        }
        
        free(tmp_class);

}

//--------------------------------------------------

void bubble_sort2(float *distance,  char *neighbors_class){

        int i, j;
        float tmp;
        char tmp_class;
        bool swapped;
        
        for(i=0; i<NN-1; i++){
           swapped=false;
           for(j=NN-1; j>i; j--){
               if(distance[j] < distance[j-1]){
                  swapped = true;
                  tmp = distance[j];
                  tmp_class = neighbors_class[j];
                  distance[j] = distance[j-1];
                  neighbors_class[j] =  neighbors_class[j-1];
                  distance[j-1] = tmp;
                  neighbors_class[j-1] = tmp_class;
               }
           }
           if(swapped==false){break;}
           
        }
        
        

}
//-----------------------------------------------------------------------------------------------------------------------------------------
void get_columns_labels(FILE *iris_p, FILE *let_p, char *buffer, list_item **iris_list, list_item **let_list, int *iris_cols, int *let_cols){

     list_item *iris_item, *let_item, *last_item;
     char *token,  *last_token;

     *iris_cols =0; *let_cols =0;

     //find num of columns in iris dataset
     fseek(iris_p, 0, SEEK_SET);  fscanf(iris_p, "%s", buffer);
     token = strtok(buffer,",");
     while(token!=NULL){
        (*iris_cols)++;
        token = strtok(NULL,",");
     }
     

     //find class labels for iris dataset
     while(fscanf(iris_p, "%s", buffer)==1){
         
         //tokenize the line just read
          token = strtok(buffer, ",");
          while(token!=NULL){
             last_token = token;
             token=strtok(NULL,",");
          }
          
          //search the list to find out if this current class label exists within it
          iris_item = *iris_list;   last_item= NULL;
          while(iris_item!=NULL){
             if(strcmp(iris_item->label, last_token)==0){
                 break;
             }
             last_item = iris_item;
             iris_item = iris_item->next;

          }

          if(iris_item==NULL){
              iris_item = (list_item*)malloc(sizeof(list_item));
              iris_item->count=0; strcpy(iris_item->label, last_token); iris_item->next = NULL;
              if(*iris_list==NULL){*iris_list= iris_item; }
              else{  last_item->next = iris_item;}
          }
          
        
     }
 

     ///FIND CLASS LABELS FOR THE LETTER_RECOGNITION DATASET

     fseek(let_p, 0, SEEK_SET);   fscanf(let_p, "%s", buffer);

     token = strtok(buffer,",");
     while(token!=NULL){
        (*let_cols)++;
        token = strtok(NULL,",");
     }
     

      while(fscanf(let_p, "%s", buffer)==1){
         
         //tokenize the line just read
          token = strtok(buffer, ",");
          while(token!=NULL){
             last_token = token;
             token=strtok(NULL,",");
          }
          
          //search the list to find out if this current class label exists within it
          let_item = *let_list;   last_item= NULL;
          while(let_item!=NULL){
             if(strcmp(let_item->label, last_token)==0){
                 break;
             }
             last_item = let_item;
             let_item = let_item->next;

          }

          if(let_item==NULL){
              let_item = (list_item*)malloc(sizeof(list_item));
              let_item->count=0; strcpy(let_item->label, last_token); let_item->next = NULL;
              if(*let_list==NULL){*let_list= let_item; }
              else{  last_item->next = let_item;}
          }
          
        
     }

}

