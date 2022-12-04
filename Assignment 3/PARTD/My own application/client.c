
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8888
#define SIZE 80

int clientSocket;

void displayMenu();

void userAdd(){
	char uname[SIZE];
	int value=0;
	printf("Enter Username\n");
	scanf("%s",uname);
	send(clientSocket,uname,sizeof(uname),0);
	recv(clientSocket,&value,sizeof(value),0); //set flag value also there on server
	if(value==0){
		char pwd[SIZE];
		int ch; int amt=0;
		printf("Enter your Password \n");
		scanf("%s",pwd);
		send(clientSocket,pwd,sizeof(pwd),0);
		printf("Enter user type(Normal-0/Joint-1)\n");
		scanf("%d",&ch);
		send(clientSocket,&ch,sizeof(ch),0);
		if(ch == 0){
			printf("Enter the initial deposit\n");
			scanf("%d",&amt);
			send(clientSocket,&amt,sizeof(amt),0);
		}
		else if(ch == 1){
			char uname1[SIZE];
			printf("Enter the Second Username\n");
			scanf("%s",uname1);
			send(clientSocket,uname1,sizeof(uname1),0);
			printf("Enter deposit Amount\n");
			scanf("%d",&amt);
			send(clientSocket,&amt,sizeof(amt),0); //handle this on server
		}
		printf("User added successfully!!!\n");
		displayMenu();
	}else{
		printf("User already there in DB !!!\n");
		displayMenu();
	}
}

void deleteUser(){
	char uname[SIZE], pwd[SIZE];
	int value=0;
	printf("Enter Username you want to delete\n");
	scanf("%s",uname);
	printf("Enter Password for above Username\n");
	scanf("%s",pwd);
	send(clientSocket,uname,sizeof(uname),0);
	recv(clientSocket,&value,sizeof(value),0);
	if(value>0){
		send(clientSocket,pwd,sizeof(pwd),0);
		int flag=1;
		recv(clientSocket,&flag,sizeof(flag),0);
		if(flag == 0){
			printf("Record deleted successfully!!!\n");
			displayMenu();
		}else{
			printf("You have entered wrong password!!!\n");
			displayMenu();
		}
	}else{
		printf("User not found to erase!!!\n");
		displayMenu();
	}
}

void modifyUser(char uname[SIZE]){
	char pwd[SIZE];
	int value=0,amt,withdraw=1;
	recv(clientSocket,&value,sizeof(value),0);
	if(value>0){
		int ch;
		printf("Enter from following : \n");
		printf("1. Password change \n");
		printf("2. Amount withdraw \n");
		printf("3. Amount deposit \n");
		scanf("%d", &ch);
		send(clientSocket,&ch,sizeof(ch),0);
		if(value==2){
			switch(ch){
				case 1 : 	printf("Enter new Password for %s \n",uname);
							scanf("%s",pwd);
							send(clientSocket,pwd,sizeof(pwd),0);
							printf("Password updated successfully !!! \n");
							displayMenu();
							break;
				case 2 :	printf("Enter the Amount to withdraw \n");
							scanf("%d",&amt);
							send(clientSocket,&amt,sizeof(amt),0);
							recv(clientSocket,&withdraw,sizeof(withdraw),0);
							if(!withdraw){
								printf("Amount deducted successfully\n");
							}else{
								printf("You've insufficient balance!!!\n");
							}
							displayMenu();
							break;
				case 3 : 	printf("Enter the Amount to deposit\n");
							scanf("%d",&amt);
							send(clientSocket,&amt,sizeof(amt),0);
							printf("Amount deposited successfully \n");
							displayMenu();
							break;
				default:  	displayMenu();
							break;	
			}

		}else if(value == 3 || value == 4){ //implement locking mechanism in this
			switch(ch){
				case 1 : 	printf("Enter new Password for %s\n",uname);
							scanf("%s",pwd);
							send(clientSocket,pwd,sizeof(pwd),0);
							printf("Password updated successfully!!! \n");
							displayMenu();
							break;
				case 2 :	printf("Enter the Amount to withdraw\n");
							scanf("%d",&amt);
							send(clientSocket,&amt,sizeof(amt),0);
							recv(clientSocket,&withdraw,sizeof(withdraw),0);
							if(!withdraw){
								printf("Amount deducted successfully\n");
							}else{
								printf("You've insufficient balance!!!\n");
							}
							displayMenu();
							break;
				case 3 : 	printf("Enter the Amount to deposit\n");
							scanf("%d",&amt);
							send(clientSocket,&amt,sizeof(amt),0);
							printf("Amount deposited successfully\n");
							displayMenu();
							break;
				default:  	displayMenu();
							break;

		}
	}
}else{
		printf("User not found to modify!!!\n");
		displayMenu();
	}
}

