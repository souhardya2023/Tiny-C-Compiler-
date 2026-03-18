#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"
#include "y.tab.c"
//Declare a global variable count for three address instructions
int addrcount = 0;

extern char code[MAXCODES][CODELEN];
extern int codelen;

//Implement make list functions
void initCode() {
    for (int i = 0; i < MAXCODES; i++) {
        code[i][0] = '\0'; // initialize each string to empty
    }
}
void printCode() {
    printf("+++ Three Address Code\n");
    for(int i=0;i<codelen;i++) {
        if(blockcount[i]==1)
            printf("\n");
        printf("%9d  : %s\n",i,code[i]);
    }
    printf("\n%9d  : \n",codelen);
    //printf("\n");
}
void newTemp() {
    addrcount=addrcount+1;
}

void newLine(){
    codelen = codelen + 1;
}
Node* makeList(int i) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = i;
    temp->next = NULL;
    return temp;
}

Node* merge(Node* l1,Node* l2) {
    if(l1==NULL) return l2;
    if(l2==NULL) return l1;
    Node* temp = l1;
    while(temp->next!=NULL) {
        temp=temp->next;
    }
    temp->next=l2;
    return l1;
}

void backpatch(Node* l,int m) {
    Node* temp = l;
    while(temp!=NULL) {
        sprintf(code[temp->data] + strlen(code[temp->data]), "%d", m);
        blockcount[m]=1;
        temp=temp->next;
    }
    return;
}

