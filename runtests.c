#include <lib.h>
#include <minix/syslib.h>
#include <minix/type.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

/* Insertion sort */
void sort(double *hole_record, int size) {
    /* Declarations */
    int index;
    int inner_index;
    int inner_size;
    
    /* Instantiations */
    inner_size = size - 1;
    
    /* Begin insertion sort */
    for (index = 0; index < size - 1; ++index) {
        for (inner_index = 0; inner_index < inner_size; ++inner_index) {
            /* If out of order, swap */
            if (hole_record[inner_index] > hole_record[inner_index + 1]) {
                double temp;
                temp = hole_record[inner_index];
                hole_record[inner_index] = hole_record[inner_index + 1];
                hole_record[inner_index + 1] = temp;
            }
        }
        
        /* Decrement inner_size */
        inner_size = inner_size - 1;
    }
}

void print_record(int t) {
    /* Declarations */
    struct pm_mem_info pmi;
    int h;
    int total_size;
    int count;
    double hole_record[_NR_HOLES];
    
    /* Note: count will double as an index for hole_record */
    
    /* (Final answers) */
    double mean;
    double std_dev;
    double median;
    int b;
    /* (To be used when calculating std_dev) */
    int temp_sum;
    int variance;
    int temp_count;

    /* Instantiations */
    total_size = 0;
    count = 0;

    /* Get system info about memory allocations */
    getsysinfo(0, SI_MEM_ALLOC, &pmi);

    for (h = 0; h < _NR_HOLES; ++h) {
        if (pmi.pmi_holes[h].h_base && pmi.pmi_holes[h].h_len) {
            /* Declare variables */
            int bytes;
            double megabytes;
            
            /* DEBUG COMMENT */
            /* printf("base: %d\t\tlen: %d", pmi.pmi_holes[h].h_base, pmi.pmi_holes[h].h_len); */

            /* Generate sizes */
            bytes = (pmi.pmi_holes[h].h_len << CLICK_SHIFT);
            megabytes = (double)bytes / (1024 * 1024);

            /* DEBUG COMMENT */
            /* printf("\t\tmegabytes: %f\n", megabytes); */
            
            /* Add to total size */
            total_size = total_size + megabytes;
            
            /* Include in local list of holes (hole_record) */
            hole_record[count] = megabytes;
            
            /* Increment count */
            count = count + 1;
        }
    }
    
    /* Determine mean */
    mean = ((double)total_size / count);
    
    /* Determine median */
    sort(hole_record, count);
    
    if (count % 2 == 0) {
        /* Average two holes closest to center */
        median = (hole_record[count / 2] + hole_record[count / 2 + 1]) / 2;
    }
    else {
        /* Assign median the center hole */
        median = hole_record[count / 2];
    }
    
    /* Determine standard deviation */
    temp_sum = 0;
    
    for (temp_count = 0; temp_count < count; ++temp_count) {
        temp_sum = temp_sum + pow(hole_record[temp_count] - mean, 2.0);
    }
    
    /* Calculate the variance */
    variance = temp_sum / count;
    
    /* Assign the standard deviation */
    std_dev = sqrt(variance);
    
    /* Print out data */
    printf("%d\t%d\t%.2f\t%.2f\t%.2f\n", t, count, mean, std_dev, median);
}

int main(int argc, char **argv) {
    /* 't' represent the ID number for this run */
    int t;
    t = atoi(argv[1]);
    
    /* Print the record, including 't' in the output */
    print_record(t);
    
    return 0;
}
