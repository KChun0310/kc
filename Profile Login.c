#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHAR 101 /* 100 character + 1 null terminator */

typedef struct
{
	char username[MAX_CHAR];
	char password[MAX_CHAR];
	int pin;
}SECURITY;

typedef struct
{
	char name[MAX_CHAR]; /* Real name */
	char ID[MAX_CHAR]; /* Matric ID */
	int category; /* 1: diploma, 2: degree, 3: master, 4: phd */
	char program_name[MAX_CHAR]; /* Programme name */
	char program_code[MAX_CHAR]; /* example UR6521001 */
	int intake[2]; /* intake year */
}INFO;

typedef struct
{
	SECURITY login;
	INFO information;
}PROFILE;



void LogIn(PROFILE *user);

void SignUp(PROFILE *user);

int SaveFile(PROFILE *user);

int LoadFile(PROFILE *user, const char *filename);

int Verification(PROFILE *user, int trial);

void Clean(void);

int main()
{
	PROFILE user;
	//login or sign up choice
	int decision;
	
	printf("1) Log In\n2) Sign Up\n");
	printf("Please Choose : ");
	while(scanf("%d", &decision) != 1)
	{
		Clean();
		printf("\nIdiots ONLY NUMBERS\n\n");
		system("PAUSE");
		system("CLS");
		printf("1) Log In\n2) Sign Up\n");
		printf("Please Choose : ");
	}
	Clean();
	switch(decision)
	{
		case 1 :
		{
			LogIn(&user);
			break;
		}
		case 2 :
		{
			SignUp(&user);
			break;
		}
		default :
		{
			printf("Stupid Out Of Choice\n");
			system("PAUSE");
			system("CLS");
			main();
			break;
		}
	}
	
	return 0;
}

void LogIn(PROFILE *user)
{
	char temp_name[MAX_CHAR];
	//Input username
	printf("Username : ");
	fgets(temp_name, MAX_CHAR, stdin);
	temp_name[strcspn(temp_name, "\n")] = '\0';
	snprintf(temp_name, sizeof(temp_name), "%s.dat", temp_name);
	
	//Check file exist
	if(LoadFile(user, temp_name) == 1) /* Username exist */
	{
		if(Verification(user, 0) == 0)/* Fail to verify */
		{
			//Freeze
			for(int second = 180 ; second >= 0 ; second--)
			{
				printf("\rFreeze Time Left : %d s", second);
				sleep(1);
			}
			system("CLS");
			main(); /* Fail login repeat the program */
		}
	}
	else
	{
		printf("Username Does Not Exist\n");
		system("PAUSE");
		system("CLS");
		main(); /* Fail login repeat the program */
	}
}

void SignUp(PROFILE *user)
{
	char confirm_pass[MAX_CHAR];
	int retype;
	//Sign in security process
	do
	{	
		//Settings username
		printf("Username : ");
		fgets(user->login.username, MAX_CHAR, stdin);
		user->login.username[strcspn(user->login.username, "\n")] = '\0';
		//Setting password
		printf("Password : ");
		fgets(user->login.password, MAX_CHAR, stdin);
		printf("Confirm Password : ");
		fgets(confirm_pass, MAX_CHAR, stdin);
		if(strcmp(user->login.password, confirm_pass) == 0)
		{
			user->login.password[strcspn(user->login.password, "\n")] = '\0';
			//Setting pin
			printf("Pin(Only Numbers) : ");
			while(scanf("%d", &user->login.pin) != 1)
			{
				Clean();
				printf("\nIdiots ONLY NUMBERS\n\n");
				system("PAUSE");
				printf("Pin(Only Numbers) : ");
			}
			Clean();
			retype = 0;
		}
		else
		{
			//Do again
			retype = 1;
			printf("Password Not Match Please Retype\n");
		}
	}while(retype == 1);
	printf("Security Settings Completed\n\n");
	
	//Building profile info
	//Name
	printf("Your Name : ");
	fgets(user->information.name, MAX_CHAR, stdin);
	user->information.name[strcspn(user->information.name, "\n")] = '\0';
	//ID
	printf("Your ID : ");
	fgets(user->information.ID, MAX_CHAR, stdin);
	user->information.ID[strcspn(user->information.ID, "\n")] = '\0';
	//Category
	printf("Category\n1) Diploma\n2) Degree\n3) Master\n4) PhD\nYou Are ... ");
	while(scanf("%d", &user->information.category) != 1)
	{
		Clean();
		printf("\nIdiots ONLY NUMBERS\n\n");
		system("PAUSE");
		printf("Category\n1) Diploma\n2) Degree\n3) Master\n4) PhD\nYou Are ... ");
	}
	Clean();
	//Programme name
	printf("Programme Name : ");
	fgets(user->information.program_name, MAX_CHAR, stdin);
	user->information.program_name[strcspn(user->information.program_name, "\n")] = '\0';
	//Programme code
	printf("Programme Code : ");
	fgets(user->information.program_code, MAX_CHAR, stdin);
	user->information.program_code[strcspn(user->information.program_code, "\n")] = '\0';
	//Intake year
	printf("Intake Year : From ");
	while(scanf("%d", &user->information.intake[0]) != 1)
	{
		Clean();
		printf("\nIdiots ONLY NUMBERS\n\n");
		system("PAUSE");
		printf("Intake Year : From ");
	}
	Clean();
	printf(" To ");
	while(scanf("%d", &user->information.intake[1]) != 1)
	{
		Clean();
		printf("\nIdiots ONLY NUMBERS\n\n");
		system("PAUSE");
		printf("Intake Year : From %d To ", user->information.intake[0]);
	}
	Clean();
	printf("\nProfile Settings Completed\n");
	
	//Save file
	if(SaveFile(user) == 1) /* Success saving */
	{
		printf("\nNew User is Registered Successfully!!! Please Return To Log In Page and LogIn\n");
		system("PAUSE");
		system("CLS");
		main();
	}
	else
	{
		printf("\nUser existed or file save unsuccessfully!!! Please Return To Log In Page\n");
		system("PAUSE");
		system("CLS");
		main();
	}
}