struct Addr* accessMem(struct Addr* addr) {
    struct Addr* new;
    if(addr->cat==4 || addr->cat==5) { 
        int new_temp=createAssignmentN(addr);
        new = makeAddr(3,new_temp,addr->type_index);
        free(addr);
        return new;
    }
    else
        return addr;
}
void changeType(struct Addr* addr) {
    if(addr->type_index==1) {
            newTemp();
            sprintf(code[codelen], "\t[lng] t%d = (lng2int) ",addrcount);
            if(addr->cat==3) sprintf(code[codelen] + strlen(code[codelen]), "t%d ",addr->intval);
            else sprintf(code[codelen] + strlen(code[codelen]),"%d ",addr->intval);
            newLine();
            addr->cat=3;
            addr->intval=addrcount;
        }
        else if(addr->type_index==2) {
            newTemp();
            printf(code[codelen], "\t[flt] t%d = (flt2int) ",addrcount);
            if(addr->cat==3) printf(code[codelen] + strlen(code[codelen]),"t%d ",addr->intval);
            else printf(code[codelen] + strlen(code[codelen]),"%.16lf ",addr->fltval);
            newLine();
            addr->cat=3;
            addr->intval=addrcount;
        }
        else if(addr->type_index==3) {
            newTemp();
            sprintf(code[codelen], "\t[dbl] t%d = (dbltoint) ",addrcount);
            if(addr->cat==3) sprintf(code[codelen] + strlen(code[codelen]),"t%d ",addr->intval);
            else sprintf(code[codelen] + strlen(code[codelen]),"%d ",addr->intval);
            // printf("\n");
            newLine();
            addr->cat=3;
            addr->intval=addrcount;
        }
}
struct Addr* copyAddr(struct Addr* addr) {
    struct Addr* newAddr = (struct Addr*)malloc(sizeof(struct Addr));
    newAddr->cat = addr->cat;
    newAddr->intval = addr->intval;
    newAddr->fltval = addr->fltval;
    newAddr->type_index = addr->type_index;
    return newAddr;
}
struct Addr* makeAddr(int cat, int intval, int type_index) {
    struct Addr* newAddr = (struct Addr*)malloc(sizeof(struct Addr));
    newAddr->cat = cat;
    newAddr->intval = intval;
    newAddr->type_index = type_index;
    return newAddr;
}
struct Addr* makeAddrD(int cat, double fltval, int type_index) {
    struct Addr* newAddr = (struct Addr*)malloc(sizeof(struct Addr));
    newAddr->cat = cat;
    newAddr->fltval = fltval;
    newAddr->type_index = type_index;
    return newAddr;
}
void printCast(struct Addr* result, struct Addr* a1) {
    newTemp();
    code[codelen][0] = '\0'; // initialize the string to empty
    sprintf(code[codelen],"\t");
    switch(result->type_index) {
        case 0:
            sprintf(code[codelen] + strlen(code[codelen]),"[int] ");

            break;
        case 1:
            sprintf(code[codelen] + strlen(code[codelen]),"[lng] ");
            //printf("Here %s\n",code[codelen]);
            break;
        case 2:
            sprintf(code[codelen] + strlen(code[codelen]),"[flt] ");
            break;
        case 3:
            sprintf(code[codelen] + strlen(code[codelen]),"[dbl] ");
            break;
        default:
            break;
    }
    sprintf(code[codelen] + strlen(code[codelen]),"t%d = ",addrcount);
    //printf("After adding t%d = : %s\n",addrcount,code[codelen]);
    switch(a1->type_index) {
        case 0: 
            sprintf(code[codelen] + strlen(code[codelen]),"(int2");
            break;
        case 1:
            sprintf(code[codelen] + strlen(code[codelen]),"(lng2");
            break;
        case 2:
            sprintf(code[codelen] + strlen(code[codelen]),"(flt2");
            break;
        case 3:
            sprintf(code[codelen] + strlen(code[codelen]),"(dbl2");
            break;
        default:
            break;
    }
    switch(result->type_index) {
        case 0:
            sprintf(code[codelen] + strlen(code[codelen]),"int) ");
            break;
        case 1:
            sprintf(code[codelen] + strlen(code[codelen]),"lng) ");
            break;
        case 2:
            sprintf(code[codelen] + strlen(code[codelen]),"flt) ");
            break;
        case 3:
            sprintf(code[codelen] + strlen(code[codelen]),"dbl) ");
            break;
        default:
            break;
    }
    if(a1->cat==3) {
        sprintf(code[codelen] + strlen(code[codelen]),"t%d",a1->intval);
    }
    else if(a1->cat==2) {
        sprintf(code[codelen] + strlen(code[codelen]),"%.16lf",a1->fltval);
    }
    else {
        sprintf(code[codelen] + strlen(code[codelen]),"%d",a1->intval);
    }
    //printf("%s\n",code[codelen]);
    newLine();
}
struct Addr* widen(struct Addr* a1,struct Addr* result) {
    if(a1->type_index==result->type_index) {
        return a1;
    }
    printCast(result,a1);
    a1->cat=3;
    a1->intval=addrcount;
    a1->type_index=result->type_index;
    return a1;
}
void printOp(struct Addr* result, struct Addr* a1, struct Addr* a2, char* op) {
     if(a2 == NULL) {
        if (result->cat == 3){
            sprintf(code[codelen] + strlen(code[codelen]), "t%d %s ", result->intval, op);
        }
        else if (result->cat == 2){
            sprintf(code[codelen] + strlen(code[codelen]), "%.16lf %s ", result->fltval, op);
        }
        else {
            sprintf(code[codelen] + strlen(code[codelen]), "%d %s ",result->intval, op);
        }
        if (a1->cat==3) {
            sprintf(code[codelen] + strlen(code[codelen]), "t%d",a1->intval);
        }
        else if(a1->cat==2) {
            sprintf(code[codelen] + strlen(code[codelen]), "%.16lf", a1->fltval);
        }
        else {
            sprintf(code[codelen] + strlen(code[codelen]), "%d", a1->intval);
        }
        return;
     }
     sprintf(code[codelen]+strlen(code[codelen]),"\t");
     newTemp();
     switch(result->type_index) {
           case 0:
               sprintf(code[codelen] + strlen(code[codelen]),"[int] ");
               break;
           case 1:
               sprintf(code[codelen] + strlen(code[codelen]),"[lng] ");
               break;
           case 2:
               sprintf(code[codelen] + strlen(code[codelen]),"[flt] ");
               break;
           case 3:
               sprintf(code[codelen] + strlen(code[codelen]),"[dbl] ");
               break;
           default:
               break;
       }
       sprintf(code[codelen] + strlen(code[codelen]),"t%d = ",addrcount);
       if(a1->cat==3) {
           sprintf(code[codelen] + strlen(code[codelen]),"t%d ",a1->intval);
       }
       else if(a1->cat==2) {
           sprintf(code[codelen] + strlen(code[codelen]),"%.16lf ",a1->fltval);
       }
       else {
           sprintf(code[codelen] + strlen(code[codelen]),"%d ",a1->intval);
       }
       sprintf(code[codelen] + strlen(code[codelen]),"%s ",op);
       if(a2->cat==3) {
           sprintf(code[codelen] + strlen(code[codelen]),"t%d",a2->intval);
       }
       else if(a2->cat==2) {
           sprintf(code[codelen] + strlen(code[codelen]),"%.16lf",a2->fltval);
       }
       else {
           sprintf(code[codelen] + strlen(code[codelen]),"%d",a2->intval);
       }
       newLine();
       result->cat=3;
       result->intval=addrcount;
}
int maxType(int a1,int a2) {
    if((a1==2 && a2==1) || (a1==1 && a2==2)) return 3;
    return (a1 > a2) ? a1 : a2;
}
struct Addr* createOperation(struct Addr* a1, struct Addr* a2, char *op) {
    //Perform type checks
    //Print required three address codes
    struct Addr* result = (struct Addr *) malloc(sizeof(struct Addr));
    result->type_index = maxType(a1->type_index,a2->type_index);
    a1=widen(a1,result);
    a2=widen(a2,result);
    //Store the final result in a new Temporary and print
    printOp(result,a1,a2,op);
    return result;
}
int createOperationS(struct Addr* a1, struct Addr* a2, char *op) {
    //For struct type
    newTemp();
    sprintf(code[codelen],"\t");
    sprintf(code[codelen] + strlen(code[codelen]),"[int] t%d = ",addrcount);
    if(a1->cat==4) {
        sprintf(code[codelen] + strlen(code[codelen]),"%d ",a1->intval);
    }
    else if(a1->cat==5) {
        sprintf(code[codelen] + strlen(code[codelen]),"t%d ",a1->intval);
    }
    sprintf(code[codelen] + strlen(code[codelen]),"%s ",op);
    if(a2->cat==4) {
        sprintf(code[codelen] + strlen(code[codelen]),"%d ",a2->intval);
    }
    else if(a2->cat==5) {
        sprintf(code[codelen] + strlen(code[codelen]),"t%d ",a2->intval);
    }
    newLine();
    return addrcount;
}
int createOperationA(int a1,int a2,char *op,int flag1,int flag2) {
    //For array types
    newTemp();
    sprintf(code[codelen],"\t");
    sprintf(code[codelen] + strlen(code[codelen]),"[int] t%d = ",addrcount);
    if(flag1==0) {
        sprintf(code[codelen] + strlen(code[codelen]),"%d ",a1);
    }
    else if(flag1==1) {
        sprintf(code[codelen] + strlen(code[codelen]),"t%d ",a1);
    }
    sprintf(code[codelen] + strlen(code[codelen]),"%s ",op);
    if(flag2==0) {
        sprintf(code[codelen] + strlen(code[codelen]),"%d",a2);
    }
    else if(flag2==1) {
        sprintf(code[codelen] + strlen(code[codelen]),"t%d",a2);
    }
    newLine();
    return addrcount;
}
void createAssignment(struct Addr* a1, struct Addr* a2) {
    //If the l-value is not of basic data type
    // if(!(a1->type_index>=0 && a1->type_index<=3)) {
    //     printf("\n*** Invalid type of l-value ***\n");
    //     exit(0);
    // }
    if(!(a2->type_index>=0 && a2->type_index<=3)) {
        sprintf(code[codelen],"\n*** Invalid type of r-value ***\n");
        newLine();
        exit(0);
    }
    //If same type directly assign ;
    if(a1->type_index==a2->type_index) {
        sprintf(code[codelen],"\t");
        if(a1->type_index==0) {
        sprintf(code[codelen] + strlen(code[codelen]),"[int] ");
        }
        else if(a1->type_index==1) {
            sprintf(code[codelen] + strlen(code[codelen]),"[lng] ");
        }
        else if(a1->type_index==2) {
            sprintf(code[codelen] + strlen(code[codelen]),"[flt] ");
        }
        else if(a1->type_index==3) {
            sprintf(code[codelen] + strlen(code[codelen]),"[dbl] ");
        }
        if(a1->cat==4) {
            if(a2->cat==3) {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(%d,%d) = t%d",a1->intval,entry[a1->type_index].memsize,a2->intval);
            }
            else if(a2->cat==2) {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(%d,%d) = %.16lf",a1->intval,entry[a1->type_index].memsize,a2->fltval);
            }
            else {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(%d,%d) = %d",a1->intval,entry[a1->type_index].memsize,a2->intval);
            }
            newLine();
        }
        else if(a1->cat==5) {
            if(a2->cat==3) {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(t%d,%d) = t%d",a1->intval,entry[a1->type_index].memsize,a2->intval);
            }
            else if(a2->cat==2) {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(t%d,%d) = %.16lf",a1->intval,entry[a1->type_index].memsize,a2->fltval);
            }
            else {
                sprintf(code[codelen] + strlen(code[codelen]),"MEM(t%d,%d) = %d",a1->intval,entry[a1->type_index].memsize,a2->intval);
            }
            newLine();
        }
    }
    else {
        //printf("To be implemented\n");
        printCast(a1,a2);
        //Assign to required type
        //printf("Codelen before assignment: %d\n",codelen);
        sprintf(code[codelen], "\t");
        //printf("%s\n",code[codelen]);
        if(a1->type_index==0) {
            sprintf(code[codelen] + strlen(code[codelen]),"[int] ");
        }
        else if(a1->type_index==1) {
            sprintf(code[codelen] + strlen(code[codelen]),"[lng] ");
        }
        else if(a1->type_index==2) {
            sprintf(code[codelen] + strlen(code[codelen]),"[flt] ");
        }
        else if(a1->type_index==3) {
            sprintf(code[codelen] + strlen(code[codelen]),"[dbl] ");
        }
        if(a1->cat==4) {
            sprintf(code[codelen] + strlen(code[codelen]),"MEM(%d,%d) = t%d",a1->intval,entry[a1->type_index].memsize,addrcount);
        }
        else if(a1->cat==5) {
            sprintf(code[codelen] + strlen(code[codelen]),"MEM(t%d,%d) = t%d",a1->intval,entry[a1->type_index].memsize,addrcount);
        }
        //printf("%s\n",code[codelen]);
        newLine();
        //printf("Codelen after assignment: %d\n",codelen);
    }      
}
int createAssignmentN(struct Addr* a1) {
    // Direct Assignment
    sprintf(code[codelen], "\t");
    if(a1->type_index==0) {
        sprintf(code[codelen] + strlen(code[codelen]),"[int] ");
    }
    else if(a1->type_index==1) {
        sprintf(code[codelen] + strlen(code[codelen]),"[lng] ");
    }
    else if(a1->type_index==2) {
        sprintf(code[codelen] + strlen(code[codelen]),"[flt] ");
    }
    else if(a1->type_index==3) {
        sprintf(code[codelen] + strlen(code[codelen]),"[dbl] ");
    }
    else {
        sprintf(code[codelen], "\n*** Error: Invalid type of r-value ***");
        newLine();
        exit(0);
    }
    newTemp();
    if(a1->cat==4) {
        sprintf(code[codelen]+strlen(code[codelen]),"t%d = MEM(%d,%d)",addrcount,a1->intval,entry[a1->type_index].memsize);
    }
    else if(a1->cat==5) {
        sprintf(code[codelen]+strlen(code[codelen]),"t%d = MEM(t%d,%d)",addrcount,a1->intval,entry[a1->type_index].memsize);
    }
    newLine();
    a1->cat=5;
    a1->intval=addrcount;
    return addrcount;
}
//DECLIST functions implemented here as it uses global arrays of Table Types and Symbol Types which is required in productions as well
struct info* makepair(int idx, int symtable) {
    struct info * temp = (struct info *)malloc(sizeof(struct info));
    temp->idx=idx;
    temp->symtable = symtable; //Take the symbol table index from one position before DECLIST
    return temp;
}

