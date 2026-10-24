#include<stdio.h>
#include<stdlib.h>

void skip_till_end(char *buff, size_t size, int *i){
    while(*i<size && buff[*i]!='\n')
        *i = *i + 1;
    //now skipping "\n"
    if(*i<size)
        *i=*i+1;
}

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Enter the arguments this way, <./nm> <symbol_table>\n");
        return 1;
    }

    //opening the symbol_table file
    FILE *fp = fopen(argv[1],"r");
    if(!fp){
        printf("Error in opening the file %s\n",argv[1]);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t sym_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *sym_buff = malloc(sym_size+1);            // +1 for a safe '\0' at the end
    if(!sym_buff){
        printf("Erron in allocating the memory using the malloc\n");
        fclose(fp);
        return 1;
    }
    fread(sym_buff, 1, sym_size, fp);
    sym_buff[sym_size] = '\0';                      // so sym_buff[i+1] never reads garbage
    fclose(fp);
    
    int i=0;
    while(i<sym_size){
        int space_cnt=0;
        while(space_cnt < 3){
            printf("%c",sym_buff[i]);
            if(sym_buff[i]=='\t'){
                space_cnt = space_cnt + 1;
            }
            i=i+1;
        }
        printf("\n");
        skip_till_end(sym_buff, sym_size, &i);
    }

    free(sym_buff);
    return 0;
}
