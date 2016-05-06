 /**
 * Problem     : Paralle Project ping, flood, mirror
 * Author      : Almgwary
 * Date        : 5-May-2016
 * Thin        : 4h
 * ID          : 20120262
 **/

#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <math.h> 


   /****************************************
	* This code run ping,floode, mirror in with parallel processes
	* 
	* in Mirror : master collect basic links then slave downlouding pages
	*
	**/














int my_rank;
int size ;
int tag;
int source;
int dest ;
MPI_Status status;
char * my_rank_char ;
// to save links here  100 link link size = 1035 ;
char links[100*1035];
int linkCount = -1 ;
int link_size  = 1035 ;

// integer to char array 
char * toArray(int n);
void ping (char * ip , int count);
void flood (char * ip  ); 
// check website reachablilty 
void curl (char *url );
void mirror (char *url );
// get http linsk fro url and store to links array then update  links
// get http linsk fro url and store to links array then update  link
void getLinksFromUrl (char* url);
void printCurrentLinks();

// all processes runMirror
void runMirror (char * url) ;
// all processes runPing
void runPing (char * ip);
// all processes runFloode
void runFloode (char * ip);
//mirror multi urls form file
void runMirrorFromFile();

int main(int argc, char  *argv[])
{



 	MPI_Init(&argc,&argv);
 	MPI_Comm_size(MPI_COMM_WORLD, &size);
 	MPI_Comm_rank(MPI_COMM_WORLD , &my_rank);
 	
 	// get my rank as char array 
 	my_rank_char = toArray(my_rank);
 	int operation ; 
 	if (my_rank==0)
 	{
 		printf("Choose Operation\n[0] mirror\n[1] ping\n[2] flode\n[3] CollectAndmirror\n[4] mirrorFromFile\n");
 		scanf("%d",&operation); 		
 	}
 	// sending operations to processes
 	MPI_Bcast (&operation, 1, MPI_INT,0, MPI_COMM_WORLD);
 	if (operation == 0)
 	{
 		 mirror("http://www.w3schools.com/html/html_links.asp");
 	}else if (operation == 1)
 	{
 		 runPing("174.129.224.129");
 	}else if (operation == 2)
 	{
 		 runFloode("193.227.14.57");
 	}else if (operation == 3)
 	{
 		 runMirror("http://www.w3schools.com/html/html_links.asp");
 	}else if (operation == 4)
 	{
 		 runMirrorFromFile();
 	}

 	
 	
 	//ping 
 	//router:ping("192.168.1.1" , 100);
 	//ecom :p ing("193.227.14.57" , 5000);
 	//newgradiance : ping("174.129.224.129" , 5000);


 	//mirror 
 	// wikipedia:("https://en.wikipedia.org/wiki/Portal:Contents/A–Z_index");
 	//cu.edu. : mirror("http://cu.edu.eg/Home");
	//wekibedia dumbs DONOT USE Its larg: ("https://dumps.wikimedia.org/enwiki/20160407/");

 	// floode : flood("193.227.14.57");
 
 	// check reachablity 
 	//curl ( "newgradiance.com") ;


	MPI_Finalize();
 	return 0;	 	
 }


//mirror multi urls form file
void runMirrorFromFile(){

	if (my_rank == 0 ) // master read urls form file
	{
		char * line = NULL;
	    size_t len = 0;
	    ssize_t read;
		FILE *file= fopen("urls.txt","r+"); //readFromFile
		if(file!=NULL){
			while ((read = getline(&line, &len, file)) != -1) {
		       // printf("Retrieved line of length %zu :\n", read);
		        
		        linkCount++;
				memcpy(&links[linkCount*link_size],line,link_size);
				 
				printf("Link ~  %s \n", &links[linkCount*link_size]);
		    	 
		    }
			fclose(file);
		}else{printf("unable to open file");}
	}



	//1. master bcast links count
 	MPI_Bcast (&linkCount, 1, MPI_INT,0, MPI_COMM_WORLD);
 	

 	if (linkCount > -1) // there exist some links
 	{
 		//1. master bcast links array
 		MPI_Bcast (&links, 100*1035, MPI_CHAR,0, MPI_COMM_WORLD);
 	}


 	//3. each process take subGroup of array and start dawenlouding dawenloud 
 	int smallCount =  linkCount / size ;
 	int remaining =  linkCount % size ;
 	int i = 0;
 	for (i ; i < smallCount; ++i)
 	{
 		mirror(&links[my_rank*smallCount + i] );
 	} 


}