int lookup(char *vname, int symtablenumber) {
    for(int i=0;i<symcount[symtablenumber];i++) {
        if(strcmp(symtab[symtablenumber][i].varName, vname) == 0) {
            return i;
        }
    }
    return -1;
}
void makeSTEntry ( char * vname, int memsize, int idx,int sname) {
    //printf("  [DEBUG] makeSTEntry: Received vname='%s', memsize=%d, type_idx=%d, symtab_num=%d\n", vname, memsize, idx, sname);
    //Check if variable already exists
    if (!vname) {
        fprintf(stderr, "  [DEBUG] makeSTEntry: FATAL - vname is NULL. Crashing soon.\n");
    }
    for(int i=0;i<symcount[sname];i++) {
        if(strcmp(symtab[sname][i].varName, vname) == 0) {
            fprintf(stderr, "** Error: Variable %s already declared\n", vname);
            exit(0);
        }
    }
    //printf("  [DEBUG] makeSTEntry: Adding variable '%s' of type '%s' (index %d) to symbol table %d at position %d\n", vname, entry[idx].typeName, idx, sname, symcount[sname]);
    symtab[sname][symcount[sname]].varName = vname;
    symtab[sname][symcount[sname]].ttindex = idx;
    symtab[sname][symcount[sname]].memRange.start = memreq[sname] - memsize;
    symtab[sname][symcount[sname]].memRange.end = memreq[sname]-1;
    symcount[sname]++;
}