int SaveFile(PROFILE *user)
{
	//Save new file
	char filename[MAX_CHAR + 4];
	snprintf(filename, sizeof(filename), "%s.dat", user->login.username); /* Input the filename with .dat extension */
	
	//Create and open file for writing(if not exist)
	if(fopen(filename, "rb") != NULL)
	{
		printf("Username: %s existed, please try other one\n", user->login.username);
		return 0; //Exist
	}
	
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL)
    {
    	perror("Error opening file\n");  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 Need help !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    	fprintf(stderr, "Error opening file: %s\n", filename);
       	return 0; /* 0 means error */
    }
    printf("File '%s' created successfully.\n", filename);
    
	//Saving process
    fwrite(user, sizeof(PROFILE), 1, fp);
    
	//Close the file
	fclose(fp);
	return 1; //Success
}

int LoadFile(PROFILE *user, const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", filename);
		return 0; /* 0 means error */
	}
	
	//Loading process
	fread(user, sizeof(PROFILE), 1, fp);
	
	//Close the file
	fclose(fp);
	printf("File '%s' loaded successfully.\n", filename);
	return 1; //Success
}

int Verification(PROFILE *user, int trial)
{
	int choice;
	printf("Choices\n1. Password\n2. Pin number\nYour choice : ");
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:
		{
			char temp_pass[MAX_CHAR];
			//Input password	
			printf("Password : ");
			fgets(temp_pass, MAX_CHAR, stdin);
			temp_pass[strcspn(temp_pass, "\n")] = '\0';
			
			if(strcmp(temp_pass, user->login.password) == 0)
			{
				printf("Welcome %s", user->login.username);
				return 1;
			}
			else
			{
				trial += 1;
				printf("Incorrect pin number. You can %d more trials", 3-trial);
				if(trial < 3) /* Still can try */
					return Verification(user, trial);
				else
					return 0; /* No more trials, freeze */
			}
		}
		case 2:
		{
			int temp_pin;
			//Input pin
			do
			{
				printf("Pin : ");
				fflush(stdin);
			}while(scanf("%d", &temp_pin) != 1);
			
			printf("\n");
			
			if(temp_pin == user->login.pin)
			{
				printf("Welcome %s", user->login.username);
				return 1;
			}
			else
			{
				trial += 1;
				printf("Incorrect pin number. You can %d more trials", 3-trial);
				if(trial < 3) /* Still can try */
					return Verification(user, trial);
				else
					return 0; /* No more trials, freeze */
			}
		}
		default :
		{
			printf("OUT OF CHOICE !!!\n");	
			return Verification(user, trial);
		}
	}	
}

void Clean(void)
{
	while (getchar() != '\n' && getchar() != EOF); /* Use while loop to clean */
}
