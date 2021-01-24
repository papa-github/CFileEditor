#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>

void introduction();                                                    //Some functions that will be called before they are defined
int lengthoffile();                                                     //Fixes implicit declaration of function warning
int numberoflines();
void recordchangelog();

// Create File function
void createfile(){
    printf("Create File \n");
    printf("------------------------------------------------------\n");
    printf("Enter file name (max 20 characters) \n");                   // Takes filename from user
    char filename[21];                                                  //Assigns name of file to 'filename' variable
    scanf(" %[^\n]%*c",filename);                                       //User input for filename
    if( access( filename, F_OK ) == 0 ) {                               //Checks if file with filename already exists
        printf("A file already exists with this name\n");               //IF YES - Displays to user that a file already exists with this name
        createfile();                                                   //       - Loops the function back to the beginning
        return;                                                         //       - Returns out of function after file has been created
    } else {                                                            //IF NOT
        FILE *fp;
        fp = fopen (filename, "w");                                     //This has the same effect as creating the file with the name 'filename'
        fclose (fp);                                                    //Closes the file
    }
    recordchangelog("Create File", filename);                           //Records Create File operation on 'filename' in changelog
}

// Copy File function
void copyfile(){
    printf("Copy File \n");
    printf("------------------------------------------------------\n");
    FILE *fp1, *fp2; 
    printf("Enter the existing file name (max 20 characters) \n");          // Takes existing filename from user and assigns it to 'filename'
    char filename[21], c;  
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                                   //File Check
        printf("This file does not exists\n");
        copyfile();
        return;
    }
    printf("Specify new file (max 20 characters)\n");                       //Takes new filename from user and assigns it to 'filename1'
    char filename1[21];
    scanf(" %[^\n]%*c",filename1);
    if( access( filename1, F_OK ) == 0 ) {                                  //File Check
        printf("This file already exists\n");
        copyfile();
        return;
    } else {
        fp1 = fopen(filename, "r");                                         //Opens existing file in read mode
        if (fp1 == NULL) {                                                  //Check if fopen was successful
            printf("%s cannot be opened \n", filename);                     //If not - it throws the error
            printf("Please check whether you have read permissions.\n");    // - (and suggests a potential reason for it)
            fclose(fp1);                                                    //closes the file 
            return;                                                         //returns back to the menu
        }
        fp2 = fopen(filename1, "w");                                        //Creates new file with name 'filename1'
        if (fp2 == NULL) {                                                  //Check if fopen was successful
            printf("%s cannot be opened \n", filename1);
            printf("Please check whether you have write permissions.\n"); 
            fclose(fp1);
            fclose(fp2);
            return;
        } 
  
        c = fgetc(fp1);                                                    
        while (c != EOF) {                                                 //Traverses through file
            fputc(c, fp2);                                                 //Copies file character by character
            c = fgetc(fp1); 
        }
        printf("%s has been copied to %s\n",filename,filename1);          //Returns success message
        fclose(fp1);                                                      //Closes both files
        fclose(fp2);
    }
    recordchangelog("Copy File", filename);                               //Records Copy File operation on 'filename' in changelog
}

// Delete File function
void deletefile(){
    printf("Delete File \n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");
    char filename[21], c;                                                 // Takes filename of file to be deleted
    scanf(" %[^\n]%*c",filename);
    if (remove(filename) == 0){                                           // The remove function returns a 0 on successful file deletion
       printf("%s has been deleted successfully\n", filename);            // - If 0 is returned - display success message
   }else{
       printf("%d could not be deleted\n", filename);                             // - If not - display error message
       return;
   }
   recordchangelog("Delete File", filename);                              // Records Delete File operation on 'filename' in changelog
}

