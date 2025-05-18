#include "shell.h"







/*
  Function Declarations for builtin shell commands:
 */
int oshell_cd(char **args);
int oshell_help(char **args);
int oshell_exit(char **args);
int oshell_gacha(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "gacha"
};

int (*builtin_func[]) (char **) = {
  &oshell_cd,
  &oshell_help,
  &oshell_exit,
  &oshell_gacha
};

int oshell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int oshell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int oshell_help(char **args)
{
  int i;
  printf("HEllo this is the first experimental Oshell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < oshell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("your current directory(the cwd command) will be displayed before each command");
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int oshell_exit(char **args)
{
  return 0;
}

int oshell_gacha(char **args){

  srand(time(0));
  const char *adjectives[]={
"Clingy", "obnoxious", "pretentious", "lazy", "smug", "flaky", "whiny", "arrogant", "clueless", "selfish", "grumpy", "awkward", "nosy", "petty", "neurotic", "gullible", "manipulative", "clingy", "stubborn", "moody", "impulsive", "bossy", "annoying", "rude", "vain", "messy", "nitpicky", "dramatic", "childish", "snobby", "uptight", "tactless", "scatterbrained", "sarcastic", "boastful", "pushy", "cynical", "sneaky", "needy", "unreliable", "judgmental", "entitled", "spiteful", "two-faced", "loud", "passive-aggressive", "hypersensitive", "meddling", "impatient", "dishonest", "untrustworthy", "insecure", "overbearing", "condescending", "selfish", "kind", "thoughtful", "reliable", "creative", "generous"
  };
  const char *loading[]={
    "[                      ]",
    "[==                    ]",
    "[====                  ]",
    "[======                ]",
    "[========              ]",
    "[==========            ]",
    "[============          ]",
    "[==============        ]",
    "[================      ]",
    "[==================    ]",
    "[====================  ]",
    "[====================💡]"
  };
  const int len= sizeof(loading)/sizeof(loading[0]);
  const int x=rand() %len;
  printf(RED"Thinking of what would describe you..\n"RST);

  for(int i=0;i<len;i++){
    printf("\r"MAG"%s\n"RST,loading[i]);
    fflush(stdout); //Force update the shell
    usleep(421337);
  }
  printf("found it!! You're... %s\n",adjectives[x]);
  
  return 1;
}


int oshell_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int oshell_exec(char **args){
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < oshell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return oshell_launch(args);
}





//wrapper functions since i couldn't figure out normal separate files wrappers
void *Malloc(size_t size){
  void *ptr;

  if(size==0){
    return NULL;
  }
  ptr=malloc(size);
  if(!ptr){
    perror(RED"Malloc failed!"RST);
    exit(EXIT_FAILURE);
  }
  return ptr;
}


void *Realloc(void *ptr,size_t size){
  void *new_ptr;
  new_ptr=realloc(ptr, size);

  if(!new_ptr && size!=0){
    perror(RED"Realloc failed!"RST);
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}



void Getcwd(char *buff, size_t buffsize){
  if(getcwd(buff, buffsize)==NULL){
    perror(RED"Getcwd failed!!!"RST);

    //there is no need to return anything because the getcwd fct already stores the directory within the buffer



  }
}


char **oshell_split_line(char *line){
  char **tokens;
  size_t bufsize;
  unsigned int position=0;
  
  bufsize=BUFSIZ;
  tokens=Malloc(bufsize * sizeof(*tokens));

  for(char *token = strtok(line, DEL);token;token =strtok(NULL, DEL))
  {
    tokens[position++]=token;

    if(position>=bufsize){
      bufsize*=2;
      tokens=realloc(tokens,bufsize* sizeof(*tokens));
    }
  }
  tokens[position]=NULL;
  return tokens;
}





char *oshell_read_line(void){
  char *buff=NULL;
  size_t buff_size=0;
  char cwd[BUFSIZ];




  Getcwd(cwd, sizeof(cwd));

printf(CYAN"[BOD]  %s  [BOD]"RST"$> ", cwd);


  if(getline(&buff, &buff_size , stdin)==-1){
    buff=NULL;

    if(feof(stdin)){

      printf(RED"[EOF]"RST);
    }
    else{
      printf(RED"\n getline failed"RST);
      exit(EXIT_FAILURE);  
    }
  }
   return buff; 
}

int main(){

  char *line;
  char **args;
  int status;
    printf(BLUE"                                                  \n"
    " ██████╗    ███████╗██╗  ██╗███████╗██╗     ██╗     \n"
    "██╔═══██╗   ██╔════╝██║  ██║██╔════╝██║     ██║     \n"
    "██║   ██║   ███████╗███████║█████╗  ██║     ██║     \n"
    "██║   ██║   ╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
    "╚██████╔╝██╗███████║██║  ██║███████╗███████╗███████╗\n"
    " ╚═════╝ ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"
    "                                                  \n"RST);
  //we have to implement the REPL in our mini bash project (READ-EVALUATE-PRINT-LOOP)
  while ((line=oshell_read_line())) {
    //1 get line
   //already done in the while loop argument 
    //2-get tokens
    args=oshell_split_line(line);
    //3-executing
    status=oshell_exec(args);

    free(line);
    free(args);

  }
  return EXIT_SUCCESS;
}
