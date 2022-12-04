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

#define ADMUNAME "admin"
#define ADMPWD "admin"

#define UDB_PATH "db_user"
#define NDB_PATH "db_nuser"
#define JDB_PATH "db_juser"

int newSocket;

struct userdb{
	char name[SIZE];
	int value;
};

struct nrmldb{
	int flag;
	char name[SIZE];
	char pwd[SIZE];
	int balance;
};

struct jointdb{
	int flag;
	char name1[SIZE];
	char name2[SIZE];
	char pwd[SIZE];
	int balance;
};

void displayMenu();

int userDbRead(char uname[SIZE]){ 
	int val = 0,fd;
	struct userdb udb;
	fd=open(UDB_PATH, O_RDWR|O_CREAT,0775);
	while(read(fd, &udb, sizeof(struct userdb))){ //seems good now
		if(strcmp(udb.name,uname)==0){
			close(fd);
			return udb.value;
		}
	}
	close(fd);
	return val;
}

void userDbWrite(struct userdb udb, int n){
	int fd;
	fd=open(UDB_PATH,O_RDWR|O_CREAT,0775);
	if(n==0){
		lseek(fd, 0, SEEK_END);
		write(fd, &udb, sizeof(struct userdb));
		close(fd);
	}else if(n == 1){
		struct userdb ub;
		while(read(fd, &ub, sizeof(struct userdb))){ //seems good now
			if(strcmp(ub.name,udb.name)==0){
				lseek(fd,-1*sizeof(struct userdb),SEEK_CUR);
				write(fd, &udb, sizeof(struct userdb));
				close(fd);
				break;
		}
	  }
    }
}

int nrmlDbWrite(struct nrmldb ndb, int n){
	int fd, flg=1; 
	fd=open(NDB_PATH,O_RDWR|O_CREAT,0775);
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(struct nrmldb);
	if(n==0){  //to write the data @ last of the file
		lseek(fd, 0, SEEK_END);
		
		fcntl(fd,F_SETLK,&lock);
		write(fd, &ndb, sizeof(struct nrmldb));
		lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		
		flg = 0;
		close(fd);
	}else if(n == 1){ //to update the data @ matched user/pwd
		struct nrmldb nb;
		while(read(fd, &nb, sizeof(struct nrmldb))){ //seems good now
			if(strcmp(nb.name,ndb.name)==0 && strcmp(nb.pwd,ndb.pwd)==0){
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				
				fcntl(fd,F_SETLKW,&lock);
				write(fd, &ndb, sizeof(struct nrmldb));
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl(fd, F_SETLKW, &lock);

				flg=0;
				close(fd);
				return flg;
		}
	  }
	}else if(n == 2){ //to change the password
		struct nrmldb nb;
		while(read(fd, &nb, sizeof(struct nrmldb))){ //seems good now
			if(strcmp(nb.name,ndb.name)==0){
				strcpy(nb.pwd,ndb.pwd);
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				
				fcntl(fd,F_SETLKW,&lock);
				write(fd, &nb, sizeof(struct nrmldb));
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl(fd, F_SETLKW, &lock);

				flg=0;
				close(fd);
				return flg;
		}
	  }
	}else if (n == 3){ //to update the balance
		struct nrmldb nb;
		while(read(fd, &nb, sizeof(struct nrmldb))){
			if(strcmp(nb.name,ndb.name)==0){
				if((nb.balance-ndb.balance) >= 0){
					nb.balance = nb.balance - ndb.balance;
					lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
					
					fcntl(fd,F_SETLKW,&lock);
					write(fd, &nb, sizeof(struct nrmldb));
					lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
					lock.l_type = F_UNLCK;
					fcntl(fd, F_SETLKW, &lock);
					
					flg=0;
					close(fd);
					return flg;
			}else{
				close(fd);
				return flg;
			}
		}
	  }
	}
	close(fd);
	return flg;
}

