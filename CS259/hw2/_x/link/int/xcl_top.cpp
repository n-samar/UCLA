#include "libspir_types.h"
#define EXPORT_PIPE_SYMBOLS 1
#include "cpu_pipes.h"
#undef EXPORT_PIPE_SYMBOLS
#include "xcl_half.h"
#include <cstddef>
#include <vector>
#include <pthread.h>

extern "C" {

void digitrec_kernel(unsigned long long test_image, size_t train_images, size_t knn_mat);

static pthread_mutex_t __xlnx_cl_digitrec_kernel_mutex = PTHREAD_MUTEX_INITIALIZER;
void __stub____xlnx_cl_digitrec_kernel(char **argv) {
  void **args = (void **)argv;
  unsigned long long test_image = *((unsigned long long*)args[0+1]);
  size_t train_images = *((size_t*)args[1+1]);
  size_t knn_mat = *((size_t*)args[2+1]);
  pthread_mutex_lock(&__xlnx_cl_digitrec_kernel_mutex);
  digitrec_kernel(test_image, train_images, knn_mat);
  pthread_mutex_unlock(&__xlnx_cl_digitrec_kernel_mutex);
}
}