void searchUser(){
	char uname[SIZE], pwd[SIZE];
	int value=0,amt=0;
	printf("Enter Username you want to search\n");
	scanf("%s",uname);
	send(clientSocket,uname,sizeof(uname),0);
	recv(clientSocket,&value,sizeof(value),0);
	if(value>0){
		int flag = 0;

		recv(clientSocket,&flag,sizeof(flag),0);
		if(flag){
		recv(clientSocket,pwd,sizeof(pwd),0);
		printf("Password for %s is %s\n",uname, pwd);
		recv(clientSocket,&amt,sizeof(amt),0);
		printf("Balance for %s is %d\n",uname, amt);

		printf("Press 1 for main menu()");
		int ch;
		scanf("%d",&ch);
		send(clientSocket,&ch,sizeof(ch),0);
		if(ch == 1)
			displayMenu();
		}else{
			printf("User not found to show!!!\n");
			displayMenu();
		}
	}else{
		printf("User not found to show!!!\n");
		displayMenu();
	}
}

void loginAdminScreen(int op){
	char uname[SIZE];
	switch(op){

		case 1 : userAdd();
				 break;
		case 2 : deleteUser();
				 break;
		case 3 : 
				 printf("Enter Username you want to modify\n");
				 scanf("%s",uname);
				 send(clientSocket,uname,sizeof(uname),0);
				 modifyUser(uname);
				 break;
		case 4 : searchUser();
				 break;
		default: printf("You are redirecting to Main Menu as option doesn't match !!!");
				 displayMenu();
				 break; //may not complete properly
	}
}

void loginAdmin(){
	char uname[SIZE], pwd[SIZE];
	int value=-1;
	printf("Enter admin Username\n");
	scanf("%s",uname);
	send(clientSocket,uname,sizeof(uname),0);
	printf("Enter Password\n");
	scanf("%s",pwd);
	send(clientSocket,pwd,sizeof(pwd),0);
	recv(clientSocket,&value,sizeof(value),0);
	if(value){
		int op;
		printf("\n\nAdmin Setup @ noUse!!!\n\n");
		printf("1: Add User\n");
		printf("2: Delete User\n");
		printf("3: Modify Details of User\n");
		printf("4: Search for...\n");
		printf("Choose option : \n");
		scanf("%d",&op);
		send(clientSocket,&op,sizeof(op),0);
		loginAdminScreen(op);
	}
	else{
		displayMenu();
	}
}

void loginNmlUsr(){
	char uname[SIZE], pwd[SIZE];
	int flag=0;
	int amount;
	printf("\n\nNormal User Login Screen @ noUse!!!\n\n");
	printf("Enter your Username\n");
	scanf("%s",uname);
	printf("Enter your Password\n");
	scanf("%s",pwd);
	send(clientSocket,uname,sizeof(uname),0);
	send(clientSocket,pwd,sizeof(pwd),0);
	recv(clientSocket,&flag,sizeof(flag),0);
	if(flag){
		
		recv(clientSocket,&amount,sizeof(amount),0);
		printf("Your account balance is %d rupees\n", amount);
		modifyUser(uname);	
	}
	else{
		printf("Username/Password doesn't match \n");
		displayMenu();
	}

}

void loginJointAcc(){
	char uname[SIZE], pwd[SIZE];
	int flag=0;
	int amount=0;
	printf("\n\nJoint Account User Login Screen !!!\n\n");
	printf("Enter your Username\n");
	scanf("%s",uname);
	printf("Enter your Password\n");
	scanf("%s",pwd);
	send(clientSocket,uname,sizeof(uname),0);
	send(clientSocket,pwd,sizeof(pwd),0);
	recv(clientSocket,&flag,sizeof(flag),0);
	if(flag){
		
		recv(clientSocket,&amount,sizeof(amount),0);
		printf("Your account balance is %d rupees\n", amount);
		recv(clientSocket,uname,sizeof(uname),0);
		modifyUser(uname);	
	}
	else{
		printf("Username/Password doesn't match\n");
		displayMenu();
	}
}

void displayMenu(){
	int op;
	printf("\n\nLogin/SignUp @ CS301 Bank\n\n");
	printf("1: Admin Login\n");
	printf("2: Normal User Login\n");
	printf("3: Joint Account User Login\n");
	printf("4: Exit\n");
	printf("Choose option :\n");
	scanf("%d",&op);
	send(clientSocket,&op,sizeof(int),0);
	switch(op){

		case 1 : loginAdmin();
				 break;
		case 2 : loginNmlUsr();
				 break;
		case 3 : loginJointAcc();
				 break;
		case 4 : close(clientSocket);
				 exit(1);
				 break; //may not complete properly
		default: close(clientSocket);
				 exit(1); 
				 break;
	}
}

int main(){

	int ret;
	struct sockaddr_in serverAddr;
	//char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Welcome to Online Banking System of noUse...\n");

	displayMenu();

	// while(1){
	// 	printf("Client: \t");
	// 	scanf("%s", &buffer[0]);
	// 	send(clientSocket, buffer, strlen(buffer), 0);

	// 	if(strcmp(buffer, ":exit") == 0){
	// 		close(clientSocket);
	// 		printf("[-]Disconnected from server.\n");
	// 		exit(1);
	// 	}

	// 	if(recv(clientSocket, buffer, 1024, 0) < 0){
	// 		printf("[-]Error in receiving data.\n");
	// 	}else{
	// 		printf("Server: \t%s\n", buffer);
	// 	}
	// }

	return 0;
}
