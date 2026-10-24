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


void going_till_comma(char *buff, size_t size, int *i){
    while(*i<size && buff[*i]!=',' && buff[*i]!='\n' && buff[*i]!='\r' && buff[*i]!=' ')
        *i=*i+1;
}

int check_main_exist(char *main, char *global_arr[], int cnt){
    int i=0;
    while(i<cnt){                                        
        int j=0;
        int found_match=1;                                  
        while(main[j]!='\0' || global_arr[i][j]!='\0'){    
            if(main[j]!=global_arr[i][j]){
                found_match=0;
                break;
            }
            j=j+1;
        }
        if(found_match)                                     
            return 1;
        i=i+1;
    }
    return 0;                                               
}

int check_mnemonics_exist(char *mnemonics, char *arr, size_t size){
    int done=0;
    int prev;

    int i=0;
    int j=0;
    while(i<size){
        if(arr[i]==mnemonics[j]){
            prev=i;                                         
            while(i<size && arr[i]==mnemonics[j] && mnemonics[j]!='\0'){
                i=i+1;
                j=j+1;
            }

            if(mnemonics[j]=='\0' && i<size && (arr[i]==' ' || arr[i]=='\t' || arr[i]=='\r' || arr[i]=='\n')){ 
                done =1;
                return done;
            }

            i=prev+1;             
            j=0;
        }
        else{
            i=i+1;
        }
    }
    return done;
}

int check_register(char* register_name, char* registers[]){
    int i=0;
    while(i<8){
        int j=0;
        while(j<3 && register_name[j]==registers[i][j]){
            j=j+1;
        }
        if(j==3 && register_name[3]=='\0'){

            return 1;
        }
        i=i+1;
    }
    return 0;
}