// Show File function
void showfile(){
    printf("Show File \n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");      //Takes filename of file to be operated on
    char filename[21], c;                                               
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                               //File check
        printf("This file does not exists\n");
        showfile();
        return;
    }else {
        FILE * fp;
        fp = fopen(filename, "r");                                          //Opens file in readmode
        if(fp == NULL){                                                     //Check if fopen was successful
            printf("%s cannot be opened.\n", filename);
            printf("Please check whether you have read permissions.\n");
            fclose(fp);
            return;
        }
        printf("Start of File\n");                                          //States File reading has started
        c = fgetc(fp);                                                       
        while (c != EOF) {                                                  //Traverses through file
           putchar(c);                                                      //Prints file to console character by character
            c = fgetc(fp);
        }
        printf("\nEnd of File\n");                                          //States file reading has ended
        fclose(fp);                                                         //Closes file
    }
    recordchangelog("Show File", filename);                                 //Records Delete File operation on 'filename' in changelog
}

//Append Line function
void appendline(){
    printf("Append Line\n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");      //Takes filename of file to be operated on
    char filename[21];                                                  
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                               //File check
        printf("This file does not exists\n");
        appendline();
        return;
    }else {
        FILE * fp;
        printf("Enter the string to be added (max 50 characters) \n");  //Takes user's input string and assigns it to 'userstring'
        char userstring[51];
        scanf(" %[^\n]%*c",userstring);
        fp = fopen(filename, "a");                                       //Opens file in append mode
        if(fp == NULL){                                                  //Checks if fopen was successful
            printf("%s cannot be opened.\n", filename);
            printf("Please check whether you have write permissions.\n");
            fclose(fp);
            return;
        }

        if (lengthoffile(filename) != 0){                                //If the file is blank, add string to the first line
            fputc('\n',fp);                                              // - If not then create a new line and add string there
        }
        
        fputs(userstring, fp);                                           //Command to add the user input string
        fclose(fp);                                                      // Close the file
        printf("Line appended to %s \n", filename);                      // Display the success message
    }
    recordchangelog("Append Line", filename);                            //Records Append Line operation on 'filename' in changelog

}

//Delete Line function
void deleteline(){
    printf("Delete Line\n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");      //Takes filename of file to be operated on
    char filename[21], c;
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                               //File Check
        printf("This file does not exists\n");
        deleteline();
        return;
    }else {
        FILE *fp1, *fp2;
        int line, currentline =  1;                                     //Initalises 'line' and 'currentline' integers

        if (lengthoffile(filename) == 0){                                                              //If the file is blank (empty file),
            printf("%s is empty. Please use Delete File if you want to delete the file \n", filename); // - the program suggests that the Delete File function is used instead
            return;                                                                                    // - And returns to the menu
        }
        
        printf(" \n Enter line number of the line to be deleted :");            //Takes user input on which line is to be deleted and assigns in to 'line'
        scanf(" %d", &line);
        while (line < 0 || line > numberoflines(filename)){                     //Loops the request for a line until a valid line (between 0 and the number of lines in the file) is chosen
            printf("The file only has %d lines.\n", numberoflines(filename));
            printf(" \n Enter line number of the line to be deleted :");
            scanf(" %d", &line);
        }

        fp1 = fopen(filename, "r");                                             //Opens the file in read mode
        if(fp1 == NULL){                                                        //Check if fopen was successful
            printf("%s cannot be opened.\n", filename);
            printf("Please check whether you have write permissions.\n");
            fclose(fp1);
            return;
        }
        fp2 = fopen("temp", "w");                                               //Creates a temporary file called 'temp'
        
        
        c = getc(fp1);
        while (c != EOF)                                                        //Traverses through file
        {
            if (c == '\n'){
                currentline++;                                                  //Increments 'currentline' when newline character is encountered
            }
            if (currentline != line){                                           //Copies file to 'temp' unless the file is on the same line - resulting in the content of the specified being deleted
                putc(c, fp2);
            }
            c = getc(fp1);                                                      // Gets next character of original file
        }
        fclose(fp1);                                                            //Closes both files
        fclose(fp2);
        remove(filename);                                                       //Removes original file
        rename("temp",filename);                                                //Renames 'temp' to the original file name
        printf("Line %d has been deleted\n", line);                             //Displays success message
    }
    getchar(); //clears input buffer                                            //Clears the input buffer (using scanf("%d") leaves a '\n' in the buffer)
    recordchangelog("Delete Line", filename);                                    //Records Delete Line operation on 'filename' in changelog

}

