#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

int get_register_code(char *register_name, char *registers[]){
    int i=0;
    while(i<8){
        int j=0;
        while(register_name[j]!='\0' && registers[i][j]!='\0' && register_name[j]==registers[i][j]) j++;
        if(register_name[j]=='\0' && registers[i][j]=='\0') return i;
        i++;
    }
    return -1;
}

void trim_operand(char *s){
    int n=(int)strlen(s);
    while(n>0 && (s[n-1]==' ' || s[n-1]=='\t' || s[n-1]=='\r')) s[--n]='\0';
    int start=0;
    while(s[start]==' ' || s[start]=='\t') start++;
    if(start>0) memmove(s,s+start,strlen(s+start)+1);
}

int memory_base_register(char *operand, char *registers[]){
    char temp[128];
    int n=0, i=0;
    while(operand[i] && n<126){ temp[n++]=operand[i++]; }
    temp[n]='\0';
    trim_operand(temp);
    if(strncmp(temp,"byte",4)==0) memmove(temp,temp+4,strlen(temp+4)+1);
    else if(strncmp(temp,"dword",5)==0) memmove(temp,temp+5,strlen(temp+5)+1);
    trim_operand(temp);
    int len=(int)strlen(temp);
    if(len>=3 && temp[0]=='[' && temp[len-1]==']'){
        temp[len-1]='\0';
        return get_register_code(temp+1,registers);
    }
    return -1;
}

void print_memory_instruction(char *mnemonic, char *op1, char *op2,
                              char *table, size_t table_size, char *registers[]){
    char a[128], b[128];
    snprintf(a,sizeof(a),"%s",op1); snprintf(b,sizeof(b),"%s",op2);
    trim_operand(a); trim_operand(b);
    int r1=get_register_code(a,registers), r2=get_register_code(b,registers);
    int m1=memory_base_register(a,registers), m2=memory_base_register(b,registers);


    if(strcmp(mnemonic,"cmp")==0 && m1>=0 && strstr(b,"'")!=NULL){
        return;
    }

    if(r1>=0 && m2>=0){
        int opcode=-1;
        if(strcmp(mnemonic,"mov")==0) opcode=0x8B;
        else if(strcmp(mnemonic,"add")==0) opcode=0x03;
        else if(strcmp(mnemonic,"and")==0) opcode=0x23;
        else if(strcmp(mnemonic,"cmp")==0) opcode=0x3B;
        else if(strcmp(mnemonic,"sub")==0) opcode=0x2B;
        else if(strcmp(mnemonic,"xor")==0) opcode=0x33;
        if(opcode>=0){
            int modrm=(r1<<3)|m2;
            printf("Instruction bytes: %02X %02X (ModR/M = %02X)\n",opcode,modrm,modrm);
            return;
        }
    }

    if(m1>=0 && r2>=0){
        int opcode=-1;
        if(strcmp(mnemonic,"mov")==0) opcode=0x89;
        else if(strcmp(mnemonic,"add")==0) opcode=0x01;
        else if(strcmp(mnemonic,"and")==0) opcode=0x21;
        else if(strcmp(mnemonic,"cmp")==0) opcode=0x39;
        else if(strcmp(mnemonic,"sub")==0) opcode=0x29;
        else if(strcmp(mnemonic,"xor")==0) opcode=0x31;
        if(opcode>=0){
            int modrm=(r2<<3)|m1;
            printf("Instruction bytes: %02X %02X (ModR/M = %02X)\n",opcode,modrm,modrm);
            return;
        }
    }

    if(strcmp(mnemonic,"mov")==0 && r1>=0 && r2<0 && m2<0 &&
       ((b[0]>='A' && b[0]<='Z') || (b[0]>='a' && b[0]<='z') || b[0]=='_')){
        int label_ok=1;
        for(int z=1; b[z]!='\0'; z++){
            if(!((b[z]>='A' && b[z]<='Z') || (b[z]>='a' && b[z]<='z') ||
                 (b[z]>='0' && b[z]<='9') || b[z]=='_')) { label_ok=0; break; }
        }
        if(label_ok){
            printf("Instruction bytes: %02X [00000000] (address of label %s; relocation required)\n",0xB8+r1,b);
            return;
        }
    }
}

