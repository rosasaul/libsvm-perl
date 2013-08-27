#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include "svm.h"
#include "svm-binding.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))


//Binding Functions
struct svm_parameter* new_param() {
  struct svm_parameter *param = Malloc(struct svm_parameter,1);
  // default values
  param->svm_type = C_SVC;
  param->kernel_type = RBF;
  param->degree = 3;
  param->gamma = 0;  // 1/num_features
  param->coef0 = 0;
  param->nu = 0.5;
  param->cache_size = 100;
  param->C = 1;
  param->eps = 1e-3;
  param->p = 0.1;
  param->shrinking = 1;
  param->probability = 0;
  param->nr_weight = 0;
  param->weight_label = NULL;
  param->weight = NULL;
  return param;
}
void destroy_param(struct svm_parameter *param){
  free(param);
  malloc_trim(0);
}
void set_weight(struct svm_parameter *param, int label, double weight){
  param->nr_weight++;
  param->weight_label = (int *)realloc(param->weight_label,sizeof(int)*param->nr_weight);
  param->weight = (double *)realloc(param->weight,sizeof(double)*param->nr_weight);
  param->weight_label[param->nr_weight-1] = label;
  param->weight[param->nr_weight-1] = weight;
}
void set_svm_type(struct svm_parameter *param, int svm_type){
  param->svm_type = svm_type;
}
void set_kernel_type(struct svm_parameter *param, int kernel_type){
  param->kernel_type = kernel_type;
}
void set_degree(struct svm_parameter *param, int degree){
  param->degree = degree;
}
void set_gamma(struct svm_parameter *param, double gamma){
  param->gamma = gamma;
}
void set_coef0(struct svm_parameter *param, double coef0){
  param->coef0 = coef0;
}
void set_C(struct svm_parameter *param, double C){
  param->C = C;
}
void set_nu(struct svm_parameter *param, double nu){
  param->nu = nu;
}
void set_epsilon(struct svm_parameter *param, double p){
  param->p = p;
}

struct svm_problem* new_problem() {
  struct svm_problem *prob = Malloc(struct svm_problem,1);
  return prob;
}
void destroy_problem(struct svm_problem *prob){
  free(prob);
  malloc_trim(0);
}
svm_problem_set* new_problem_set(){
  svm_problem_set *problem_set = Malloc(svm_problem_set,1);
  problem_set->count = 0;
  problem_set->dataset = Malloc(svm_dataset,1);
  problem_set->dataset->last = -1;
  problem_set->prob = Malloc(struct svm_problem,1);
  problem_set->prob->l = 0;
  problem_set->x_space = NULL;
  return problem_set;
}
void destroy_problem_set(svm_problem_set *problem_set){
  svm_dataset *dataset_node = problem_set->dataset;
  svm_dataset *dataset_last_node;
  while(dataset_node->last != -1){
    dataset_last_node = dataset_node;
    dataset_node = dataset_node->node;
    destroy_dataset(dataset_last_node);
  }
  free(dataset_node); //Last Node doesn't need destroy_dataset since it's not a real node
  if(problem_set->x_space != NULL){
    free(problem_set->x_space);
  }
  free(problem_set->prob);
  free(problem_set);
  malloc_trim(0);
}

double do_cross_validation(svm_problem_set *problem_set, struct svm_parameter *param, double nr_fold){
  //Setup libsvm problem
  problem_set->prob->y = Malloc(double,problem_set->count);
  problem_set->prob->x = Malloc(struct svm_node *,problem_set->count);
  problem_set->prob->l = problem_set->count;
  int elements = 0;
  svm_dataset *dataset_node = problem_set->dataset;
  while(dataset_node->last != -1){
    elements += dataset_node->count + 1;
    dataset_node = dataset_node->node;
  }
  struct svm_node *x_space = Malloc(struct svm_node,elements);
  int i; int j = 0;
  int max_index = 0;
  dataset_node = problem_set->dataset;
  for(i=0; i < problem_set->count; i++){
    problem_set->prob->x[i] = &x_space[j];
    problem_set->prob->y[i] = dataset_node->label;
    svm_x_item *x_item_node = dataset_node->x_item;
    while(x_item_node->last != -1){
      if(max_index < x_item_node->index){
        max_index = x_item_node->index;
      }
      x_space[j].index = x_item_node->index;
      x_space[j].value = x_item_node->value;
      x_item_node = x_item_node->node;
      j++;
    }
    x_space[j++].index = -1;
    dataset_node = dataset_node->node;
  }
  if(param->gamma == 0 && max_index > 0){
    param->gamma = 1.0 / max_index;
  }
  svm_check_parameter(problem_set->prob, param);

  // Run Cross validation
  int total_correct = 0;
  double total_error = 0;
  double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0, accuracy = 0;
  double *target = Malloc(double,problem_set->count);
   
  svm_cross_validation(problem_set->prob,param,nr_fold,target);
  if(param->svm_type == EPSILON_SVR || param->svm_type == NU_SVR){
    for(i=0;i < problem_set->prob->l;i++){
      double y = problem_set->prob->y[i];
      double v = target[i];
      total_error += (v-y)*(v-y);
      sumv += v;
      sumy += y;
      sumvv += v*v;
      sumyy += y*y;
      sumvy += v*y;
    }
    accuracy = ((problem_set->prob->l*sumvy-sumv*sumy)*(problem_set->prob->l*sumvy-sumv*sumy)) /
      ((problem_set->prob->l*sumvv-sumv*sumv)*(problem_set->prob->l*sumyy-sumy*sumy));
  }
  else{
    for(i=0;i < problem_set->prob->l;i++){
      if(target[i] == problem_set->prob->y[i]){ ++total_correct; }
    }
    accuracy = (double) total_correct / (double) problem_set->prob->l;
  }
  free(target);
  return accuracy;
}

