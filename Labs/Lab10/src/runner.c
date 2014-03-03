//alex meijer
//runner.c


int main(int argc, char *argv[]){

  //first run 31 first fit  
  double ffavgs[31];
  for (int i = 0; i < 31; i++){
    ffargs[i] = frageval(0, i+2, 50);
  }

//next run 31 best fit  
  double bfavgs[31];
  for (int i = 0; i < 31; i++){
    bfargs[i] = frageval(1, i+2, 50);
  }

//next run 31 worst fit  
  double wfavgs[31];
  for (int i = 0; i < 31; i++){
    wfargs[i] = frageval(2, i+2, 50);
  }

  printf("first fit: \n\n");

 for (int i = 0; i < 31; i++){
   printf("%f\n", ffargs[i]);
  }

}

double frageval(int alg, int seed, int requests){
srand(seed);

  int r = 0;
  int s = 0;
  void *p = NULL;
allocator_init(10240);
  while (r < requests){
    s = (rand() % 900) + 100;

    p = allocate(alg, s);

    deallocate(p);
  }

  double frag = average_frag();
  
  return frag;
}
