#ifndef _SVM_BINDING_H
#define _SVM_BINDING_H
#include "svm.h"

//Data Structure for dynamic allocation
typedef struct svm_x_item_node {
  int last; //set to -1 to identify last in series
  int index;
  double value;
  struct svm_x_item_node *node;
} svm_x_item;

typedef struct svm_dataset_node {
  int label;
  int count;
  int last; //set to -1 to identify last in series
  svm_x_item *x_item; //Stores the data for this dataset linked list
  struct svm_dataset_node *node; //Linkage for group of datasets
} svm_dataset;

typedef struct {
  int count;
  struct svm_problem *prob; //libsvm format
  struct svm_node *x_space; //Store link to xspace used in train to free
  svm_dataset *dataset; //linked list of datasets
} svm_problem_set;

//Binding Functions Definitions
struct svm_parameter* new_param();
void destroy_param(struct svm_parameter *param);
void set_svm_type(struct svm_parameter *param, int svm_type);
void set_kernel_type(struct svm_parameter *param, int kernel_type);
void set_degree(struct svm_parameter *param, int degree);
void set_gamma(struct svm_parameter *param, double gamma);
void set_coef0(struct svm_parameter *param, double coef0);
void set_C(struct svm_parameter *param, double C);
void set_nu(struct svm_parameter *param, double nu);
void set_epsilon(struct svm_parameter *param, double p);
struct svm_problem* new_problem();
void destroy_problem(struct svm_problem *prob);
svm_problem_set* new_problem_set();
void destroy_problem_set(svm_problem_set *problem_set);
svm_dataset* new_dataset(double label);
void destroy_dataset(svm_dataset *dataset);
void set_attribute(svm_dataset *dataset, int index, double value);
double predict(struct svm_model *model, svm_dataset *dataset);
struct svm_model *load_model(char *model_file_name);
void destroy_model(struct svm_model *model);
void save_model_file(char *model_file_name, struct svm_model *model);
void add_dataset(svm_problem_set *problem_set, svm_dataset *dataset);
struct svm_model* train(svm_problem_set *problem_set, struct svm_parameter *param);

#endif