void print_modrm_for_registers(char *mnemonic, char *reg1, char *reg2,
                               char *table, size_t table_size, char *registers[]){
    char opcode_text[16], table_mnemonic[32], operand1[16], operand2[16], direction[16], digit[16];
    int pos=0;
    int r1=get_register_code(reg1, registers);
    int r2=get_register_code(reg2, registers);
    if(r1<0 || r2<0) return;

    while(pos<(int)table_size){
        int start=pos;
        while(pos<(int)table_size && table[pos]!='\n') pos++;
        int len=pos-start;
        if(len>0 && len<127){
            char line[128];
            int k=0;
            while(k<len){ line[k]=table[start+k]; k++; }
            line[k]='\0';
            if(sscanf(line, "%15s %31s %*s %15s %15s %15s %15s", opcode_text, table_mnemonic, operand1, operand2, direction, digit)==6 &&
               strcmp(table_mnemonic, mnemonic)==0 && strcmp(operand1,"reg")==0 && strcmp(operand2,"reg")==0){
                int opcode=(int)strtol(opcode_text,NULL,16);
                int reg_field, rm_field;
                
                if(opcode==0x8B || opcode==0x23 || opcode==0x3B || opcode==0x2B || opcode==0x33){
                    reg_field=r1; rm_field=r2;
                } else {
                    reg_field=r2; rm_field=r1;
                }
                return;
            }
        }
        if(pos<(int)table_size) pos++;
    }
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

void print_memory_lines(char *asm_buff, size_t asm_size, char *table,
                        size_t table_size, char *registers[]){
    size_t p=0;
    int in_text=0;
    while(p<asm_size){
        size_t start=p;
        while(p<asm_size && asm_buff[p]!='\n') p++;
        size_t end=p;
        if(p<asm_size) p++;
        char line[512];
        size_t len=end-start;
        if(len>=sizeof(line)) len=sizeof(line)-1;
        memcpy(line,asm_buff+start,len); line[len]='\0';
        trim_operand(line);
        if(line[0]=='\0' || line[0]==';') continue;
        if(strncmp(line,"section .text",12)==0){ in_text=1; continue; }
        if(strncmp(line,"section .data",12)==0 || strncmp(line,"section .bss",12)==0){ in_text=0; continue; }
        if(!in_text || strncmp(line,"global ",7)==0 || strncmp(line,"extern ",7)==0) continue;
        char *colon=strchr(line,':');
        if(colon){
            char *after=colon+1; trim_operand(after);
            if(*after=='\0') continue;
            memmove(line,after,strlen(after)+1);
        }
        char *cursor=line;
        char *mn=cursor;
        while(*cursor && *cursor!=' ' && *cursor!='\t') cursor++;
        if(*cursor=='\0') continue;
        *cursor++='\0';
        while(*cursor==' ' || *cursor=='\t') cursor++;
        if(*cursor=='\0' || *cursor==';') continue;
        char *comma=NULL; int quote=0, bracket=0;
        for(char *q=cursor; *q; q++){
            if(*q=='\'') quote=!quote;
            else if(!quote && *q=='[') bracket++;
            else if(!quote && *q==']') bracket--;
            else if(!quote && bracket==0 && *q==','){ comma=q; break; }
        }
        if(!comma) continue;
        *comma='\0'; char *op1=cursor; char *op2=comma+1;
        trim_operand(op1); trim_operand(op2);
        int starts_label = (op2[0]>='A' && op2[0]<='Z') ||
                           (op2[0]>='a' && op2[0]<='z') || op2[0]=='_';
        if(memory_base_register(op1,registers)>=0 || memory_base_register(op2,registers)>=0 ||
           (strcmp(mn,"mov")==0 && get_register_code(op1,registers)>=0 &&
            get_register_code(op2,registers)<0 && op2[0]!='[' && starts_label)){
            printf("%s %s, %s -> ",mn,op1,op2);
            print_memory_instruction(mn,op1,op2,table,table_size,registers);
        }
    }
}


typedef struct { char name[128]; unsigned int address; } ListingLabel;
typedef struct { char text[512]; unsigned int address; int size; } ListingLine;

static void listing_trim(char *s) {
    size_t n;
    while (*s==' ' || *s=='\t' || *s=='\r') memmove(s,s+1,strlen(s));
    n=strlen(s);
    while(n>0 && (s[n-1]==' ' || s[n-1]=='\t' || s[n-1]=='\r')) s[--n]='\0';
}
static int listing_reg(const char *s) {
    if(!strcmp(s,"eax")) return 0; if(!strcmp(s,"ecx")) return 1;
    if(!strcmp(s,"edx")) return 2; if(!strcmp(s,"ebx")) return 3;
    if(!strcmp(s,"esp")) return 4; if(!strcmp(s,"ebp")) return 5;
    if(!strcmp(s,"esi")) return 6; if(!strcmp(s,"edi")) return 7;
    return -1;
}
static void listing_split(char *text, char *mn, char *a, char *b) {
    char *p=text, *q; mn[0]=a[0]=b[0]='\0';
    while(*p==' ' || *p=='\t') p++;
    q=p; while(*q && *q!=' ' && *q!='\t') q++;
    if(q==p) return;
    if(*q) *q++='\0';
    snprintf(mn,64,"%s",p);
    while(*q==' ' || *q=='\t') q++;
    if(!*q) return;
    p=q; while(*q && *q!=',') q++;
    if(*q==',') { *q++='\0'; snprintf(a,128,"%s",p); while(*q==' '||*q=='\t') q++; snprintf(b,128,"%s",q); }
    else snprintf(a,128,"%s",p);
    listing_trim(a); listing_trim(b);
}
static int listing_size(char *text) {
    char mn[64],a[128],b[128]; int r;
    listing_split(text,mn,a,b);
    if(!strcmp(mn,"nop") || !strcmp(mn,"hlt") || !strcmp(mn,"ret")) return 1;
    if(!strcmp(mn,"jmp")) return 5;
    if(!strcmp(mn,"jg")) return 2;
    if(!strcmp(mn,"mov")) {
        r=listing_reg(a);
        if(r>=0 && b[0]) return listing_reg(b)>=0 ? 2 : 5;
    }
    if(!strcmp(mn,"add") && listing_reg(a)>=0 && listing_reg(b)>=0) return 2;
    if(!strcmp(mn,"sub") && listing_reg(a)>=0 && b[0]) return 3;
    return 0;
}
static int listing_find_label(ListingLabel *labels,int count,const char *name) {
    int k; for(k=0;k<count;k++) if(!strcmp(labels[k].name,name)) return (int)labels[k].address;
    return -1;
}
static void listing_put32(unsigned char *out, unsigned int x) {
    out[0]=(unsigned char)(x&255); out[1]=(unsigned char)((x>>8)&255);
    out[2]=(unsigned char)((x>>16)&255); out[3]=(unsigned char)((x>>24)&255);
}
static int listing_number(const char *s, unsigned int *v) {
    char *end; long x;
    if(!s || !*s) return 0;
    x=strtol(s,&end,0); if(end==s || *end!='\0') return 0;
    *v=(unsigned int)x; return 1;
}
static void print_text_listing(char *source, size_t source_size) {
    ListingLine lines[2048]; ListingLabel labels[512];
    int line_count=0,label_count=0,in_text=0; size_t p=0; unsigned int lc=0; int pass;
    memset(lines,0,sizeof(lines)); memset(labels,0,sizeof(labels));
    
    while(p<source_size && line_count<2048) {
        char raw[512], *semi, *colon; size_t start=p,n; char mn[64],a[128],b[128];
        while(p<source_size && source[p]!='\n') p++;
        n=p-start; if(n>=sizeof(raw)) n=sizeof(raw)-1;
        memcpy(raw,source+start,n); raw[n]='\0'; if(p<source_size) p++;
        semi=strchr(raw,';'); if(semi) *semi='\0'; listing_trim(raw); if(!raw[0]) continue;
        if(!strncmp(raw,"section .text",12)) { in_text=1; continue; }
        if(!strncmp(raw,"section .data",12)||!strncmp(raw,"section .bss",12)) { in_text=0; continue; }
        if(!in_text || !strncmp(raw,"global ",7) || !strncmp(raw,"extern ",7)) continue;
        colon=strchr(raw,':');
        if(colon) {
            *colon='\0'; listing_trim(raw);
            if(raw[0] && label_count<512) { snprintf(labels[label_count].name,sizeof(labels[label_count].name),"%s",raw); labels[label_count++].address=lc; }
            memmove(raw,colon+1,strlen(colon+1)+1); listing_trim(raw); if(!raw[0]) continue;
        }
        snprintf(lines[line_count].text,sizeof(lines[line_count].text),"%s",raw);
        lines[line_count].address=lc; lines[line_count].size=listing_size(raw);
        if(lines[line_count].size==0) { /* Skip unsupported instruction messages. */ }
        else lc+=(unsigned int)lines[line_count].size;
        line_count++;
    }
    
    for(pass=0;pass<line_count;pass++) {
        unsigned char out[8]={0}; int count=0; char mn[64],a[128],b[128], original[512]; unsigned int imm=0;
        snprintf(original,sizeof(original),"%s",lines[pass].text);
        listing_split(original,mn,a,b);
        if(!strcmp(mn,"nop")) { out[0]=0x90; count=1; }
        else if(!strcmp(mn,"hlt")) { out[0]=0xF4; count=1; }
        else if(!strcmp(mn,"ret")) { out[0]=0xC3; count=1; }
        else if(!strcmp(mn,"mov")) {
            int dst=listing_reg(a),src=listing_reg(b);
            if(dst>=0 && src>=0) { out[0]=0x89; out[1]=(unsigned char)(0xC0|(src<<3)|dst); count=2; }
            else if(dst>=0) {
                if(!listing_number(b,&imm)) { int la=listing_find_label(labels,label_count,b); imm=(la>=0)?(unsigned int)la:0; }
                out[0]=(unsigned char)(0xB8+dst); listing_put32(out+1,imm); count=5;
            }
        }
        else if(!strcmp(mn,"add") && listing_reg(a)>=0 && listing_reg(b)>=0) {
            int dst=listing_reg(a),src=listing_reg(b); out[0]=0x01; out[1]=(unsigned char)(0xC0|(src<<3)|dst); count=2;
        }
        else if(!strcmp(mn,"sub") && listing_reg(a)>=0 && listing_number(b,&imm) && imm<=127) {
            int dst=listing_reg(a); out[0]=0x83; out[1]=(unsigned char)(0xE8|dst); out[2]=(unsigned char)imm; count=3;
        }
        else if(!strcmp(mn,"jmp")) {
            int target=listing_find_label(labels,label_count,a); int displacement;
            if(target<0) {
                fprintf(stderr,"Listing: unresolved label: %s\n",a);
                continue;
            }
            displacement=target-(int)(lines[pass].address+5);
            out[0]=0xE9; listing_put32(out+1,(unsigned int)displacement); count=5;
        }
        else if(!strcmp(mn,"jg")) {
            int target=listing_find_label(labels,label_count,a); int displacement;
            if(target<0) {
                fprintf(stderr,"Listing: unresolved label: %s\n",a);
                continue;
            }
            displacement=target-(int)(lines[pass].address+2);
            if(displacement < -128 || displacement > 127) {
                fprintf(stderr,"Listing: jg target is too far for short jump: %s\n",a);
                continue;
            }
            out[0]=0x7F; out[1]=(unsigned char)displacement; count=2;
        }
        if(count==0) continue;
        printf("%08X  ",lines[pass].address);
        for(int k=0;k<count;k++) printf("%02X ",out[k]);
        for(int k=count;k<5;k++) printf("   ");
        printf("            %s\n",lines[pass].text);
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
            printf("\nData Section : \n\n");
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
            printf("\n\n");
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

                    // Keep mnemonics available until ModR/M lookup is complete.

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

                        if(strcmp(mnemonics,"jmp")!=0 && strcmp(mnemonics,"jg")!=0 &&
                           !check_register(reg1, registers)){
                            printf("Invalid register : %s\n",reg1);
                        }
                        // reg1 is kept until both operands have been checked.

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
                            if(check_register(reg1, registers) && check_register(reg2, registers)) {
                                print_modrm_for_registers(mnemonics, reg1, reg2, txt_buff, txt_size, registers);
                            } else {
                                print_memory_instruction(mnemonics, reg1, reg2, txt_buff, txt_size, registers);
                                if(!check_register(reg1, registers) && memory_base_register(reg1, registers)<0 &&
                                   strncmp(reg1,"byte",4)!=0) {
                                    printf("Invalid register : %s\n",reg1);
                                }
                            }
                            free(reg2);
                        }
                        free(reg1);
                    }

                    free(mnemonics);
                    skip_till_end(asm_buff, asm_size, &i);
                    skip_spaces(asm_buff, asm_size, &i);
                }
            }

            free(main);                        
        }

        i=i+1;
    }

    print_memory_lines(asm_buff, asm_size, txt_buff, txt_size, registers);

    printf("\n\n\nMain Section : \n\n");
    print_text_listing(asm_buff, asm_size);


    //at last
    free(asm_buff);
    free(txt_buff);
    return 0;

}
