#include<stdio.h>
#include<stdlib.h>

int match_word(char *buff, size_t size, int *i, char *word ){
    int done=0;
    int prev=*i;
    int j=0;

    if(*i<size && buff[*i]==word[j]){
        while(*i<size && buff[*i]==word[j] && word[j]!='\0'){
            *i = *i+1;
            j=j+1;
        }

        if(word[j]=='\0')
            done =1;
    }

    if(done!=1)
        *i=prev;
    return done;
}

// Same as match_word, but it only LOOKS at the text. It does not move the
// caller's index, because "i" is passed by value (a copy), not by pointer.
int at_word(char *buff, size_t size, int i, char *word){
    return match_word(buff, size, &i, word);
}

// Reads a decimal number at position *i and moves *i past it.
int read_number(char *buff, size_t size, int *i){
    int x=0;
    while(*i<size && buff[*i]>='0' && buff[*i]<='9'){
        x = x*10 + (buff[*i]-'0');
        *i = *i+1;
    }
    return x;
}

// Returns 1 if "name" is present in list[0..cnt-1], otherwise 0.
int is_in_list(char *name, char *list[], int cnt){
    int i=0;
    while(i<cnt){
        int j=0;
        while(name[j]!='\0' && name[j]==list[i][j])
            j=j+1;
        if(name[j]=='\0' && list[i][j]=='\0')       // both strings ended together = same name
            return 1;
        i=i+1;
    }
    return 0;
}

void skip_till_end(char *buff, size_t size, int *i){
    while(*i<size && buff[*i]!='\n')
        *i = *i + 1;
    //now skipping "\n"
    if(*i<size)
        *i=*i+1;
}

void skip_spaces(char *buff, size_t size,  int *i){
    while(*i<size && (buff[*i]==' ' || buff[*i]=='\t' || buff[*i]=='\r'))
        *i = *i+1;
}

void going_till_space(char *buff, size_t size, int *i){
    while(*i<size && buff[*i]!=' ' && buff[*i]!='\n' && buff[*i]!='\t' && buff[*i]!='\r' && buff[*i]!=',')
        *i = *i+1;
}