int jDbWrite(struct jointdb jdb, int n){
	int fd, flg=1;
	fd=open(JDB_PATH,O_RDWR|O_CREAT,0775);
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(struct jointdb);
	if(n == 0){
		lseek(fd, 0, SEEK_END);

		fcntl(fd,F_SETLK,&lock);
		write(fd, &jdb, sizeof(struct jointdb));
		lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);

		flg = 0;
		close(fd);
	}else if(n == 1){
		struct jointdb jb;
		while(read(fd, &jb, sizeof(struct jointdb))){ //seems good now
			if((strcmp(jb.name1,jdb.name1)==0 || strcmp(jb.name2,jdb.name1)==0)
				&& strcmp(jb.pwd,jdb.pwd)==0){
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);

				fcntl(fd,F_SETLKW,&lock);
				write(fd, &jdb, sizeof(struct jointdb));
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl(fd, F_SETLKW, &lock);

				flg=0;
				close(fd);
				return flg;
		}
	  }
	}else if(n == 2){  //to change the password
		struct jointdb jb;
		while(read(fd, &jb, sizeof(struct jointdb))){ //seems good now
			if(strcmp(jb.name1,jdb.name1)==0 || strcmp(jb.name2,jdb.name1)==0){
				strcpy(jb.pwd,jdb.pwd);
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
				
				fcntl(fd,F_SETLKW,&lock);
				write(fd, &jb, sizeof(struct jointdb));
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl(fd, F_SETLKW, &lock);

				flg=0;
				close(fd);
				return flg;
		}
	  }
	}else if( n == 3){  //to update the balance
		struct jointdb jb;
		while(read(fd, &jb, sizeof(struct jointdb))){ //seems good now
			if(strcmp(jb.name1,jdb.name1)==0 || strcmp(jb.name2,jdb.name1)==0){
				if((jb.balance-jdb.balance) >= 0){
					jb.balance-=jdb.balance;
					lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
					
					fcntl(fd,F_SETLKW,&lock);
					write(fd, &jb, sizeof(struct jointdb));
					lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
					lock.l_type = F_UNLCK;
					fcntl(fd, F_SETLKW, &lock);

					flg=0;
					close(fd);
					return flg;
			}else{
				close(fd);
				return flg;
			}
		}
	  }
	}
	return flg;
}

struct nrmldb nrmlDbRead(char user[SIZE]){
	struct nrmldb ndb;
	int fd;
	fd=open(NDB_PATH,O_RDWR|O_CREAT,0775);
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(struct nrmldb);
	while(1){
		fcntl(fd,F_SETLKW,&lock);
		if(read(fd, &ndb, sizeof(struct nrmldb))){
			if(strcmp(ndb.name,user) ==  0){
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl (fd, F_SETLKW, &lock);
				close(fd);
				return ndb;
			}else{
				lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl (fd, F_SETLKW, &lock);
				lseek(fd, sizeof(struct nrmldb), SEEK_CUR);
			}
		}else{
			//lseek(fd, -1*sizeof(struct nrmldb), SEEK_CUR);
			lock.l_type = F_UNLCK;
			fcntl (fd, F_SETLKW, &lock);
			//lseek(fd, sizeof(struct nrmldb), SEEK_CUR);
			//since no bytes are read
			close(fd);
			break;
		}
	}
	return ndb;
}

struct jointdb jDbRead(char user[SIZE]){
	struct jointdb jdb;
	int fd;
	fd=open(JDB_PATH,O_RDWR|O_CREAT,0775);
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(struct jointdb);
	while(1){
		fcntl(fd,F_SETLKW,&lock);
		if(read(fd, &jdb, sizeof(struct jointdb))){
			if(strcmp(jdb.name1,user) ==  0 || strcmp(jdb.name2,user) ==  0 ){
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl (fd, F_SETLKW, &lock);
				close(fd);
				return jdb;
			}else{
				lseek(fd, -1*sizeof(struct jointdb), SEEK_CUR);
				lock.l_type = F_UNLCK;
				fcntl (fd, F_SETLKW, &lock);
				lseek(fd, sizeof(struct jointdb), SEEK_CUR);
			}
		}else{
			lock.l_type = F_UNLCK;
			fcntl (fd, F_SETLKW, &lock);
			//lseek(fd, sizeof(struct jointdb), SEEK_CUR);
			//since no bytes are read
			close(fd);
			break;
		}
		
	}
	return jdb;
}

