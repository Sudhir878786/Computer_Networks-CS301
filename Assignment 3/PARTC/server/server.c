#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <stdbool.h>

#define MAXLINE 1024  
#define PORT "3495"


struct node
{
	int data;
	struct node *next;
};

typedef struct node socket_list;

#define MAX 			2005
#define INF 			1000000000
#define false 			0
#define true 			1

struct heap
{
	int freq;
	int data;
	int size;
};

typedef struct heap heap;

heap *min_pq[MAX];

int parent(int i)
{
	return (i/2);
}

int left(int i)
{
	return i+i;
}

int right(int i)
{
	return i+i+1;
}

void swap(heap *a, heap *b)
{
	heap c = *a;
	*a = *b;
	*b = c;
}

bool cmp(int i, int j)
{
	if (min_pq[i]->freq == min_pq[j]->freq)
		return (min_pq[i]->data < min_pq[j]->data);
	return (min_pq[i]->freq < min_pq[j]->freq);
}

void initialise()
{
	int i;
	for(i=0; i<MAX; ++i)
        min_pq[i] = (heap *)calloc(1, sizeof(heap));
}

void print(heap *x)
{
    printf("%d --> %d, ", x->data, x->freq);
}

void print_heap()
{
    int i;
    for (i=1; i<=min_pq[0]->size; ++i)
        print(min_pq[i]);
    printf("\n");
}

void min_heapify(int i)
{
	int l = left(i);
	int r = right(i);

	int smallest = i;

	if (l <= min_pq[0]->size)
	{
		if (cmp(l, smallest))
			smallest = l;
	}
	if (r <= min_pq[0]->size)
	{
		if (cmp(r, smallest))
			smallest = r;
	}

	if (smallest != i)
	{
		swap(min_pq[i], min_pq[smallest]);
		min_heapify(smallest);
	}
}

void build_min_heap()
{
	int i, j = (min_pq[0]->size)/2;
	for(i=j; i>=1; --i)
		min_heapify(i);
}

heap *heap_minimum()
{
	return min_pq[1];
}

heap *heap_extract_minimum()
{
	if (min_pq[0]->size < 1)
	{
		printf("heap underflow\n");
		exit(0);
	}

	heap *w;
	swap(min_pq[1], min_pq[min_pq[0]->size]);

	w = min_pq[min_pq[0]->size];
	min_pq[0]->size -= 1;
	min_heapify(1);
	return w;
}

void heap_decrease_key(int i, int key)
{
	if (key > min_pq[i]->freq)
	{
		printf("New key is greater than current key\n");
		return ;
	}
	min_pq[i]->freq = key;
	while ((i>1) && (cmp(i, parent(i))))
	{
		swap(min_pq[i], min_pq[parent(i)]);
		i = parent(i);
	}
}

void insert(heap *temp)
{
	min_pq[0]->size += 1;
	min_pq[min_pq[0]->size] = temp;
	heap_decrease_key(min_pq[0]->size, temp->freq);
}

int input_func(char *message, char end)
{
  int i=0;
  int id = 0;
  while(message[i] != end)
  {
    id = id*10+(message[i]-'0');
    i++;
  }
  return id;
}

void *cast_ipv(struct sockaddr* sa)
{
  if(sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void print_list(socket_list *a)
{
	if (a == NULL)
	{
		printf("List is empty\n");
		return;
	}
	while (a!=NULL)
	{
		printf("%d ", a->data);
		a = a->next;
	}
	printf("\n");
}

socket_list* add_front(socket_list *a, int val)
{
	socket_list *ref = (socket_list*)malloc(sizeof(socket_list));
	ref->data = val;
	ref->next = a;
	a = ref;
	return a;
}

socket_list* add_end(socket_list *a, int val)
{
	socket_list *ref = (socket_list*)malloc(sizeof(socket_list));
	ref->data = val;
	ref->next = NULL;
	socket_list *last = a;
	if (last==NULL)
	{
		a = ref;
	}
	else
	{
		while(last->next!=NULL)
			last = last->next;
		last->next = ref;
	}
	return a;
}

socket_list* delete_front(socket_list *a)
{
	if (a==NULL)
	{
		printf("List is empty");
		return a;
	}
	socket_list *ref = a;
	a = a->next;
	printf("Data deleted is %d\n", ref->data);
	free(ref);
	return a;
}

socket_list* delete_end(socket_list *a)
{
	if (a==NULL)
	{
		printf("List is empty");
		return a;
	}
	socket_list *prev = a;
	socket_list *last = a->next;
	if (last != NULL)
	{
		while (last->next!=NULL)
		{
			prev = last;
			last = last->next;
		}
		prev->next = NULL;
		printf("Data deleted is %d\n", last->data);
		free(last);
	}
	else
	{
		a = NULL;
		printf("Data deleted is %d\n", prev->data);
		free(prev);
	}
	return a;
}
int main(int argc, char* argv[])
{
  char* serverAddrString = NULL;
  int delay = 2;
  for(int i=1;i<argc;i++)
  {
    if(argv[i][0] == '-' && argv[i][1] == 'd')
    {
      i++;
      delay = input_func(argv[i],'\0');
    }
    else 
    {
      serverAddrString = (char*)malloc(sizeof(*argv[i]));
      strcpy(serverAddrString, argv[i]);
    }
  }
  int socketDescriptor;
  int number;
  int addressLength;
  char message[MAXLINE];

  struct sockaddr_storage  clientAddress;
  //Get IP from name
  int ev;
  struct addrinfo hints, *servinfo, *it;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if((ev = getaddrinfo(serverAddrString, PORT, &hints, &servinfo))!=0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ev));
    return 1;
  }

  for(it = servinfo; it != NULL; it = it->ai_next)
  {
    if((socketDescriptor = socket(it->ai_family, it->ai_socktype, it->ai_protocol))!=-1)
    {
      if(bind(socketDescriptor,it->ai_addr, it->ai_addrlen)==-1)
      {
        close(socketDescriptor);
        continue;
      }
      else
        break;
    }
  }

  if(it == NULL)
  {
    perror("Failed to acquire a socket!\n");
    return 2;
  }
  char sa[INET6_ADDRSTRLEN];
  inet_ntop(it->ai_family,cast_ipv(it->ai_addr),sa, INET6_ADDRSTRLEN);
  printf("\nServer Started ...%s\n",sa);

  freeaddrinfo(servinfo);

  int c=0;

  while(1){
    // printf("\n");
    addressLength = sizeof(clientAddress);

    number = recvfrom(socketDescriptor,message,MAXLINE,0,(struct sockaddr*)&clientAddress,&addressLength);
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(clientAddress.ss_family,cast_ipv((struct sockaddr*)&clientAddress),ip, INET6_ADDRSTRLEN);
    printf("\n Message from client %s:%d\n ", ip, ntohs(((struct sockaddr_in6*)&clientAddress)->sin6_port)); 
    // printf("\n Client's Message: %s ",message);

    if(number<1)
      perror("send error");

    int n = rand();
    if(c>0 && n%c==0)
    {
      sleep(delay);
    }
    printf("n=%d c=%d\n",n, c);
      // printf("%s\n", message);
      sendto(socketDescriptor,message,number,0,(struct sockaddr*)&clientAddress,addressLength);
      if(c>20)
        c=0;
      else
        c++;
  }
  close(socketDescriptor);
  return 0;
}