svm_dataset* new_dataset(double label){
  svm_dataset *dataset = Malloc(svm_dataset,1);
  dataset->label = label;
  dataset->count = 0;
  dataset->x_item =  Malloc(svm_x_item,1);
  dataset->x_item->last = -1;
  return dataset;
}
void destroy_dataset(svm_dataset *dataset){
  svm_x_item *x_item_node = dataset->x_item;
  svm_x_item *x_item_last_node;
  while(x_item_node->last != -1){
    x_item_last_node = x_item_node;
    x_item_node = x_item_node->node;
    free(x_item_last_node);
  }
  free(x_item_node);
  free(dataset);
  malloc_trim(0);
}
void set_attribute(svm_dataset *dataset, int index, double value){
  svm_x_item *x_item_node = dataset->x_item;
  while(x_item_node->last != -1 && x_item_node->index < index){
    x_item_node = x_item_node->node;
  }
  if(x_item_node->last == -1){
    x_item_node->last = 0;
    x_item_node->index = index;
    x_item_node->value = value;

    x_item_node->node = Malloc(svm_x_item,1);
    x_item_node->node->last = -1;
  }
  else if(x_item_node->index == index){
    x_item_node->index = index;
    x_item_node->value = value;
    return;
  }
  else{
    svm_x_item *insert_x_item_node = Malloc(svm_x_item,1);
    insert_x_item_node->index = x_item_node->index;
    insert_x_item_node->value = x_item_node->value;
    insert_x_item_node->node = x_item_node->node;

    x_item_node->index = index;
    x_item_node->value = value;
    x_item_node->node = insert_x_item_node;
  }
  dataset->count++;
}
double predict(struct svm_model *model, svm_dataset *dataset){
  struct svm_node *x = Malloc(struct svm_node, (1 + dataset->count));
  svm_x_item *x_item = dataset->x_item;
  int i;
  for(i=0; i < dataset->count; i++){
    if(x_item->last == -1){
      fprintf(stderr,"WARNING: Dataset ended early\n");
      break;
    }
    x[i].index = x_item->index;
    x[i].value = x_item->value;
    x_item = x_item->node;
  }
  x[i].index = -1;
  double y = svm_predict(model,x);
  free(x);
  return y;
}
struct svm_model *load_model(char *model_file_name){
  return svm_load_model(model_file_name);
}

void destroy_model(struct svm_model *model){
  svm_free_and_destroy_model(&model);
  malloc_trim(0);
}

void save_model_file(char *model_file_name, struct svm_model *model){
  if(svm_save_model(model_file_name,model)){
    fprintf(stderr, "ERROR: Could not save model file %s\n", model_file_name);
    exit(1);
  }
}

void add_dataset(svm_problem_set *problem_set, svm_dataset *dataset){
  dataset->node = problem_set->dataset;
  problem_set->dataset = dataset;
  problem_set->count++;
}

struct svm_model* train(svm_problem_set *problem_set, struct svm_parameter *param){
  //Setup libsvm problem
  problem_set->prob->y = Malloc(double,problem_set->count);
  problem_set->prob->x = Malloc(struct svm_node *,problem_set->count);
  problem_set->prob->l = problem_set->count;
  int elements = 0;
  svm_dataset *dataset_node = problem_set->dataset;
  while(dataset_node->last != -1){
    elements += dataset_node->count + 1;
    dataset_node = dataset_node->node;
  }
  struct svm_node *x_space = Malloc(struct svm_node,elements);
  int i; int j = 0;
  int max_index = 0;
  dataset_node = problem_set->dataset;
  for(i=0; i < problem_set->count; i++){
    problem_set->prob->x[i] = &x_space[j];
    problem_set->prob->y[i] = dataset_node->label;
    svm_x_item *x_item_node = dataset_node->x_item;
    while(x_item_node->last != -1){
      if(max_index < x_item_node->index){
        max_index = x_item_node->index;
      }
      x_space[j].index = x_item_node->index;
      x_space[j].value = x_item_node->value;
      x_item_node = x_item_node->node;
      j++;
    }
    x_space[j++].index = -1;
    dataset_node = dataset_node->node;
  }
  if(param->gamma == 0 && max_index > 0){
    param->gamma = 1.0 / max_index;
  }
  svm_check_parameter(problem_set->prob, param);
  struct svm_model *model = svm_train(problem_set->prob, param);
  return model;
}