void userAdd(){
	char uname[SIZE];
	int value=0;
	// printf("Enter username\n");
	// scanf("%s",uname);
	recv(newSocket,uname,sizeof(uname),0);
	value = userDbRead(uname);
	send(newSocket,&value,sizeof(value),0); 
	if(value==0){
		char pwd[SIZE];
		// printf("Enter your Password \n");
		// scanf("%s",&pwd);
		recv(newSocket,pwd,sizeof(pwd),0);
		// printf("Enter user type(N/J)\n");
		int ch; int amt=0;
		// scanf("%c",&ch);
		recv(newSocket,&ch,sizeof(ch),0);
		if(ch==0){
			// printf("Enter the initial deposite\n");
			// scanf("%d",&amt);
			recv(newSocket,&amt,sizeof(amt),0);
			struct userdb udb;
			struct nrmldb ndb;

			strcpy(udb.name ,uname);
			udb.value = 2;
			userDbWrite(udb,0);

			ndb.flag = 1;
			strcpy(ndb.name, uname);
			strcpy(ndb.pwd, pwd);
			ndb.balance = amt;
			nrmlDbWrite(ndb,0);
		}
		else if(ch== 1){
			// printf("Enter deposite amount\n");
			// scanf("%d",&amt);
			char uname1[SIZE];
			// printf("Enter the second username");
			// scanf("%s",uname1);
			recv(newSocket,uname1,sizeof(uname1),0);
			recv(newSocket,&amt,sizeof(amt),0);
			struct userdb udb1;
			struct userdb udb2;
			struct jointdb jdb;

			strcpy(udb1.name ,uname);
			udb1.value = 3;
			userDbWrite(udb1,0);

			strcpy(udb2.name ,uname1);
			udb2.value = 4;
			userDbWrite(udb2,0);

			jdb.flag = 1;
			strcpy(jdb.name1, uname);
			strcpy(jdb.name2, uname);
			strcpy(jdb.pwd, pwd);
			jdb.balance = amt;
			jDbWrite(jdb,0);

		}
		//printf("User added successfully!!!\n");
		displayMenu();
	}else{
		//printf("User already there in DB !!!\n");
		displayMenu();
	}
}

void deleteUser(){
	char uname[SIZE], pwd[SIZE];
	int value=0;
	// printf("Enter username you want to delete\n");
	// scanf("%s",uname);
	// printf("Enter password for above username\n");
	// scanf("%s",pwd);
	recv(newSocket,uname,sizeof(uname),0);
	value = userDbRead(uname);
	send(newSocket,&value,sizeof(value),0);
	if(value>0){
		recv(newSocket,pwd,sizeof(pwd),0);
		int flag=1;
		if(value == 2){
			struct userdb udb;
			strcpy(udb.name, uname);
			udb.value = 0;
			userDbWrite(udb, 1);

			struct nrmldb ndb;
			ndb.flag = 0;
			strcpy(ndb.name, uname);
			strcpy(ndb.pwd, pwd);
			ndb.balance = 0;
			flag = nrmlDbWrite(ndb,1);

		}else if(value == 3 || value == 4){
			struct userdb udb;
			strcpy(udb.name, uname);
			udb.value = 0;
			userDbWrite(udb, 1);

			struct jointdb jdb;
			jdb.flag = 0;
			strcpy(jdb.name1, uname);
			strcpy(jdb.name2, "xxx");
			strcpy(jdb.pwd, pwd);
			jdb.balance = 0;
			flag = jDbWrite(jdb,1);

		}

		send(newSocket,&flag,sizeof(flag),0);
		if(flag == 0){
			// printf("Record deleted successfully\n");
			displayMenu();
		}else{
			// printf("You have entered wrong password!!!\n");
			displayMenu();
		}
	}else{
		// printf("User not found to erase !!!\n");
		displayMenu();
	}
}

