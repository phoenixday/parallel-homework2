#include "mpi.h"
#include <random>

int get_random(int color) {
    if (color == 0) return 0;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1); // define the range
    return distr(gen);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); //initialize MPI library
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) printf("485739\n");

    MPI_Comm new_comm;
    int round = 0, color = 1, new_color, new_comm_size;

    do {
        do {
            new_color = get_random(color);
            MPI_Comm_split(MPI_COMM_WORLD, new_color, world_rank, &new_comm);
            MPI_Comm_size(new_comm, &new_comm_size);
        } while (new_comm_size == 6 && new_color == 0);
        color = new_color;
        round++;
    } while(!((new_comm_size == 5 && color == 0) || (new_comm_size == 1 && color == 1)));

    MPI_Request request;
    if (color == 1) {
        int winner_rank = world_rank;
        MPI_Isend(&winner_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    }
    int winner = 0;
    if (world_rank == 0) {
        MPI_Irecv(&winner, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }

    MPI_Bcast(&winner, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Sloužím ti, můj vládče, slunce naše jasné.%d\n", winner);

    MPI_Comm_free(&new_comm);
    MPI_Finalize(); //MPI cleanup
    return 0;
}