// all processes runMirror
void runMirror (char * url){
	printf("runMirror\n" );
 	if (my_rank==0)
 	{
 		printf("Master Collecting Links form URL: http://stackoverflow.com\n");
 		getLinksFromUrl(url);
		printCurrentLinks();
 	}

   /**
 	*  --- DawenLouding  links files ---
 	*  1. master bcast links count
 	*  2. master bcast links array
 	*  3. each process take subGroup of array and start dawenlouding dawenloud 
 	*
 	*
 	**/

 	//1. master bcast links count
 	MPI_Bcast (&linkCount, 1, MPI_INT,0, MPI_COMM_WORLD);
 	

 	if (linkCount > -1) // there exist some links
 	{
 		//1. master bcast links array
 		MPI_Bcast (&links, 100*1035, MPI_CHAR,0, MPI_COMM_WORLD);
 	}


 	//3. each process take subGroup of array and start dawenlouding dawenloud 
 	int smallCount =  linkCount / size ;
 	int remaining =  linkCount % size ;
 	int i = 0;
 	for (i ; i < smallCount; ++i)
 	{
 		mirror(&links[my_rank*smallCount + i] );
 	} 
}


// all processes runPing
void runPing (char * ip){
	printf("runPing\n" );
	ping(ip , 5000);
}



// all processes runFloode
void runFloode (char * ip){
	printf("runFloode\n" );
	flood(ip);
}
// integer to char array 
char * toArray(int n){
	char buf[sizeof(int)*3+2];
	snprintf(buf, sizeof buf, "%d", n);
    return buf;
	}


void ping (char * ip , int count){
   /**
	* Domain Name : newgradiance.com
	* IP Address  : 174.129.224.129
	*
	**/
	char * c = toArray (count ) ;
	char * command  =   (char *) malloc(10 + strlen(ip)+ strlen(c));
	strcpy(command, "ping -c ");
	strcat(command, c);
	strcat(command, " ");
	strcat(command, ip);
	
	int i = 0 ;
	printf("command : %s\n",command );
	system(command);

	}


void flood (char * ip  ){
   /**
	* Domain Name : newgradiance.com
	* IP Address  : 174.129.224.129
	*
	**/ 
	char * command  =   (char *) malloc(20 + strlen(ip));
	strcpy(command, "ping -i 2 -f -s 65507  ");
	strcat(command, " ");
	strcat(command, ip);
	
	int i = 0 ;
	printf("command : %s\n",command );
	system(command);

	}
 
// check website reachablilty 
void curl (char *url ){
	//wget --mirror -p --convert-links -P ./example-mirror/my_rank /website

	// allocating command size
	char * command  =   (char *) malloc(55 + strlen(url)+ strlen(my_rank_char) );
	
	// creating command
	strcpy(command, "curl -Is  "); 
	strcat(command, url);
	strcat(command, " | head -1 ");

	// run command
	system(command);
	}



void mirror (char *url ){
	//wget --mirror -p --convert-links -P ./example-mirror/my_rank /website

	// allocating command size
	char * command  =   (char *) malloc(100+ strlen(url)+ strlen(my_rank_char) );
	
	// creating command
	// recursiv
	//strcpy(command, "wget -r -l 2  -e robots=off --mirror -p --convert-links -P ./mirrorFiles/");
	// not recursiv
	strcpy(command, "wget   -l 2  -e robots=off --mirror -p --convert-links -P ./mirrorFiles/");
	strcat(command, my_rank_char);
	strcat(command, " ");
	strcat(command, url);

	// run command
	system(command);
	}


// get http linsk fro url and store to links array then update  links
// get http linsk fro url and store to links array then update  links count
void getLinksFromUrl (char* url){
		
		//  prepare command
		char * command  =   (char *) malloc(150 + strlen(url) );
	
		// creating command : wget --quiet -O - http://stackoverflow.com | grep -o 'http://[^\"]*'
		strcpy(command, "wget --quiet -O - "); 
		strcat(command, url);
		strcat(command, " | grep -o 'http://[^\"]*'");


		//printf("Enterd command %s\n" , command);
		FILE *fp;
		char path[1035] ; 
		/* Open the command for reading. */
		fp = popen(command, "r");
		if (fp == NULL) {
		  printf("Failed to run command\n" );
		  exit(1);
		}

	    linkCount ; 
		/* Read the output a line at a time - output it. */
		while (fgets(path, sizeof(path)-1, fp) != NULL) {
		  //printf("Printig : %s", path);
		linkCount++;
		strcpy(&links[linkCount*link_size], path);
		

		}

		/* close */
  		pclose(fp);
  		
		}


void printCurrentLinks(){
	    int i = 0 ; 
  	    printf("linksCount= [%d] \n", linkCount);
	 	
	 	for ( i = 0; linkCount > -1 && i <= linkCount; ++i)
	 	{
	 		 printf(" P%d linkCount=[%d]: %s\n", my_rank,i , &links[i*link_size]);
	 	}
		}
  

 	 
 

 
