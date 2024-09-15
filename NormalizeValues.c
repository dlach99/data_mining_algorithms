#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLASS_SIZE 30
#define BUFF_SIZE 200


//main
int main(int argc, char *argv[]){

    int i;
    int instance, norm_instance;
    int let_cols, iris_cols;
    float *iris_attribute, *let_attribute;
    float *max_iris_attribute, *max_let_attribute;
    char *iris_class, let_class;
    char *buffer;   
    char *token;

    
    //open files
    FILE *p_iris, *p_letters;
    FILE *p_iris_norm, *p_letters_norm;
    
    p_iris = fopen("mycsvfiles/iris.csv", "r");
    if (p_iris == NULL) {printf("The file is not opened. The program will exit\n"); exit(1);}

    p_iris_norm = fopen("mycsvfiles/irisNormalized.csv", "w+");
    if (p_iris_norm == NULL) {printf("The file is not opened. The program will exit\n"); exit(1);}

    p_letters = fopen("mycsvfiles/letter-recognition.csv", "r");
    if (p_letters == NULL) { printf("The file is not opened. The program will exit\n"); exit(1);}

    p_letters_norm = fopen("mycsvfiles/letter-recognitionNormalized.csv", "w+");
    if (p_letters_norm == NULL) {printf("The file is not opened. The program will exit\n"); exit(1);}
    //---------------------------------------------------------------------------------------------------------

    //allocate memory for the buffer
    buffer = (char*)malloc (BUFF_SIZE* sizeof(char));
    if (buffer == NULL) {printf("Can't allocate memory. The program will exit\n"); exit(1);}

    //find the number of columns in each csv file
    fscanf(p_iris, "%s", buffer);   iris_cols= 0; 
    token= strtok(buffer, ",");
    while( token !=NULL){
         iris_cols++;
         token = strtok(NULL, ",");
    }
    
    fscanf(p_letters, "%s", buffer);   let_cols=0;
    token = strtok(buffer,",");
    while( token !=NULL){
        let_cols++;
        token = strtok(NULL, ",");
    }


    printf("Iris row size:%d\n", iris_cols);
    printf("Let row size:%d\n", let_cols);

    //---------------------------------------------------------------------------------------------------------------
    
    //MEMORY ALLOCATION FOR ARRAYS 
    let_attribute = (float*) malloc ((let_cols-1)*sizeof(float));
    if(let_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

    max_let_attribute = (float*) malloc ((let_cols-1)*sizeof(float));
    if(max_let_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

    iris_attribute = (float*) malloc ((iris_cols-1)*sizeof(float));
    if(iris_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

    max_iris_attribute = (float*) malloc ((iris_cols-1)*sizeof(float));
    if(max_iris_attribute==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

    iris_class = (char*) malloc (CLASS_SIZE*sizeof(char));
    if(iris_class==NULL){printf("Can't allocate memory. The program will exit\n"); exit(1);}

    //----------------------------------------------------------------------------------------------------------------   

    //NORMALIZE IRIS DATASET

    //set the file pointer to the begining of the file
    fseek(p_iris, 0, SEEK_SET); 

    fscanf(p_iris, "%*s"); 
    
    //iterate over the iris.csv lines and find the max value of each column (except class column)
    while(fscanf(p_iris, "%s", buffer)==1){ 
        
       for(i=0, token=strtok(buffer,","); i<iris_cols-1 && token!=NULL; i++){
          sscanf(token, "%f ",iris_attribute+i);
          printf("%f ", iris_attribute[i]);
          if(iris_attribute[i]>max_iris_attribute[i]){ 
                max_iris_attribute[i] = iris_attribute[i];
          }

          token = strtok(NULL, ",");
          
        }
        
    }//max found

   printf("Max in iris dataset just found\n");

   fseek(p_iris, 0, SEEK_SET); 
   if(fscanf(p_iris, "%s",buffer)!=1){printf("Cant read the line. The program will exit\n"); exit(1);}
   fprintf(p_iris_norm, "%s\n", buffer); instance=0;

   //divide each value with the max of its column
   while(fscanf(p_iris, "%s", buffer)==1){
       instance++;
  
       for(i=0, token=strtok(buffer,","); i<iris_cols && token!=NULL; i++){
          if(i<(iris_cols-1)){
              sscanf(token, "%f",iris_attribute+i);     iris_attribute[i]/=max_iris_attribute[i];
              if(iris_attribute[i]>1.0){ 
                  printf("Division with max value should give a value >1\n"); exit(1);
              }
           }
           else{
              sscanf(token, "%s", iris_class);
           }
           
           token = strtok(NULL, ",");
        }

        
    
        //write the new data to irisNormalized  
        for(i=0; i<iris_cols-1; i++){
             fprintf(p_iris_norm, "%.3f,", iris_attribute[i]);
        }
        fprintf(p_iris_norm, "%s\n", iris_class);

        printf("Data instance %d of iris dataset just normalized\n", instance);
    }

    //count the instances in the normalized dataset - check if aν error was occured in reading/writing from/to file
    fseek(p_iris_norm, 0 ,SEEK_SET);   norm_instance=0;   fscanf(p_iris_norm, "%*s");
    while(fscanf(p_iris_norm, "%s", buffer)==1){
        norm_instance++;
    }

    printf("Data instances in iris dataset: %d\n", instance);
    printf("Data instances in normalized iris dataset: %d\n", norm_instance);
    
    fclose(p_iris);
    fclose(p_iris_norm);    

    printf("Done with Iris dataset\n\n");
    
    //------------------------------------------------------------------------------------------------------------
    
    //NORMALIZE LETTERS DATASET
   
    fseek(p_letters, 0, SEEK_SET); fscanf(p_letters, "%*s");
    

    while(fscanf(p_letters, "%s", buffer)==1){

        
         for(i=0, token=strtok(buffer,","); i<let_cols-1 && token!=NULL; i++){
             sscanf(token, "%f",let_attribute+i);
             if(let_attribute[i]> max_let_attribute[i]){
                  max_let_attribute[i] = let_attribute[i];
             }
             token = strtok(NULL, ",");
          }
          
    }
    //max for letter dataset found

    
    fseek(p_letters, 0, SEEK_SET);
    fscanf(p_letters, "%s", buffer);
    fprintf(p_letters_norm, "%s\n", buffer);
    instance = 0;

    //write to new file lettersNormalized.csv the normalized dataset
    while(fscanf(p_letters, "%s", buffer)==1){
        instance++;

       for(i=0, token=strtok(buffer,","); i<let_cols && token!=NULL; i++){
            if(i<let_cols-1){
                sscanf(token, "%f",let_attribute+i);    let_attribute[i]/=max_let_attribute[i]; 
                if(let_attribute[i]>1.0){ 
                  printf("Division with max value should give a value >1\n"); exit(1);
                }

            }   
            else{
                sscanf(token, "%c", &let_class);
            }  
            token= strtok(NULL, ","); 
        }

        for(i=0;  i<let_cols-1; i++){
            fprintf(p_letters_norm, "%.3f,", let_attribute[i]);
        }

        fprintf(p_letters_norm, "%c\n", let_class);

        printf("Data item %d of letter-recognition dataset just normalized\n", instance);

    }

    //count the instances in the normalized dataset - check if  any problem was occured in reading/writing from/to file
    fseek(p_letters_norm, 0 ,SEEK_SET);   norm_instance=0;   fscanf(p_letters_norm, "%*s"); 
    while(fscanf(p_letters_norm, "%s", buffer)==1){
        norm_instance++;
    }

    printf("Data instances in letter-recognition dataset: %d\n", instance);
    printf("Data instances in normalized letter-recognition dataset: %d\n", norm_instance);



    printf("Done with letters dataset\n");

    fclose(p_letters); 
    fclose(p_letters_norm);
    free(iris_attribute); free(max_iris_attribute);
    free(let_attribute);
    free(max_let_attribute);
    free(buffer);
    


} //end main








