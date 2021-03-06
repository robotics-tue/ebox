/* 
 *TEST02    Test FFT using FFTW library (i.e. generate data and compare 
 *          result with matlab). Real data is assumed.
 *   
 *   Usage: Run from seperate console, don't call directly from matlab
 *          using ./test02
 *   
 *   Inputs: none  
 *   
 *   Outputs: input_data.txt and output_data.txt
 *   
 *   See also 

 *   References: 
 *   
 *   Changes: 
 *   20110907 - Initial version (M.J.C. Ronde) 
 *   
 *   
 *   M.J.C. Ronde (2011-09-07) 
 *   ________________________________
 *   Eindhoven University of Technology
 *   Dept. of Mechanical Engineering
 *   Control Systems Technology group
 *   PO Box 513, WH -1.127
 *   5600 MB Eindhoven, NL
 *   T +31 (0)40 247 2798
 *   F +31 (0)40 246 1418
 *   E m.j.c.ronde@tue.nl
 */ 
 
#include <stdio.h> 
#include <stdlib.h>
#include <fftw3.h> 
#include <math.h> 

#include "timer.h"
 
 
#define N 1024
 
int main(int argc, char **argv) 
{ 
    FILE *fp_in, *fp_out;
    int i,n2;
    double temp,t0,t1,*in;
     
    fftw_complex *out,*out_shift;   
    fftw_plan p; 
   
    in = malloc(sizeof(double) * N);
    /* (floor((double) N/2.0)+1) is sufficient */
    out         = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N); 
    out_shift   = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);

    
    //     in = fftw_alloc_complex(N); 
    //     out = fftw_alloc_complex(N);
    p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE); 
 
    
    /* Create input data */
    for(i = 0; i < N; ++i){ 
        temp = (double) i/ (double) N; 
        in[i] = sin(temp*2*M_PI*5); 
        out[i][0] = 0.0; 
        out[i][1] = 0.0; 
    } 
     
    /* Print input data */ 
    for( i = 0 ; i < N ; i++ ) { 
        //printf("data[%d] = { %2.2f, %2.2f }\n",i, in[i][0], in[i][1] ); 
    } 
    
    fp_in = fopen("input_data.txt","w"); 
    if(fp_in == NULL){
        printf("Opening file failed\n");
        return -1;        
    } else {
         /* Write to file*/
        for( i = 0 ; i < N ; i++ ) {
            temp = (double) i/ (double) N;
            fprintf(fp_in,"%.15e\n",sin(temp*2*M_PI*5)); 
        } 
        fclose(fp_in);
    }
     
    t0 = rtc_get_time();
    fftw_execute(p); 
    t1 = rtc_get_time();
    printf("DFT took %e \n",t1-t0);
     
    
    /* Symmetry for DFTs of real data */
    for(i = (N/2+1); i < N; i++){
        out[i][0] =  out[N-i][0];
        out[i][1] = -out[N-i][1];
    }
    
    /* Generate shifted version */
    n2 = ceil(N/2);
    for( i = 0 ; i < N ; i++ ) {
        if(i < n2){
            out_shift[i][0] = out[n2+i][0];
            out_shift[i][1] = out[n2+i][1];
//            printf("%d\n",n2+i);
        } else {
            out_shift[i][0] = out[i-n2][0];
            out_shift[i][1] = out[i-n2][1];
//            printf("%d\n",i-n2);
        }
    }
    
    /* Print output data */
    for(i = 0; i < N; i++){ 
       //printf("%d %e %ei\n",i,out[i][0],out[i][1]); 
    }
    
    
    fp_out = fopen("output_data.txt","w");
    if(fp_out == NULL){
        printf("Opening file failed\n");
        return -1;
    } else {
        for(i = 0; i < N; i++){
            fprintf(fp_out,"%d %.15e %.15e\n",i,out_shift[i][0],out_shift[i][1]); 
        } 
        fclose(fp_out);
    }
    
    
    /* Clean variables */ 
    fftw_destroy_plan(p); 
  
    fftw_free(in); 
    fftw_free(out); 
 
    return 0; 
} 
