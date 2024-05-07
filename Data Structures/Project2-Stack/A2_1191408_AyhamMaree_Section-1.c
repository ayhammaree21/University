#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node* Mynode;
typedef Mynode Stack;
struct node{
char info[100];
Mynode Next;
};
void PopUn( Stack S,Stack S2 );
void PopRe( Stack S2,Stack S );
void Pop(Stack S);
int IsEmpty( Stack S );
Stack CreateStack( Stack S);
void Push(Stack S,char X[100]);
void Display(Stack S);
void WriteToFile(Stack S);
int main(){
    Stack S=CreateStack(NULL);
    Stack S2=CreateStack(NULL);
    int ch=0;
    char X[100];
    while(1)
    {

        printf("****************************************Welcome To Menu ******************************************\n");
        printf("\n");
        printf("1-Add Data\n");
         printf("2-Undo Last Data You Entered\n");
          printf("3-Redo Last Data You Removed\n");
           printf("4- Print The Data you added in stack  \n");
            printf("5- Save Data Of Stack In File results.txt \n");
            printf("6- Save And Quit\n");
                printf("\nEnter The Number Of Choice:");
            scanf("%d",&ch);

    switch(ch)

    {

    case 1:
        printf("\nEnter The Data You Want To Add :");
       scanf("\n \n %[^\n]s",X);
        Push(S,X);
        printf("The Data Added Sucessfully\n");
        break;
    case 2:
        PopUn(S,S2);
        break;
    case 3:
         PopRe(S2,S);
         break;

    case 4:
        Display(S);
         break;

    case 5:
        WriteToFile(S);
        break;

    case 6:
        WriteToFile(S);
        printf("Good Bye :)\n");
        exit (0);

    default:
    printf("Invalid Choice , Try Again\n");
    }
    }
}



//Undo ----------------------------------
void PopUn( Stack S, Stack S2 ){
Mynode firstCell;
Stack head = S;
char X[100];
if( IsEmpty( S ) ) {
printf( "Empty stack" );
return;
}
else{
firstCell = head->Next;
// transfer content main stack to the second
Push (S2,firstCell->info);
strcpy(X,firstCell->info);
head->Next = head->Next->Next;
free( firstCell );
printf("This Value Deleted:%s\n",X);
}
}
// Redo -------------------------------
void PopRe( Stack S2, Stack S ){
Mynode firstCell;
Stack head = S2;
char X[100];
if( IsEmpty( S2 ) ) {
printf( "Empty stack" );
return;
}
else{
firstCell = head->Next;
// transfer content main stack to the second
Push (S,firstCell->info);
strcpy(X,firstCell->info);
head->Next = head->Next->Next;
free( firstCell );
printf("This Value Reset To Stack:%s\n",X);
}
}
//------------------------------------------------
void Pop( Stack S ){
Mynode firstCell;
Stack head = S;
char X[100];
if( IsEmpty( S ) ) {
printf( "Empty stack" );
return;
}
else{
firstCell = head->Next;
strcpy(X,head->info);
head->Next = head->Next->Next;
free( firstCell );
printf("This Value Deleted:%s\n",X);
}
}

//----------------------------------------------------
int IsEmpty( Stack S ){
return S->Next == NULL;
}

void MakeEmpty( Stack S ){
if( S == NULL )
printf( "Out of space!" );
else
while( !IsEmpty( S ))
Pop( S );
}
//-----------------------------------------
Stack CreateStack( Stack S){
S = (struct node*)malloc( sizeof( struct node ) );
S->Next = NULL;
return S;
}
//---------------------------------------
void Push(Stack S,char X[100]){
Stack head=S;
Mynode part;
part = (Stack)malloc( sizeof( struct node ) );
if( part == NULL){
printf( "Out of space!" );
return;
}
else{
strcpy(part->info,X);
part->Next = head->Next;
head->Next=part;
}
}
//-----------------------------
void Display(Stack S){
    Mynode p=S;
    if (IsEmpty(S))
    {
        printf("Stack Is Empty !\n");
        return;
    }
   while (p->Next!=NULL)
   {
       p=p->Next;
       printf("MyCommand> %s\n\n",p->info);

   }
}
void WriteToFile(Stack S){
    FILE *fout;
    fout=fopen("results.txt","w+");
    Mynode p1=S;
    p1=p1->Next;
    if (!p1) {
        fprintf(fout,"this Stack is Empty!\n");
    }
    else{
        while(p1){
            fprintf(fout,"MyCommand> %s\n\n",p1->info);
            p1=p1->Next;

        }
    }
    fclose(fout);



}