int makeEntry ( char *tname, int msize ) {
    for(int i=0;i<entrycount;i++) {
        if(strcmp(entry[i].typeName, tname) == 0) {
            //Entry already exists
            return i;
        }
    }
    //Create a new entry
    entry[entrycount].typeName = tname;
    entry[entrycount].memsize = msize;
    entry[entrycount].ref = -1;
    entrycount++;
    return entrycount-1;
}
int  makeArrayEntry ( TT *btype, int idx, int asize ) {
    for(int i=0;i<entrycount;i++) {
        char check[200];
        sprintf(check, "array(%d, %s)", asize, btype->typeName);
        if(strcmp(entry[i].typeName, check) == 0) {
            //Entry already exists
            return i;
        }
    }
    TT * arr = (TT *)malloc(sizeof(TT));
    char * temp = (char *)malloc(strlen(btype->typeName)+20);
    strcpy(temp, "array(");
    char asizestr[10];
    sprintf(asizestr, "%d", asize);
    strcat(temp, asizestr);
    strcat(temp, ", ");
    strcat(temp, btype->typeName);
    strcat(temp, ")");
    arr->typeName = temp;
    arr->memsize = asize * btype->memsize;
    //arr->symtable = symtablecount;
    arr->ref=idx; //Store its reference to the type it is an array of
    arr->dim=asize;
    entry[entrycount] = *arr;
    entrycount++;
    return entrycount-1;
}
int lookupStruct(char *sname) {
    for(int i=0; i<entrycount; i++) {
        char check[200];
        sprintf(check, "struct %s [st = %d]", sname, entry[i].ref);
        if(strcmp(entry[i].typeName, check) == 0) {
            //Entry already exists
           // printf("  [DEBUG] lookupStruct: Found struct '%s' with type index %d and symtab %d\n", sname, i, entry[i].symtable);
            return i;
        }
    }
    return -1; // Not found
}
int makeStructEntry (char *sname) {
    int idx=lookupStruct(sname);
    if(idx!=-1) {
        printf("Struct %s already exists. Cannot redefine\n", sname);
        exit(0);
    }
    //Create a new symbol table for this struct
    TT * struc = (TT *)malloc(sizeof(TT));
    char check[200];
    sprintf(check, "struct %s [st = %d]", sname, symtablecount+1);
    struc->typeName = strdup(check);
    //Marking memsize negative to indicate struct is being defined which will get updated only when entire DECLIST is processed
    struc->memsize = -1; //Will be updated later
    struc->ref = ++symtablecount;
    //Make check = struct sname
    sprintf(check, "struct %s", sname);
    symtabnames[symtablecount]=strdup(check);
    entry[entrycount] = *struc;
    entrycount++;
    return entrycount-1; 
}
void printTypeTable() {
    printf("\n+++ %d types\n",entrycount);
    for (int i = 0; i < entrycount; i++) {
        // Adding a check here to prevent crash from bad pointer
        if (entry[i].typeName == NULL) {
             printf("[DEBUG] printTypeTable: Found NULL typeName at index %d! Skipping.\n", i);
             continue;
        }
        printf("Type %3d:%10d      %s\n",
               i,
               entry[i].memsize,
               entry[i].typeName);
    }
}
void printSymbolTable() {
    printf("\n+++ Symbol Table(s)\n");
    int maxVarLen = 12; 
    for (int j = 0; j <= symtablecount; j++) {
        for (int i = 0; i < symcount[j]; i++) {
            int len = (int)strlen(symtab[j][i].varName);
            if (len > maxVarLen) maxVarLen = len;
        }
    }
    for (int j = 0; j <= symtablecount; j++) {
        printf("\n+++ Symbol Table %d [%s]\n", j, symtabnames[j]);
        for (int i = 0; i < symcount[j]; i++) {
             // Adding a check here to prevent crash from bad pointer
            if (symtab[j][i].ttindex ==-1) {
                printf("+++ Warning : Found NULL type/typeName for var '%s'! Skipping.\n", symtab[j][i].varName);
                continue;
            }
            printf("\t%-*s  %10d - %-10d   type = %3d = %s\n",
                   maxVarLen,
                   symtab[j][i].varName,
                   symtab[j][i].memRange.start,
                   symtab[j][i].memRange.end,
                   symtab[j][i].ttindex,
                   entry[symtab[j][i].ttindex].typeName);
        }
        printf("\tTotal width of this symbol table = %d\n", memreq[j]);
    }
}

 void loadTT(TT *t) {
    makeEntry("int", sizeof(int));
    makeEntry("long", sizeof(long));
    makeEntry("float", sizeof(float));
    makeEntry("double", sizeof(double));
}
int main ( int argc, char *argv[] )
{
   if (argc > 1) yyin = (FILE *)fopen(argv[1],"r");
   loadTT(entry);
   initCode();
   yyparse();
   printCode();
   fclose(yyin);
   exit(0);
}