void modifyUser(char uname[SIZE]){
	char pwd[SIZE];
	int value=0,amt=0,withdraw=1;
	value = userDbRead(uname);
	send(newSocket,&value,sizeof(value),0);
	if(value>0){
		int ch;
		struct nrmldb ndb;
		struct jointdb jdb;
		// printf("Enter from following : \n");
		// printf("1. Password change \n");
		// printf("2. Amount withdraw \n");
		// printf("3. Amount deposite \n");
		// scanf("%d", &ch);
		recv(newSocket,&ch,sizeof(ch),0);
		if(value==2){
			switch(ch){
				case 1 : 	//printf("Enter new password for %s \n",uname);
							recv(newSocket,pwd,sizeof(pwd),0);
							ndb.flag = 0;
							strcpy(ndb.name, uname);
							strcpy(ndb.pwd, pwd);
							ndb.balance = 0;
							nrmlDbWrite(ndb,2);
							//printf("Password updated successfully !!! \n");
							displayMenu();
							break;
				case 2 :	//printf("Enter the amount to withdraw \n");
							recv(newSocket,&amt,sizeof(amt),0);
							ndb.flag = 0;
							strcpy(ndb.name, uname);
							strcpy(ndb.pwd, "xxx");
							ndb.balance = amt;
							withdraw = nrmlDbWrite(ndb,3);
							send(newSocket,&withdraw,sizeof(withdraw),0);
							// if(!withdraw){
							// 	printf("Amount deducted successfully \n");
							// }else{
							// 	printf("You've insufficient balance !!! \n");
							// }
							displayMenu();
							break;
				case 3 : 	//printf("Enter the amount to deposite \n");
							recv(newSocket,&amt,sizeof(amt),0);
							ndb.flag = 0;
							strcpy(ndb.name, uname);
							strcpy(ndb.pwd, "xxx");
							ndb.balance = -1*amt;
							
							withdraw = nrmlDbWrite(ndb,3);
							//printf("Amount deposited successfully \n");
							displayMenu();
							break;
				default:  	displayMenu();
							break;	
			}

		}else if(value == 3 || value == 4){ //implement locking mechanism in this
			switch(ch){
				case 1 : 	//printf("Enter new password for %s \n",uname);
							recv(newSocket,pwd,sizeof(pwd),0);
							jdb.flag = 0;
							strcpy(jdb.name1, uname);
							strcpy(jdb.name2, "xxx");
							strcpy(jdb.pwd, pwd);
							jdb.balance = 0;
							jDbWrite(jdb,2);
							//printf("Password updated successfully !!! \n");
							displayMenu();
							break;
				case 2 :	//printf("Enter the amount to withdraw \n");
							recv(newSocket,&amt,sizeof(amt),0);
							printf("%d\n",amt);
							jdb.flag = 0;
							strcpy(jdb.name1, uname);
							strcpy(jdb.name2, "xxx");
							strcpy(jdb.pwd, "xxx");
							jdb.balance = amt;
							
							withdraw = jDbWrite(jdb,3);
							send(newSocket,&withdraw,sizeof(withdraw),0);
							// if(!withdraw){
							// 	printf("Amount deducted successfully \n");
							// }else{
							// 	printf("You've insufficient balance !!! \n");
							// }
							displayMenu();
							break;
				case 3 : 	//printf("Enter the amount to deposite \n");
							recv(newSocket,&amt,sizeof(amt),0);
							jdb.flag = 0;
							strcpy(jdb.name1, uname);
							strcpy(jdb.name2, "xxx");
							strcpy(jdb.pwd, "xxx");
							jdb.balance = -1*amt;
							
							withdraw = jDbWrite(jdb,3);
							//printf("Amount deposited successfully \n");
							displayMenu();
							break;
				default:  	displayMenu();
							break;

		}
	}
}else{
		//printf("User not found to modify!!!\n");
		displayMenu();
	}
}

void searchUser(){
	char uname[SIZE], pwd[SIZE];
	int value=0,amt=0;
	// printf("Enter username you want to search\n");
	// scanf("%s",uname);
	recv(newSocket,uname,sizeof(uname),0);
	value = userDbRead(uname);
	printf("%d\n",value);
	send(newSocket,&value,sizeof(value),0);
	if(value>0){
		int flag = 0;
		
		if(value == 2){
			struct nrmldb nb; 
			nb = nrmlDbRead(uname);
			flag = nb.flag;
			printf("%d\n", flag);
			strcpy(pwd, nb.pwd);
			amt = nb.balance;
		}else if(value == 3 || value == 4){
			struct jointdb jb; 
			jb =  jDbRead(uname);
			flag = jb.flag; 
			strcpy(pwd, jb.pwd);
			amt = jb.balance;
		}
		send(newSocket,&flag,sizeof(flag),0);
		if(flag){
		
		send(newSocket,pwd,sizeof(pwd),0);
		// printf("Password for %s is %s \n",uname, pwd);
		send(newSocket,&amt,sizeof(amt),0);
		//printf("Balance for %s is %d \n",uname, amt);
		//printf("Press enter for main menu()");
		int ch;
		//scanf("%c",&ch);
		recv(newSocket,&ch,sizeof(ch),0);
		if(ch == 1)
			displayMenu();
		}else{
			//printf("User not found to show!!!\n");
			displayMenu();
		}
	}else{
		//printf("User not found to show!!!\n");
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
				 // printf("Enter username you want to modify\n");
				 // scanf("%s",uname);
				 recv(newSocket,uname,sizeof(uname),0);
				 modifyUser(uname);
				 break;
		case 4 : searchUser();
				 break;
		default: //printf("You are redirecting to Main Menu as option doesn't match !!!");
				 displayMenu();
				 break; //may not complete properly
	}
}