void symbol_table(FILE *out, int address, char symbol, char *symbol_name, int size){
    char hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    // 1. Convert and print address formatted to 8 hex digits (e.g., 00000004)
    char addr_arr[8] = {'0','0','0','0','0','0','0','0'};
    unsigned int addr_val = (unsigned int)address;
    int z = 7;

    if (addr_val == 0) {
        addr_arr[7] = '0';
    } else {
        while (addr_val != 0 && z >= 0) {
            addr_arr[z] = hex[addr_val % 16];
            addr_val /= 16;
            z--;
        }
    }

    for (int k = 0; k < 8; k++) {
        fprintf(out, "%c", addr_arr[k]);
    }
    fprintf(out, "\t");

    // 2. symbol (section belong to  eg: b(bss section))
    //                                   d(data section)
    fprintf(out, "%c\t",symbol);

    // 3. symbol_name
    fprintf(out, "%s\t",symbol_name);

    // 4. size
    fprintf(out, "%d\n",size);
}

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Enter the arguments this way, <./create_symbol_table> <assembly_file>\n");
        return 1;
    }

    //opening the assembly file
    FILE *fp = fopen(argv[1],"r");
    if(!fp){
        printf("Error in opening the file %s\n",argv[1]);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t asm_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *asm_buff = malloc(asm_size+1);            // +1 for a safe '\0' at the end
    if(!asm_buff){
        printf("Erron in allocating the memory using the malloc\n");
        fclose(fp);
        return 1;
    }
    fread(asm_buff, 1, asm_size, fp);
    asm_buff[asm_size] = '\0';                      // so asm_buff[i+1] never reads garbage
    fclose(fp);

    // the symbol table is written into this file instead of the screen
    FILE *out = fopen("symbol_table.txt","w");
    if(!out){
        printf("Error in creating the file \"symbol_table.txt\"\n");
        free(asm_buff);
        return 1;
    }

    char *arr[] = {"section .data", "section .bss", "section .text", "global", "extern", "ret"};

    int i,j,old;
    i=0;

    char *global_arr[50];          // max 50 functions can be declared
                                    //  main, myfunction , etc

    char *extern_arr[50];                           //max 50 funcitons can be declared
                                                    //eg: extern printf, scanf, puts

    int addr=0;

    //now checking the assembly file line by line
    while(i<asm_size){

        //matching the word         "section .data"
        if(i<asm_size && match_word(asm_buff, asm_size, &i, arr[0])){
            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int data_var_len = 0;

            // at_word only peeks, so "section .bss"/"section .text" is NOT eaten here
            while(i<asm_size && (!at_word(asm_buff, asm_size, i, arr[1]) && !at_word(asm_buff, asm_size, i, arr[2]))){

                old=i;          //stores the starting index of the variable
                going_till_space(asm_buff, asm_size, &i);
                data_var_len = i-old;           //this contains data variable length

                char *data_var = malloc(data_var_len + 1);

                i=i+1;
                j=0;
                while(j<data_var_len){
                    data_var[j] =  asm_buff[old+j];
                    j=j+1;
                }
                data_var[j] = '\0';

                if(asm_buff[i]=='d' && (asm_buff[i+1]=='b' || asm_buff[i+1]=='w' || asm_buff[i+1]=='d' || asm_buff[i+1]=='q' || asm_buff[i+1]=='t' || asm_buff[i+1]=='o' || asm_buff[i+1]=='y' || asm_buff[i+1]=='z')){

                    if(asm_buff[i]=='d' && asm_buff[i+1]=='b')
                    {
                        int address = addr;
                        int p=i;
                        while(p<asm_size && asm_buff[p]!='"')
                        {
                            p++;
                        }
                        p++;
                        int pre=p;
                        while(p<asm_size && asm_buff[p]!='"')
                        {
                            p++;
                        }
                        int char_size=p-pre;
                        addr += char_size;              // 1 byte per character
                        symbol_table(out, address, 'd', data_var, char_size);
                    }
                   else {             //dd 4byte
                        int address = addr;
                        int p = i + 2;
                        while (p < asm_size && asm_buff[p] != '\n') p++;

                        int count = 0;                  // how many values are on this line
                        int pre = i + 2;
                        while (pre < p) {
                            // skip separators
                            while (pre < p && (asm_buff[pre] == ' ' || asm_buff[pre] == '\t' || asm_buff[pre] == ',' || asm_buff[pre] == '\r')) {
                                pre++;
                            }
                            if (pre >= p)
                                break;

                            // skip the value itself
                            while (pre < p && asm_buff[pre] != ' ' && asm_buff[pre] != '\t' && asm_buff[pre] != ',' && asm_buff[pre] != '\r') {
                                pre++;
                            }
                            count++;
                            addr+=4;
                        }
                        symbol_table(out, address, 'd', data_var, count*4);   // size in bytes
                    }

                    i=i+1;
                }
                else                //if other than db, dw, dd, dq, dt, do, dy, dz then print the error
                    printf("Error in the \"section .data\" declaration of variable\n");

                free(data_var);
                skip_till_end(asm_buff, asm_size, &i);
                skip_spaces(asm_buff, asm_size, &i);
            }
        }

        // matching the word        "section .bss"
        if(i<asm_size && match_word(asm_buff, asm_size, &i, arr[1])){
            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int bss_var_len = 0;

            while(i<asm_size && (!at_word(asm_buff, asm_size, i, arr[2]))){

                old = i;            //stores the starting index of the variable
                going_till_space(asm_buff, asm_size, &i);
                bss_var_len = i-old;            //this contains the bss variable length

                char *bss_var = malloc(bss_var_len +1);

                i=i+1;
                j=0;
                while(j<bss_var_len){
                    bss_var[j] = asm_buff[old+j];
                    j=j+1;
                }
                bss_var[j] = '\0';

                if(asm_buff[i]=='r' && asm_buff[i+1]=='e' && asm_buff[i+2]=='s' && (asm_buff[i+3]=='b' || asm_buff[i+3]=='w' || asm_buff[i+3]=='d' || asm_buff[i+3]=='q' || asm_buff[i+3]=='t' || asm_buff[i+3]=='o')){
                    i=i+3;
                    int address = addr;

                    if(asm_buff[i]=='b'){
                        i=i+1;
                        skip_spaces(asm_buff, asm_size, &i);

                        int int_size = read_number(asm_buff, asm_size, &i);      // resb 64 -> 64 bytes
                        addr += int_size;

                        symbol_table(out, address, 'b', bss_var, int_size);
                    }
                    else if(asm_buff[i]=='d'){
                        i=i+1;
                        skip_spaces(asm_buff, asm_size, &i);

                        int int_size = read_number(asm_buff, asm_size, &i) * 4;  // resd 4 -> 16 bytes
                        addr += int_size;

                        symbol_table(out, address, 'b', bss_var, int_size);
                    }
                    else{
                        printf("Error in the \"section .bss\" declaration of variable\n");
                    }
                }
                else
                    printf("Error in the \"section .bss\" declaration of variable\n");

                free(bss_var);
                skip_till_end(asm_buff, asm_size, &i);
                skip_spaces(asm_buff, asm_size, &i);
            }
        }

        // matching the word        "section .text"
        if(i<asm_size && match_word(asm_buff, asm_size, &i, arr[2])){
            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int global_var_len=0;
            int global_var_cnt=0;

            //now matching      "global"
            //considering that after the "section .text"
            //global main ...   will exist
            if(match_word(asm_buff, asm_size, &i, arr[3])){
                while(i<asm_size && asm_buff[i]!='\n'){
                    if(asm_buff[i]==',')        // if there is more than one global function
                        i=i+1;                  //  eg : global main, myfunction

                    skip_spaces(asm_buff, asm_size, &i);

                    old = i;        //stores the starting index of the variable
                    going_till_space(asm_buff, asm_size, &i);
                    global_var_len = i-old;         //this contains global variable length

                    char *global_var = malloc(global_var_len + 1);

                    int n=0;
                    while(n<global_var_len){
                        global_var[n] = asm_buff[old];
                        old=old+1;
                        n=n+1;
                    }
                    global_var[n] = '\0';

                    if(global_var_cnt<50){
                        global_arr[global_var_cnt] = global_var;
                        global_var_cnt = global_var_cnt +1;
                    } else {
                        printf("Too many global declarations (max 50)\n");
                        free(global_var);
                    }
                }
            }
            else                    // if global word is not there then print some message
                printf("global section is not there\n");

            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int extern_var_len=0;
            int extern_var_cnt=0;
            // now checking the extern word in the file
            if(match_word(asm_buff, asm_size, &i, arr[4])){
                while(i<asm_size && asm_buff[i]!='\n'){
                    if(asm_buff[i]==',')            //if there is more than one function in the extern
                        i=i+1;                      // eg: extern printf, puts, scanf, etc

                    skip_spaces(asm_buff, asm_size, &i);

                    old=i;          //stores the starting index of the variable
                    going_till_space(asm_buff, asm_size, &i);
                    extern_var_len = i-old;     // this contains extern variable length

                    char *extern_var = malloc(extern_var_len +1);

                    int n=0;
                    while(n<extern_var_len){
                        extern_var[n] = asm_buff[old];
                        old=old+1;
                        n=n+1;
                    }
                    extern_var[n] = '\0';

                    if(extern_var_cnt<50){
                        extern_arr[extern_var_cnt] = extern_var;
                        extern_var_cnt = extern_var_cnt + 1;
                    } else {
                        printf("Too many extern declarations (max 50)\n");
                        free(extern_var);
                    }
                }
            }
            else                //if extern word is not there then print something
                printf("nothing in extern\n");

            //copying the remaining asm_buff into a new_asm_buff
            int new_asm_size  = asm_size - i;
            char *new_asm_buff = malloc(new_asm_size + 1);
            int k=0;
            while(k<new_asm_size){
                new_asm_buff[k] = asm_buff[k+i];
                k=k+1;
            }
            new_asm_buff[new_asm_size] = '\0';

            //"T" global
            k=0;
            while(k<global_var_cnt){
                int j=0;
                int address = 0;
                int var_match=0;
                while(j<new_asm_size){
                    if(match_word(new_asm_buff, new_asm_size, &j, global_arr[k]))
                        var_match=1;
                    j=j+1;
                }
                if(var_match==1){
                    symbol_table(out, address, 'T', global_arr[k], 0);
                }
                k=k+1;
            }

            //"U" extern
            k=0;
            while(k<extern_var_cnt){
                int j=0;
                int address = 0;
                int var_match = 0;
                while(j<new_asm_size){
                    if(match_word(new_asm_buff, new_asm_size, &j, extern_arr[k]))
                        var_match=1;
                    j=j+1;
                }
                if(var_match == 1){
                    symbol_table(out, address, 'U', extern_arr[k], 0);
                }
                k=k+1;
            }

            //  label "t"
            k=0;
            int start_pt=0;
            int var_len=0;
            int address = 0;
            while(k<new_asm_size){
                // a new word starts after a space, tab, newline or '\r'
                if(new_asm_buff[k] == ' ' || new_asm_buff[k] == '\t' || new_asm_buff[k] == '\n' || new_asm_buff[k] == '\r')
                    start_pt=k+1;

                if(new_asm_buff[k] == ':'){
                    var_len = k-start_pt;
                    char *var = malloc(var_len+1);

                    j=0;
                    while(j<var_len){
                        var[j] = new_asm_buff[j+start_pt];
                        j=j+1;
                    }
                    var[j] = '\0';

                    // if this label is declared "global", it was already printed as 'T',
                    // so do not print it again as small 't'
                    if(!is_in_list(var, global_arr, global_var_cnt))
                        symbol_table(out, address, 't', var, 0);
                    free(var);
                }
                k=k+1;
            }

            // cleaning up the memory used by this section
            free(new_asm_buff);
            k=0;
            while(k<global_var_cnt){
                free(global_arr[k]);
                k=k+1;
            }
            k=0;
            while(k<extern_var_cnt){
                free(extern_arr[k]);
                k=k+1;
            }
        }

        i=i+1;
    }

    fclose(out);
    free(asm_buff);
    return 0;
}