//Insert Line function
void insertline(){
    printf("Insert Line\n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");              //Takes filename of file to be operated on
    char filename[21], c;
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                                       //File Check
        printf("This file does not exists\n");
        insertline();
        return;
    }else {
        FILE *fp1, *fp2;
        printf("Enter the string to be added (max 50 characters) \n");          //Takes user's input string and assigns it to 'userstring'
        char userstring[51];
        scanf(" %[^\n]%*c",userstring);
        int line, currentline =  1;                                             //Initalises 'line' and 'currentline' integers
        
        printf(" \n Enter line number of the line to be inserted\n");           //Takes the line number that the content will be inserted at and assigns it to 'line'
        scanf(" %d", &line);
        
        if (line == numberoflines(filename) + 1){                               //If the user wishes to add a string to the end of file - they are transferred to the Append Lines function
            printf("To add a line at the end of the file use Append Lines\n");
            printf("Transferring to Append Line\n");
            appendline();
            return;
        }
        if (line <= 0 || line > numberoflines(filename)){                       //If the user specifies a number 'out of bounds' 
            printf("The file only has %d lines \n", numberoflines(filename));   //- They are told the number of lines in the file
            insertline();                                                       //- Then they are taken back to the start of the function
            return;
        }
        
        fp1 = fopen(filename, "r");                                      //Opens file in read mode
        if(fp1 == NULL){                                                 //Check if fopen was successful
            printf("%s cannot be opened.\n", filename);
            printf("Please check whether you have read permissions.\n");
            fclose(fp1);
            return;
        }
        fp2 = fopen("temp", "w");                                        //Creates a temporary file called 'temp'
        
        if(line == 1){                                                   //Special Case - The user wants to add content at the start of the file
            fputs(userstring, fp2);                                      //Places the newstring on the first line
            putc('\n',fp2);                                              //moves to the second line
            currentline++;                                               //Iterates the 'currentline'
        }                                                                //and then copies the original file character for character (See below)
        
        c = getc(fp1);
        while (c != EOF)                                                 //Traverses through file
        {
            if (c == '\n'){
                currentline++;                                           //Increments 'currentline' when newline character is encountered
            }
            if (currentline != line){                                    //If the character is not on the specified line then just copy the file character by character to 'temp'
                putc(c, fp2);
            }else{                                                       //If it is{
                putc('\n',fp2);                                          //- Places 'c' (which is a newline) on 'temp' allowing the line to be moved onto the actual requested line
                fputs(userstring, fp2);                                  //- Places the userstring on the line
                putc('\n',fp2);                                          //- Places newline character to change to the next line
                currentline++;                                           //- Increments 'currentline'
            }
            c = getc(fp1);                                               //Get the next character
        }
        fclose(fp1);                                                     //Closes both files
        fclose(fp2);
        remove(filename);                                               //Removes the original file
        rename("temp",filename);                                        //Renames 'temp' to the original filename
        printf("A line  has been inserted at line %d\n", line);         //Displays success message
    }
    getchar();                                                          //Clear Buffer
    recordchangelog("Insert Line", filename);                           //Records Insert Line operation on 'filename' in changelog

}