void findhex(int arr[], char s[], int size, int ssize, int address)
{
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
        printf("%c", addr_arr[k]);
    }
    printf("  ");

    // 2. Handle 'dd' section (32-bit integer array)
    if (size != 0)
    {
        for (int i = 0; i < size; i++)
        {
            char val_arr[8] = {'0','0','0','0','0','0','0','0'};
            char ans_arr[8] = {'0','0','0','0','0','0','0','0'};
            unsigned int x = (unsigned int)arr[i];
            z = 7;

            if (x == 0) {
                val_arr[7] = '0';
            } else {
                while (x != 0 && z >= 0) {
                    val_arr[z] = hex[x % 16];
                    x /= 16;
                    z--;
                }
            }

            int k=0;
            while(k<8){
                ans_arr[k] = val_arr[7-k-1];
                ans_arr[k+1] = val_arr[7-k-1+1];
                k=k+2;
            }

            // Print 8-digit padded hex representation
            for (int k = 0; k < 8; k++) {
                printf("%c", ans_arr[k]);
            }
        }
            printf("    ");
            for(int i=0; i<size; i++){
                printf("%d ",arr[i]);
            }
        printf("\n");
    }
    // 3. Handle 'db' section (string byte values)
    else if (s != NULL && s[0] != '\0')
    {
        int i = 0; // Start at index 0 without skipping s[0]
        while (i < ssize)
        {
            char op[256];
            int x = (unsigned char)s[i];
            int j = 0;

            if (x == 0) {
                op[j++] = '0';
            } else {
                while (x != 0) {
                    op[j++] = hex[x % 16];
                    x /= 16;
                }
            }
            op[j] = '\0';

            // Reverse hex string for current byte
            int p = 0, q = j - 1;
            while (p < q) {
                char temp = op[p];
                op[p] = op[q];
                op[q] = temp;
                p++;
                q--;
            }

            // Print byte (pad single digit to 2 digits if needed)
            if (j == 1) printf("0");
            for (int k = 0; k < j; k++) {
                printf("%c", op[k]);
            }
            i++;
        }
            printf("      ");
            printf("%s",s);
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Enter the arguments this way, <./validate> <assembly_file>\n");
        return 1;
    }

    FILE *fp = fopen("modrm.txt", "r");
    if(!fp){
        printf("Error in opening the file \"modrm.txt\" \n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t txt_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *txt_buff = malloc(txt_size);
    if(!txt_buff){
        printf("Error in allocating the memory using the malloc\n");
        fclose(fp);
        return 1;
    }

    fread(txt_buff, 1, txt_size, fp);          
    fclose(fp);

    FILE *fp1 = fopen(argv[1],"r");
    if(!fp1){
        printf("Error in opening the file %s\n",argv[1]);
        free(txt_buff);
        return 1;
    }

    fseek(fp1, 0, SEEK_END);
    size_t asm_size = ftell(fp1);
    fseek(fp1, 0, SEEK_SET);

    char *asm_buff = malloc(asm_size);
    if(!asm_buff){
        printf("Erron in allocating the memory using the malloc\n");
        fclose(fp1);
        return 1;
    }
    fread(asm_buff, 1, asm_size, fp1);      
    fclose(fp1);

    char *arr[] = {"section .data", "section .bss", "section .text", "global", "extern", "ret"};
    char *registers[ ] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};

    //now checking the assembly file line by line
    int i, j, old;
    i=0;

    char *data_arr[128];            //  max 128 initialised variables can be declared
                                    //  db : "bVAR_NAME"
                                    //  dd : "dVAR_NAME"

    char *bss_arr[128];             //  max 128 uninitialised variables can be declared
                                    //  resb : "bVAR_NAME"
                                    //  resd : "dVAR_NAME"

    char *global_arr[50];          // max 50 functions can be declared
                                    //  main, myfunction , etc

    char *extern_arr[50];           // max 50 functions can be declared
                                    // eg:  extern printf, scanf, puts
    int addr= 0;
    while(i<asm_size)
    {
        //  matching the word the       "section .data"
        if(i<asm_size && match_word(asm_buff, asm_size, &i, arr[0])){
            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int data_var_len=0;
            int data_var_cnt=0;
            while(i<asm_size && (!match_word(asm_buff, asm_size, &i, arr[1]) && !match_word(asm_buff, asm_size, &i, arr[2]))){
                
                old = i;        //stores the starting index of the variable
                going_till_space(asm_buff, asm_size, &i);
                data_var_len = i-old;           //this contains data variable length

                char *data_var = malloc(data_var_len+2);  

                i=i+1;      //skipping the space " "
                if(asm_buff[i]=='d' && (asm_buff[i+1]=='b' || asm_buff[i+1]=='w' || asm_buff[i+1]=='d' || asm_buff[i+1]=='q' || asm_buff[i+1]=='t' || asm_buff[i+1]=='o' || asm_buff[i+1]=='y' || asm_buff[i+1]=='z')){
                    
                    //calculating the hexadecimal 

                    if(asm_buff[i]=='d' && asm_buff[i+1]=='b')
                    {
                        int address = addr;
                        int arr[]={};
                        int int_size=0;
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
                        char *s= malloc(char_size+1);
                        int j=0;
                        while(pre<p)
                        {
                            s[j]=asm_buff[pre];
                            pre++;
                            j++;
                            addr++;
                        }
                        s[j]='\0';
                        j++;
                        findhex(NULL,s,0,j, address);
                    }
                   else {             //dd 4byte
                        int address = addr;
                        int p = i + 2;
                        while (p < asm_size && asm_buff[p] != '\n') p++;

                        int int_size = p - (i + 2);
                        int *arr = malloc((int_size + 1) * sizeof(int));
                        int j = 0;

                        int pre = i + 2;
                        while (pre < p) {
                            while (pre < p && (asm_buff[pre] == ' ' || asm_buff[pre] == '\t' || asm_buff[pre] == ',')) {
                                pre++;
                            }

                            int x = 0;
                            while (pre < p && asm_buff[pre] >= '0' && asm_buff[pre] <= '9') {
                                x = x * 10 + (asm_buff[pre] - '0');
                                pre++;
                            }

                            arr[j++] = x;
                            pre++;
                            addr+=4;
                        }
                        findhex(arr, NULL, j, 0, address);
                        free(arr);
                    }

                    i=i+1;

                    j=0;
                    data_var[j] = asm_buff[i];
                    j=j+1;
                    i=i+1;
                    int n=0;
                    while(n<data_var_len){
                        data_var[j] = asm_buff[old];
                        old=old+1;
                        j=j+1;
                        n=n+1;                        
                    }
                    data_var[j]='\0';

                    if(data_var_cnt<128){                    
                        data_arr[data_var_cnt] = data_var;
                        data_var_cnt = data_var_cnt + 1;
                    } else {
                        printf("Too many \"section .data\" variables (max 128)\n");
                        free(data_var);
                    }
                }
                else            //if other than db, dw, dd, dq, dt, do, dy, dz  then print the error
                    printf("Error in the \"section .data\" declaration of variable\n");

                skip_till_end(asm_buff, asm_size, &i);
                skip_spaces(asm_buff, asm_size, &i);
            }
        }

        // matching the word        "section .bss"
        if(i<asm_size && match_word(asm_buff, asm_size, &i, arr[1])){
            skip_till_end(asm_buff, asm_size, &i);
            skip_spaces(asm_buff, asm_size, &i);

            int bss_var_len=0;
            int bss_var_cnt=0;
            while(i<asm_size && !match_word(asm_buff, asm_size, &i, arr[2])){
                old=i;      //stores the starting index of the variable
                going_till_space(asm_buff, asm_size, &i);
                bss_var_len = i-old;            //this contains bss variable lenght

                char *bss_var = malloc(bss_var_len+2);   

                i=i+1;          //skipping the space " "
                if(asm_buff[i]=='r' && asm_buff[i+1]=='e' && asm_buff[i+2]=='s' && (asm_buff[i+3]=='b' || asm_buff[i+3]=='w' || asm_buff[i+3]=='d' || asm_buff[i+3]=='q' || asm_buff[i+3]=='t' || asm_buff[i+3]=='o')){
                
                    i=i+3;

                    j=0;
                    bss_var[j] = asm_buff[i];
                    j=j+1;
                    i=i+1;
                    int n=0;
                    while(n<bss_var_len){
                        bss_var[j] = asm_buff[old];
                        old=old+1;
                        j=j+1;
                        n=n+1;                          
                    }
                    bss_var[j] = '\0';

                    if(bss_var_cnt<128){                     
                        bss_arr[bss_var_cnt] = bss_var;
                        bss_var_cnt = bss_var_cnt + 1;
                    } else {
                        printf("Too many \"section .bss\" variables (max 128)\n");
                        free(bss_var);
                    }
                }
                else            //if other than resb, resw, resd, resq, rest, reso  then print the error
                    printf("Error in the \"section .bss\" declaration of variable\n");

                skip_till_end(asm_buff, asm_size, &i);
                skip_spaces(asm_buff, asm_size, &i);
            }
        }

        //  matching the word       "section .text"
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

            //now skipping till end
            skip_till_end(asm_buff, asm_size, &i);
            

            //now going to ":"
            //main function
            int main_len=0;
            char *main;

            old=i;
            while(i<asm_size && asm_buff[i]!=':'){
                main_len = main_len+1;
                i=i+1;
            }
            main = malloc(main_len+1);

            int n=0;
            while(old<asm_size && asm_buff[old]!=':'){  
                main[n] = asm_buff[old];
                old=old+1;
                n=n+1;
            }
            main[n] = '\0';                       

            //now checking that the main exists in the global array
            if(check_main_exist(main, global_arr, global_var_cnt)){
                //now going till end
                skip_till_end(asm_buff, asm_size, &i);
                skip_spaces(asm_buff, asm_size, &i);

                int mnemonics_len=0;
                char *mnemonics;
                while(i<asm_size && !match_word(asm_buff, asm_size, &i, arr[5])){
                   
                    skip_spaces(asm_buff, asm_size, &i);
                    // this is to handle empty lines or line comments inside text section
                    if(i < asm_size && (asm_buff[i] == '\n' ||asm_buff[i] == ';')) {
                        skip_till_end(asm_buff, asm_size, &i);
                        continue;
                    }

                    old=i;
                    mnemonics_len=0;                        
                    while(i<asm_size && asm_buff[i]!=' ' && asm_buff[i]!='\t' && asm_buff[i]!='\n' ){
                        mnemonics_len = mnemonics_len+1;
                        i=i+1;
                    }

                    mnemonics = malloc(mnemonics_len + 1);
                    int n=0;
                    while(n<mnemonics_len){
                        mnemonics[n] = asm_buff[old];
                        old=old+1;
                        n=n+1;
                    }
                    mnemonics[n]='\0';

                    if(!check_mnemonics_exist(mnemonics, txt_buff, txt_size)){
                        printf("%s mnemonics doesn't exist\n",mnemonics);
                    }

                    free(mnemonics);       


                    //now checking the registers
                    skip_spaces(asm_buff, asm_size, &i);//skipping the space" "

                    // Check if line has operands before attempting to parse registers (e.g., ret)
                    if(i < asm_size && asm_buff[i] != '\n' && asm_buff[i] != ';') {
                        int reg1_len;
                        char *reg1;
                        old=i;
                        going_till_comma(asm_buff, asm_size, &i);
                        reg1_len = i-old;
                        reg1 = malloc(reg1_len+1);

                        n=0;
                        while(n<reg1_len){
                            reg1[n] = asm_buff[old];
                            old=old+1;
                            n=n+1;
                        }
                        reg1[n] = '\0';

                        if(!check_register(reg1, registers)){
                            printf("Invalid register : %s\n",reg1);
                        }
                        free(reg1);      

                        //
                        if(i < asm_size && asm_buff[i] == ','){
                            i=i+1;     //skipping the comma ','
                            skip_spaces(asm_buff, asm_size, &i);
                            int reg2_len;
                            char *reg2;

                            old=i;
                            going_till_space(asm_buff, asm_size, &i);
                            reg2_len = i-old;
                            reg2 = malloc(reg2_len+1);

                            n=0;
                            while(n<reg2_len){
                                reg2[n] = asm_buff[old];
                                old = old+1;
                                n=n+1;
                            }
                            reg2[n] = '\0';
                            if(!check_register(reg2, registers)){
                                printf("Invalid register : %s\n",reg2);
                            }
                            free(reg2);
                        }
                    }

                    skip_till_end(asm_buff, asm_size, &i);
                    skip_spaces(asm_buff, asm_size, &i);
                }
            }

            free(main);                        
        }

        i=i+1;
    }

    //at last
    free(asm_buff);
    free(txt_buff);
    return 0;

}
