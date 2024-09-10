#include <stdio.h>
#include <math.h>

const double principle_d = 142184.85;
const float principle_f = 142184.85;
const double interestRate_d = (3.28/100);
const float interestRate_f = (3.28F/100);
const int TIME = 5;

float Mohan(float principle, float interest, int time);  //first banker
double Jeifan(double princple, double interest, int time); //second banker
float Linda(float principle, float interest, int time); //third banker
double Jeff(double principle, double interest, int time); //fourth banker

int main(){
    printf("Mohan approach sum: %f \n", Mohan(principle_f,
     interestRate_f, TIME));
    printf("Jeifan approach sum: %lf \n", Jeifan(principle_d,
     interestRate_d, TIME));
    printf("Linda approach sum: %f \n", Linda(principle_f,
     interestRate_f, TIME));
    printf("Jeff approach sum: %lf \n", Jeff(principle_d,
     interestRate_d, TIME));
    return 0;
}

float Mohan(float principle, float interest, int time){
    float sum = principle_f * powf((1 + interestRate_f), TIME);
    return sum;
}

double Jeifan(double principle, double interest, int time){
    double sum = principle * pow((1 + interest), TIME);
    return sum;
}

float Linda(float principle, float interest, int time){
    float sum = principle;
    for(int i = 0; i < time; i++){
        sum *= (1 + interest);
    } 
    return sum;
}

double Jeff(double principle, double interest, int time){
    double sum = principle;
    for(int i = 0; i < time; i++){
        sum *= (1 + interest);
    }
    return sum;
}