// Show Line function
void showline(){
    printf("Show Line\n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");               //Takes filename of file to be operated on
    char filename[21], c;
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                                        //File Check
        printf("This file does not exists\n");
        showline();
        return;
    }else {
        FILE *fp1;
        int line, currentline =  1;                                              //Initalises 'line' and 'currentline' integers
        printf(" \n Enter line number of the line to shown:");
        scanf(" %d", &line);                                                     //Takes user input on which line is to be shown and assigns in to 'line'
        if (line <= 0 || line > numberoflines(filename)){                        //If the user picks an invalid line the function is called again
            printf("The file only has %d lines \n", numberoflines(filename));
            showline();
            return;
        }

        fp1 = fopen(filename, "r");                                 //Opens file in read mode
        if(fp1 == NULL){                                            //Check if fopen was successful
            printf("%s cannot be opened.\n", filename);
            printf("Please check whether you have read permissions.\n");
            fclose(fp1);
            return;
        }

        c = getc(fp1);
        while (c != EOF)                                           //Traverses through file
        {
            if (c == '\n'){
                currentline++;                                     //Increments 'currentline' when newline character is encountered
            }
            if (currentline == line){                              //If  'c' is on the user-specified line then each character is printed to the console
                putchar(c);
            }
            c = getc(fp1);                                         //Gets the next character
        }
        fclose(fp1);                                               //Closes file
    }
    printf("\n");
    getchar();                                                     //Clear Buffer
    recordchangelog("Show Line", filename);                        //Records Show Line operation on 'filename' in changelog

}

// Show Number of Lines function
void shownumberoflines(){
    printf("Show Number of Lines\n");
    printf("------------------------------------------------------\n");
    printf("Enter the existing file name (max 20 characters) \n");                    //Takes filename of file to be operated on
    char filename[21];
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                                             //File Check
        printf("This file does not exists\n");
        shownumberoflines();
        return;
    }else {
        printf("The file has %d lines \n", numberoflines(filename));                 //Displays number of lines in file by calling the numberoflines function
    }
    recordchangelog("Show number of lines", filename);                               //Records Show number of lines operation on 'filename' in changelog

}

// Length of File function - used mainly to determine whether a file is blank(where it returns 0) or not
int lengthoffile(char filename[21]){
    FILE *fp1;
    char c;
    int length =  0;
    fp1 = fopen(filename, "r");                                                     //Opens file in read mode
    if(fp1 == NULL){                                                                //Check if fopen was successful
        printf("%s cannot be opened.\n", filename);
        printf("Please check whether you have read permissions.\n");
        fclose(fp1);
        return 0;
    }
    c = getc(fp1);
    while (c != EOF){                                                               //Traverses through file
        length++;                                                                   //Increments length for each character
        c = getc(fp1);                                                              //Moves onto the next character
    }
    fclose(fp1);                                                                    //Closes file
    return length;                                                                  //Returns the number of characters (the length) of the file
}

// Number of Lines function - Used in a variety of functions to determine the number of lines in the file
int numberoflines(char filename[21]){
    FILE *fp1;
    char c;
    int currentline =  1;                                               //Initialised line counter
    fp1 = fopen(filename, "r");                                         //Opens file in append mode
    if(fp1 == NULL){                                                    //Check if fopen was successful
        printf("%s cannot be opened.\n", filename); 
        printf("Please check whether you have read permissions.\n");
        fclose(fp1);
        return 0;
    }
    c = getc(fp1);
    while (c != EOF) {                                                  //Traverses through file
        if (c == '\n'){
            currentline++;                                              //Increments 'currentline' when newline character is encountered
        }
        c = getc(fp1);
    }
    fclose(fp1);                                                        //Closes file
    return currentline;                                                 //Returns the number of lines in the file
}

