#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>

const int kBurstLength = 225; // 256;    // Number of samples read per digit per burst
const int TOTAL_SAMPLES_PER_DIGIT = 1800;
int BATCH_NUM = TOTAL_SAMPLES_PER_DIGIT / kBurstLength;

void Load(unsigned long* local_train, const unsigned long* train_images) {
#pragma HLS inline off
load:
  for (int digit = 0; digit < 10; ++digit) {    
    for (int sample = 0; sample < kBurstLength; ++sample) {
      local_train[sample + (digit * kBurstLength)] = train_images[sample + (digit * TOTAL_SAMPLES_PER_DIGIT)];
    }
  }
}

void Compute(const unsigned long test_image, 
	     const unsigned long* train_images, 
	     unsigned char* knn_mat) {
#pragma HLS inline off
compute:
  // Compute the difference using XOR
  unsigned long temp[10 * kBurstLength];
diff:
  for (int x1 = 0; x1 < 10; ++x1) {
    for (int y1 = 0; y1 < kBurstLength; ++y1) {
      temp[(y1 + (x1 * kBurstLength))] = train_images[(y1 + (x1 * kBurstLength))] ^ test_image;
    }
  }

  // Compute the distance
dis:
  for (int x2 = 0; x2 < 10; ++x2) {
    for (int y2 = 0; y2 < kBurstLength; ++y2) {
      unsigned long dis = 0;
      for (int i = 0; i < 49; ++i) {
        dis += (temp[y2 + (x2 * kBurstLength)] & (1L << i)) >> i;
      }
      temp[y2 + (x2 * kBurstLength)] = dis;
    }
  }

  // Update knn set
  // Nikola: (this gives top 3 closest matches for each digit, I guess?)
update:
  for (int x3 = 0; x3 < 10; ++x3) {
    for (int y3 = 0; y3 < kBurstLength; ++y3) {
      unsigned long max_id = 0;
      for (int i1 = 0; i1 < 3; ++i1) {
        if (knn_mat[max_id + (x3 * 3)] < knn_mat[i1 + (x3 * 3)]) {
          max_id = i1;
        }
      }
      if (temp[y3 + (x3 * kBurstLength)] < knn_mat[max_id + (x3 * 3)]) {
        knn_mat[max_id + (x3 * 3)] = temp[y3 + (x3 * kBurstLength)];
      }
    }
  }
}

extern "C" {

void digitrec_kernel(
    unsigned long test_image,
    unsigned long* train_images,
    unsigned char* knn_mat) {
#pragma HLS interface m_axi port=train_images offset=slave bundle=gmem
#pragma HLS interface m_axi port=knn_mat offset=slave bundle=gmem2
#pragma HLS interface s_axilite port=test_image bundle=control
#pragma HLS interface s_axilite port=train_images bundle=control
#pragma HLS interface s_axilite port=knn_mat bundle=control
#pragma HLS interface s_axilite port=return bundle=control

  unsigned int a;

  // Initialize the knn set
init:
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 3; ++y) {
      // Note that the max distance is 49
      knn_mat[(y + (x * 3))] = (unsigned char)50;
    }
  }
  
  unsigned long local_train[kBurstLength * 10];
  for (int batch_index = 0; batch_index < BATCH_NUM; batch_index++) {
    Load(local_train, train_images + batch_index * kBurstLength);
    Compute(test_image, local_train, knn_mat);
  }
}

} // extern "C"