void loginAdmin(){
	char uname[SIZE], pwd[SIZE];
	int value=0;
	// printf("Enter admin username\n");
	// scanf("%s",uname);
	recv(newSocket,uname,sizeof(uname),0);
	// printf("Enter Password\n");
	// scanf("%s",pwd);
	recv(newSocket,pwd,sizeof(pwd),0);
	//file access
	value = userDbRead(uname);
	if( value && (strcmp(pwd,ADMPWD)==0)) {
	}else
		value = 0;

	send(newSocket,&value,sizeof(value),0);
	if(value){
		int op;
		// printf("\n\n Admin Setup @ noUse !!! \n\n");
		// printf("1: Add User\n");
		// printf("2: Delete User\n");
		// printf("3: Modify Details of User\n");
		// printf("4: Search for...\n");
		// printf("Choose option : \n");
		// scanf("%d",&op);
		recv(newSocket,&op,sizeof(op),0);
		loginAdminScreen(op);
	}
	else{
		displayMenu();
	}
}

void loginNmlUsr(){
	char uname[SIZE], pwd[SIZE];
	int flag=0, value =0;
	int amount=0;
	// printf("Normal User Login Screen !!!\n");
	// printf("Enter your username\n");
	// scanf("%s",uname);
	// printf("Enter your passwrord\n");
	// scanf("%s",pwd);
	recv(newSocket,uname,sizeof(uname),0);
	recv(newSocket,pwd,sizeof(pwd),0);
	value = userDbRead(uname);
	
	if(value == 2){
		struct nrmldb nb;
		nb =  nrmlDbRead(uname);
		flag = nb.flag;
		if(strcmp(nb.pwd,pwd) != 0)
			flag = 0;
		else
			amount = nb.balance;
		send(newSocket,&flag,sizeof(flag),0);
	}else{
		send(newSocket,&flag,sizeof(flag),0);
	}
	
	if(flag){
		
		send(newSocket,&amount,sizeof(amount),0);
		//printf("Your account balance is %d rupees \n", amount);
		modifyUser(uname);	
	}
	else{
		// printf("Username/Password doesn't match \n", );
		displayMenu();
	}

}

void loginJointAcc(){
	char uname[SIZE], pwd[SIZE];
	int flag=0, value = 0;
	int amount=0;
	// printf("Joint Account User Login Screen !!!\n");
	// printf("Enter your username\n");
	// scanf("%s",uname);
	// printf("Enter your passwrord\n");
	// scanf("%s",pwd);
	recv(newSocket,uname,sizeof(uname),0);
	recv(newSocket,pwd,sizeof(pwd),0);
	//send(clientSocket,&flag,sizeof(flag),0);
	value = userDbRead(uname);
	
	if(value == 3 || value == 4){
		struct jointdb jb;
		jb =  jDbRead(uname);
		flag = jb.flag;
		if(strcmp(jb.pwd,pwd) != 0)
			flag = 0;
		else{
			amount = jb.balance;
			strcpy(uname,jb.name1);
		}
		send(newSocket,&flag,sizeof(flag),0);
	}else{
		send(newSocket,&flag,sizeof(flag),0);
	}
	
	if(flag){
		
		send(newSocket,&amount,sizeof(amount),0);
		send(newSocket,uname,sizeof(uname),0);
		// printf("Your account balance is %d rupees \n", amount);
		modifyUser(uname);	
	}
	else{
		// printf("Username/Password doesn't match \n", );
		displayMenu();
	}
}

void displayMenu(){
	int op;
	// printf("\n\n Login-SignUp \n\n");
	// printf("1: Admin Login \n");
	// printf("2: Normal User Login\n");
	// printf("3: Joint Account User Login\n");
	// printf("4: Exit\n\n\n");
	// printf("Choose option : ");
	// scanf("%d",&op);
	recv(newSocket,&op,sizeof(int),0);
	printf("option is %d\n", op);
	switch(op){

		case 1 : loginAdmin();
				 break;
		case 2 : loginNmlUsr();
				 break;
		case 3 : loginJointAcc();
				 break;
		default: exit(1);
	}
}

void startExecution(char admin[SIZE], int val){
	struct userdb udb;
	strcpy(udb.name, admin);
	udb.value = val;
	int fd;
	fd=open(UDB_PATH,O_RDWR|O_CREAT,0775);
	write(fd, &udb, sizeof(struct userdb));
	close(fd);
	displayMenu();

}

int main(){

	int sockfd, ret, opt = 1;
	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;
	
	socklen_t addr_size;
	pid_t childpid;
	// Creating socket file descriptor
    //socket(domain, type, protocol) (IPv4, TCP,prctl field of IP header)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

    //int listen(int sockfd, int backlog)
    //backlog: max length to which the queue of pending conn for sockfd may grow 
	if(listen(sockfd, 5) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			printf("Error on accept!!! \n");
			exit(1);
		}
		printf("Connection accepted from %s:%d \n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
			startExecution(ADMUNAME,1);
		}else
			close(newSocket);

	}

	return 0;
}