// Record Changelog function - Used to write to changelog.txt
void recordchangelog(char *function,char filename[21]){
    FILE* fp;
    time_t now;                                                             //Initalises current time - seemed like a useful addition to the changelog - allowing the user to the time at which               
    time(&now);                                                             // - operations were carried out using the program
    if( access( "changelog.txt", F_OK ) != 0 ) {                            //If 'changelog.txt' does not exist
        printf("Creating changelog.txt\n");                                 //- Displays message stating 'changelog.txt' is being created
        fp = fopen("changelog.txt", "w");                                   //- Creates a file called 'changelog.txt'
        fclose(fp);                                                         //- Closes file
    }
    fp = fopen("changelog.txt", "a");                                       //Opens file in append mode
    if (function == "main"){                                                //When the program begins the following code is carried out
        fputs("---------------------------------------------------", fp);
        fputc('\n', fp);
        fputs("Program initialised at: ", fp);                              //States that the program has been initialised
        fputs(ctime(&now), fp);                                             //Adds the time of initialisation
        fputc('\n', fp);
        return;                                                             //returns out of the function
    }

    //The following code is carried out after an operation has been completed 
    fputs(ctime(&now), fp);                                      // Records current time to the changelog (includes a newline character at the end, so one does not have to be added)
    fputs("          ", fp);                                     // These spaces make the changelog easier to read
    fputs("Using ", fp);                                         // The following code records the function used and the file it was used on to changelog.txt
    fputs(function, fp);
    if (function != "Show Changelog"){                          //If applicable the file the operation was carried out on is listed
        fputs(" function on ", fp);
        fputs(filename, fp);
        if (function != "Delete File"){                         // If applicable (if the function was not Delete File or Show Changelog) the new number of lines the file has is added
            fputs(" [", fp);
            fputs("File now has ", fp);
            char lines[20];
            sprintf(lines, "%d", numberoflines(filename));
            fputs(lines, fp);
            fputs(" lines", fp);
            fputs("]", fp);
        }
    }
    fputc('\n', fp);
    fflush(fp); //Flushes the I/O buffer - Allows changelog.txt to be updated whilst program is running
}

// Show Changelog function - This is basically the Show File function, with 'changelog.txt' as the filename
void showchangelog(){
    printf("Show Changelog\n");
    printf("------------------------------------------------------\n");
    char filename[21] = "changelog.txt", c;
    FILE * fp;
    fp = fopen(filename, "r");                                           //Opens file in read mode
    if(fp == NULL){                                                      //Check if fopen was successful
        printf("%s cannot be opened.\n", filename);
        printf("Please check whether you have read permissions.\n");
        fclose(fp);
        return;
    }
    printf("Start of File\n");
    c = fgetc(fp); 
    while (c != EOF) {                                                   //Traverses through file
        putchar(c);                                                      //Prints character to console
        c = fgetc(fp);                                                   //Moves onto the next character
    }
    printf("\nEnd of File\n");
    fclose(fp);                                                          //Closes file
    recordchangelog("Show Changelog", "n/a");                            //Records Show Changelog operation in changelog

}

// Clear Changelog function - included as an extension to the General Operations
// The changelog can quickly become filled, so this function was included to allow the user to quickly
// clear the changelog. Could be useful to use if the program is moved to a different folder or system so that no
// longer relevant information are deleted from the changelog
void clearchangelog(){
    if( access("changelog.txt", F_OK ) != 0 ) {                         //File Check
        printf("This file does not exists\n");
        return;
    }else {
        fclose(fopen("changelog.txt", "w"));                            //Opens, then clears, then closes changelog.txt
        printf("The changelog has been cleared\n");                     //Displays success message
    }
}

// Rename File function - Included as an extension to the 4 File Operations
// THis was included as it is a common feature of many text and file editors 
// and seemed to complement the 4 existing file operations
void renamefile(){
    printf("Rename File");
    printf("------------------------------------------------------\n");
    char filename[21], filename1[21];
    printf("Enter the existing file name (max 20 characters) \n");              //Takes filename of file to be operated on
    scanf(" %[^\n]%*c",filename);
    if( access( filename, F_OK ) != 0 ) {                                       //File Check
        printf("This file does not exists\n");
        renamefile();
        return;
    }else {
        printf("Enter the new file name (max 20 characters) \n");              //Takes new filename of file
        scanf(" %[^\n]%*c",filename1);
    }
    
    if (rename(filename, filename1) == 0){                                     //Renames the file; much like remove(), rename() also returns 0 on a successful operation
        printf("%s successfully renamed to %s\n", filename, filename1);        //If 0 is returned - Displays success message
    }
    else
    {
        printf("Cannot rename %s. Please check if you have permissions to modify %s.\n", filename, filename); // Else - Displays error message
    }

    recordchangelog("Rename file", filename1);                       //Records Rename File operation on 'filename1' in changelog
}

