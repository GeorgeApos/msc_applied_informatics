#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int N;

double jacobi_over_relaxation(bool method, double A[N][N], double b[N], double x[N], double omega, double tolerance) {
    double x_new[N];
    double error;
    int i, j, iter = 0;

    do {
        for (i = 0; i < N; i++) {
            double sigma = 0.0;
            for (j = 0; j < N; j++) {
                if (j != i) {
                    sigma += A[i][j] * x[j];
                }
            }
            x_new[i] = (1 - omega) * x[i] + (omega / A[i][i]) * (b[i] - sigma);
        }

        error = 0.0;
        for (i = 0; i < N; i++) {
            double diff = fabs(x_new[i] - x[i]);
            if (diff > error) {
                error = diff;
            }
            x[i] = x_new[i];
        }

        iter++;
    } while (error > tolerance);

    if(!method) {
        printf("Jacobi Over-Relaxation: Converged in %d iterations\n", iter);
    }

    return iter;
}

double successive_over_relaxation(bool method, double A[N][N], double b[N], double x[N], double omega, double tolerance) {
    double x_new[N];
    double error;
    int i, j, iter = 0;

    do {
        for (i = 0; i < N; i++) {
            double sum1 = 0.0, sum2 = 0.0;
            for (j = 0; j < i; j++) {
                sum1 += A[i][j] * x_new[j];
            }
            for (j = i + 1; j < N; j++) {
                sum2 += A[i][j] * x[j];
            }
            x_new[i] = (1 - omega) * x[i] + (omega / A[i][i]) * (b[i] - sum1 - sum2);
        }

        error = 0.0;
        for (i = 0; i < N; i++) {
            double diff = fabs(x_new[i] - x[i]);
            if (diff > error) {
                error = diff;
            }
            x[i] = x_new[i];
        }

        iter++;
    } while (error > tolerance && iter < 1000);

    if (!method) {
        printf("Successive Over-Relaxation: Converged in %d iterations\n", iter);
    }

    return iter;
}


double compute_optimal_omega(double A[N][N]) {
    double max_eigenvalue = -1;
    double trace = 0;

    for (int i = 0; i < N; i++) {
        trace += A[i][i];
        double row_sum = 0;
        for (int j = 0; j < N; j++) {
            row_sum += fabs(A[i][j]);
        }
        if (row_sum > max_eigenvalue) {
            max_eigenvalue = row_sum;
        }
    }

    if (trace < max_eigenvalue) {
        printf("Warning: The matrix A might not converge. Consider using a different method.\n");
    }

    return 2 / (1 + sqrt(1 - pow(trace / max_eigenvalue, 2)));
}

int main2() {
    printf("Enter the value of N: ");
    scanf("%d", &N);

    double A[N][N];
    double b[N]; // Removed initialization
    double x[N]; // Removed initialization
    double optimal_omega = compute_optimal_omega(A);
    double tolerance = 1e-7;

    // Construct the A matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                A[i][j] = 4;
            } else if (abs(i - j) == 1) {
                A[i][j] = -1;
            } else {
                A[i][j] = 0;
            }
        }
        b[i] = 1; // Initialize b to 1
        x[i] = 0; // Initialize x to 0
    }

    jacobi_over_relaxation(false, A, b, x, optimal_omega, tolerance);

    printf("Solution using Jacobi Over-Relaxation:\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %f\n", i, x[i]);
    }

    printf("\n");

    successive_over_relaxation(false, A, b, x, optimal_omega, tolerance);

    printf("Solution using Successive Over-Relaxation:\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %f\n", i, x[i]);
    }

    printf("Optimal omega: %f\n", optimal_omega);

    return 0;
}
