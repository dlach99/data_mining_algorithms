#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define CLASS_SIZE 30
#define BUFF_SIZE 200
#define NN 5
#define NN_LET 5


int main(int argc, char **argv){
    
    int i, j;
    int iris_cols, let_cols;
    int random_iris, random_let;
    int iris_instances, let_instances, ib2_instances;
    float *iris_attr, *iris_ib2_attr;
    float *let_attr, *let_ib2_attr;
    float distance, min_distance;
    char *token;
    char *iris_class, let_class;
    char *iris_nn_class, let_nn_class;
    char *iris_ib2_class, let_ib2_class;
    bool exist;      
    char *buffer, *tmp_buffer;

    FILE *iris_ptr, *let_ptr; 
    FILE *iris_ib2_ptr, *let_ib2_ptr;


    //allocate memory for the buffer
    buffer = (char*)malloc (BUFF_SIZE* sizeof(char));
    if (buffer == NULL) {printf("Can't allocate memory. The program will exit\n"); exit(1);}  

    //open files
    iris_ptr = fopen("mycsvfiles/irisNormalized.csv", "r");                     if(iris_ptr==NULL){printf("Can't open file!\n"); exit(1);}
    iris_ib2_ptr = fopen("mycsvfiles/irisIB2.csv", "w+");                       if(iris_ib2_ptr==NULL){printf("Can't open file!\n"); exit(1);}
    let_ptr= fopen("mycsvfiles/letter-recognitionNormalized.csv", "r");         if(let_ptr==NULL){printf("Can't open file!\n"); exit(1);}
    let_ib2_ptr= fopen("mycsvfiles/letter-recognitionIB2.csv", "w+");           if(let_ib2_ptr==NULL){printf("Can't open file!\n"); exit(1);}

    
    //find the number of columns in each one of the normalized csv files
    fscanf(iris_ptr, "%s", buffer);
    token = strtok(buffer, ",");  iris_cols=0;
    while(token!=NULL){
       iris_cols++;
       token = strtok(NULL, ",");
    }

    fscanf(let_ptr, "%s", buffer); 
    token = strtok(buffer, ",");  let_cols=0;
    while(token!=NULL){
       let_cols++;
       token = strtok(NULL, ",");
    }
    //----------------------------------------------------------------------------

    //MEMORY ALLOCATION
    iris_attr = (float*)malloc(sizeof(float)*(iris_cols-1));         if(iris_attr==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    iris_ib2_attr = (float*)malloc(sizeof(float)*(iris_cols-1));   if(iris_ib2_attr==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    iris_class = (char*)malloc(sizeof(char)*CLASS_SIZE);           if(iris_class==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    let_attr = (float*)malloc(sizeof(float)*(let_cols-1));          if(let_attr==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    let_ib2_attr = (float*)malloc(sizeof(float)*(let_cols-1));    if(let_ib2_attr==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    iris_nn_class = (char*)malloc(sizeof(char)*CLASS_SIZE);        if(iris_nn_class==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    iris_ib2_class = (char*)malloc(sizeof(char)*CLASS_SIZE);       if(iris_ib2_class==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    buffer = (char*)malloc(sizeof(char)*BUFF_SIZE);                if(buffer==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    tmp_buffer = (char*)malloc(sizeof(char)*BUFF_SIZE);            if(tmp_buffer==NULL){printf("Can't allocate memory. The program will exit\n");  exit(1);}
    
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------
    
     //find the number of instances in each dataset
     iris_instances=0; let_instances=0;  
     while(fscanf(iris_ptr, "%*s")!=EOF){
         iris_instances++;
     }

     while(fscanf(let_ptr, "%*s")!=EOF){
        let_instances++;

     }

     printf("Iris instances: %d\n", iris_instances);
     printf("Letter-recognition instances: %d\n", let_instances);
     //---------------------------------------------------------------------------------


     //select a random item and move it to the condensing set
     random_iris = rand()%iris_instances+ 1;
     random_let = rand()%let_instances+1;

     fseek(iris_ptr, 0, SEEK_SET);
     fscanf(iris_ptr, "%s", buffer); i=0;
     fprintf(iris_ib2_ptr, "%s\n", buffer);

     while(fscanf(iris_ptr, "%s", buffer)==1){
        i++;
        if(i==random_iris){
            fprintf(iris_ib2_ptr, "%s\n", buffer);
            break;
         }
         
     }

     fseek(let_ptr, 0, SEEK_SET);
     fscanf(let_ptr, "%s", buffer);   i=0;
     fprintf(let_ib2_ptr, "%s\n", buffer);

     while(fscanf(let_ptr, "%s", buffer)==1){
         i++;
         if(i==random_let){
              fprintf(let_ib2_ptr, "%s\n", buffer);
              break;
         }

     }
     //---------------------------------------------------------------------------------------------------------------


     //APPLY IB2 ON IRIS DATASET
     fseek(iris_ptr, 0, SEEK_SET); fscanf(iris_ptr, "%*s"); ib2_instances=1; 
     while(fscanf(iris_ptr, "%s", buffer)==1){

          //check if the current item of training set has an identical item that has already been added to the condensing set 
          fseek(iris_ib2_ptr, 0, SEEK_SET); fscanf(iris_ib2_ptr, "%*s"); exist=false;
          while(fscanf(iris_ib2_ptr, "%s", tmp_buffer)==1 && exist==false){
             if(strcmp(buffer, tmp_buffer)==0){
               exist=true;
             }
          }

      
          //if not, find the nearest neighbor and its class label
          if(exist==false){
            
               strcpy(tmp_buffer, buffer);
            
               //extract tokens of training set item
               for(i=0, token=strtok(tmp_buffer, ","); token!=NULL && i<iris_cols; i++){
                       
                       if(i<iris_cols-1){
                          sscanf(token, "%f", iris_attr+i);
                       }
                       else{
                         sscanf(token, "%s", iris_class);
                       }

                       token = strtok(NULL,",");
                 }

                 fseek(iris_ib2_ptr, 0, SEEK_SET); fscanf(iris_ib2_ptr, "%*s");
                 min_distance=-1.0;

                 //read each line of the IB2 file - get each item of the condensing set
                 while( fscanf(iris_ib2_ptr, "%s", tmp_buffer)==1 ){
                  
                      //extract tokens 
                      for(i=0, token=strtok(tmp_buffer, ","); token!=NULL && i<iris_cols; i++){
                       
                          if(i<iris_cols-1){
                             sscanf(token, "%f", iris_ib2_attr+i);
                          }
                          else{
                            sscanf(token, "%s", iris_ib2_class);
                          }

                          token = strtok(NULL,",");
                      }


                      //compute Euclideian distance
                      distance=0.0;
                      for(i=0; i<iris_cols-1; i++){
                          distance+= pow( (iris_attr[i] - iris_ib2_attr[i]), 2 );
                      }
                      distance = sqrt(distance);

                      if(min_distance<0 || distance<min_distance){
                         min_distance =distance;
                         strcpy(iris_nn_class, iris_ib2_class);
                      }

                  }
    
                  //if the nearest neighbor in the condensing set has a different class label then add the item to the condensing set
                  if(strcmp(iris_nn_class, iris_class)!=0){
                      fprintf(iris_ib2_ptr, "%s\n", buffer);
                      printf("TS item's class: %s - NN class: %s\n", iris_class, iris_nn_class);
                      ib2_instances++;
                  }
                  
                  
            }//end if

      }//end while

     
      printf("Number of instances in initial dataset: %d\n", iris_instances);
      printf("Number of instances in condensing set: %d\n", ib2_instances);
      printf("Done with iris dataset\n\n");

      fclose(iris_ptr); fclose(iris_ib2_ptr); free(iris_attr); free(iris_ib2_attr); free(iris_class); free(iris_nn_class);
      free(iris_ib2_class);
      //-------------------------------------------------------------------------------------------------------------


      //APPLY IB2 ON LETTER-RECOGNITION DATASET
      fseek(let_ptr, 0, SEEK_SET); fscanf(let_ptr, "%*s"); ib2_instances=1;
      while(fscanf(let_ptr, "%s", buffer)==1){

          //check if the current item of training set has an identical item that has already been added to the condensing set 
          fseek(let_ib2_ptr, 0, SEEK_SET); fscanf(let_ib2_ptr, "%*s"); exist=false;
          while(fscanf(let_ib2_ptr, "%s", tmp_buffer)==1 && exist==false){
             if(strcmp(buffer, tmp_buffer)==0){
               exist=true;
             }
          }

      
          //if not, find the nearest neighbor and its class label
          if(exist==false){
            
               strcpy(tmp_buffer, buffer);
            
               //extract tokens of training set item
               for(i=0, token=strtok(tmp_buffer, ","); token!=NULL && i<let_cols; i++){
                       
                       if(i<let_cols-1){
                          sscanf(token, "%f", let_attr+i);
                       }
                       else{
                         let_class= *token;
                       }

                       token = strtok(NULL,",");
                 }

                 fseek(let_ib2_ptr, 0, SEEK_SET); fscanf(let_ib2_ptr, "%*s");
                 min_distance=-1.0;

                 //read each line of the IB2 file - get each item of the condensing set
                 while( fscanf(let_ib2_ptr, "%s", tmp_buffer)==1 ){
                  
                      //extract tokens 
                      for(i=0, token=strtok(tmp_buffer, ","); token!=NULL && i<let_cols; i++){
                       
                          if(i<let_cols-1){
                             sscanf(token, "%f", let_ib2_attr+i);
                          }
                          else{
                            let_ib2_class = *token;
                          }

                          token = strtok(NULL,",");
                      }


                      //compute Euclideian distance
                      distance=0.0;
                      for(i=0; i<let_cols-1; i++){
                          distance+= pow( (let_attr[i] - let_ib2_attr[i]), 2 );
                      }
                      distance = sqrt(distance);

                      if(min_distance<0 || distance<min_distance){
                         min_distance =distance;
                         let_nn_class = let_ib2_class;
                      }

                  }
    
                  //if the nearest neighbor in the condensing set has a different class label then add the item to the condensing set
                  if(let_class!=let_nn_class){
                      fprintf(let_ib2_ptr, "%s\n", buffer);
                      printf("TS item's class: %c - NN class: %c\n", let_class, let_nn_class);
                      ib2_instances++;
                  }
                  
                  
            }//end if

      }//end while

     
      printf("Number of instances in initial dataset: %d\n", let_instances);
      printf("Number of instances in condensing set: %d\n", ib2_instances);
      printf("Done with letter-recognition dataset\n\n");

      fclose(let_ptr); fclose(let_ib2_ptr); free(let_attr); free(let_ib2_attr); 
      free(buffer); free(tmp_buffer); 
      


}//end main