//Main Menu - This prints the introduction (and main menu) of the program
void introduction(){
    int n;
    printf("Welcome to the program, please choose from the options below \n") ;                                          // All the options available are listed
    printf("-------------------------------------------------------------\n");
    printf("File Operations:\n") ;
    printf("-------------------------------------------------------------\n");
    printf("  (1) Create File - Create a new file with a specified name.\n");
    printf("  (2) Copy File - Create a new file with a specified name and identical contents to an existing file.\n") ;
    printf("  (3) Delete File - Delete an existing file with a specified name.\n") ;
    printf("  (4) Show File - Display the contents of an existing file with a specified name.\n") ;
    printf("--------------------------------------------------------------\n");
    printf("Line Operations:\n") ;
    printf("--------------------------------------------------------------\n");
    printf("  (5) Append Line - Create a new line of content at the end of a specified file.\n") ;
    printf("  (6) Delete Line - Delete a line of content at a particular line number in a specified file.\n") ;
    printf("  (7) Insert Line - Create a new line of content at a particular line number in a specified file.\n") ;
    printf("  (8) Show Line - Display the contents of a file at a particular line number in a specified file.\n") ;
    printf("--------------------------------------------------------------\n");
    printf("General Operations:\n") ;
    printf("--------------------------------------------------------------\n");
    printf("  (9) Show Change Log- Display the sequence of operations performed on all files, including the number of lines following each operation\n") ;
    printf("  (10) Show Number of Lines- Show the number of lines in a specified file.\n") ;
    printf("--------------------------------------------------------------\n");
    printf("Extra Operations\n");
    printf("  (11) Clear changelog - Clears the changelog.txt file\n") ;
    printf("  (12) Rename file - Change the name of a specified file.\n") ;
    printf("--------------------------------------------------------------\n");
    printf("Enter 0 to exit\n");


    printf("Enter option \n") ;         // Asks for user input for the number corresponding to which operation they want to carry out
	scanf("%d", &n);                    // Assigns user input to variable 'n' - to be used in switch statement to send user to the correct function
    int c;
    while ( (c = getchar()) != '\n' && c != EOF ) { } //Clears input buffer (implemented as the above scanf("%d") caused issues when the user was next asked for an input)
    
    //A switch case to send the user to the currect function
    switch (n){                                       
        case 0:exit(0);
        case 1:createfile();
        break;
        case 2:copyfile();
        break;
        case 3:deletefile();
        break;
        case 4:showfile();
        break;
        case 5:appendline();
        break;
        case 6:deleteline();
        break;
        case 7:insertline(); 
        break;
        case 8:showline();
        break;
        case 9:showchangelog();
        break;
        case 10:shownumberoflines();
        break;
        case 11:clearchangelog();
        break;
        case 12: renamefile();
        break;
        default: printf("Please pick an option from 1-10\n"); //Used if the user picks an invalid option (Anything that is not an integer from 1 to 10 )
    }

}

int main(){
    recordchangelog("main", "n/a");                                  //Records Program Initialisation in changelog
    introduction();                                                  //Initialises introduction function
    
    while (1){
        printf("Press enter to return to menu\n");                   //The program arrives here after a operation has been completed
        char enter = 0;                                              
        while (enter != '\r' && enter != '\n') { enter = getchar(); }//The program waits until the user presses the enter button to return to the main menu
        introduction();                                              //This prevents the user from having to restart the program if they wish to carry out multiple operations on a file
    }
} 