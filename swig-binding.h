/* svm_bindings.i */

extern struct svm_parameter* new_param();
extern void destroy_param(struct svm_parameter *param);
extern void destroy_param(struct svm_parameter *param);
extern void set_weight(struct svm_parameter *param, int label, double weight);
extern void set_svm_type(struct svm_parameter *param, int svm_type);
extern void set_kernel_type(struct svm_parameter *param, int kernel_type);
extern void set_degree(struct svm_parameter *param, int degree);
extern void set_gamma(struct svm_parameter *param, double gamma);
extern void set_coef0(struct svm_parameter *param, double coef0);
extern void set_C(struct svm_parameter *param, double C);
extern void set_nu(struct svm_parameter *param, double nu);
extern void set_epsilon(struct svm_parameter *param, double p);
extern struct svm_problem* new_problem();
extern void destroy_problem(struct svm_problem *prob);
extern svm_problem_set* new_problem_set();
double do_cross_validation(svm_problem_set *problem_set, struct svm_parameter *param, double nr_fold);
extern void destroy_problem_set(svm_problem_set *problem_set);
extern svm_dataset* new_dataset(double label);
extern void destroy_dataset(svm_dataset *dataset);
extern void set_attribute(svm_dataset *dataset, int index, double value);
extern double predict(struct svm_model *model, svm_dataset *dataset);
extern struct svm_model *load_model(char *model_file_name);
extern void destroy_model(struct svm_model *model);
extern void save_model_file(char *model_file_name, struct svm_model *model);
extern void add_dataset(svm_problem_set *problem_set, svm_dataset *dataset);
extern struct svm_model* train(svm_problem_set *problem_set, struct svm_parameter *param);

