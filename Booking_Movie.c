#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 10
#define MAX_MOVIES 2
#define MAX_SHOWTIMES 3
#define MAX_SEATS 10

// Customer structure to store customer information
struct Customer {
    char username[50];
    char password[50];
    int bookedMovie;  // Index of the booked movie
    int bookedShowtime;  // Index of the booked showtime
    int bookedSeats[MAX_SEATS];  // Seat numbers booked by the customer
};

// Movie structure to store movie information
struct Movie {
    char title[50];
    char showtimes[MAX_SHOWTIMES][10];
    int seats[MAX_SHOWTIMES][MAX_SEATS];
    float cost;  // New field for the cost of the movie
};

int customerCount = 0;
struct Customer customers[MAX_CUSTOMERS];

// Function to display movie showtimes
void displayShowtimes(struct Movie movie) {
    printf("Showtimes for %s:\n", movie.title);
    for (int i = 0; i < MAX_SHOWTIMES; i++) {
        printf("%d. %s\n", i + 1, movie.showtimes[i]);
    }
}

// Function to display available seats visually
void displaySeats(struct Movie movie, int showtimeIdx) {
    if (showtimeIdx < 0 || showtimeIdx >= MAX_SHOWTIMES) {
        printf("Invalid showtime selection.\n");
        return;
    }

    printf("Seat Layout for %s at %s:\n", movie.title, movie.showtimes[showtimeIdx]);

    for (int i = 0; i < MAX_SEATS; i++) {
        if (movie.seats[showtimeIdx][i] == 0) {
            printf("[X] ");  // X represents a booked seat
        } else {
            printf("[ ] ");  // [ ] represents an available seat
        }
    }

    printf("\n");
}

// Function to book movie tickets
void bookTicket(struct Customer* customer, struct Movie* movie, int showtimeIdx, int numTickets) {
    if (showtimeIdx < 0 || showtimeIdx >= MAX_SHOWTIMES) {
        printf("Invalid showtime selection.\n");
        return;
    }

    int availableSeats = 0;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (movie->seats[showtimeIdx][i] == 1) {
            availableSeats++;
        }
    }

    if (numTickets <= 0 || numTickets > availableSeats) {
        printf("Invalid number of tickets or not enough seats available.\n");
        return;
    }

    printf("Select seat(s) to book (e.g., 1 3 5): ");
    float totalCost = 0.0;  // Variable to store the total cost
    for (int i = 0; i < numTickets; i++) {
        int seatNum;
        scanf("%d", &seatNum);
        if (seatNum < 1 || seatNum > MAX_SEATS || movie->seats[showtimeIdx][seatNum - 1] == 0) {
            printf("Invalid seat selection.\n");
            return;
        }
        movie->seats[showtimeIdx][seatNum - 1] = 0;  // Mark the seat as booked
        customer->bookedSeats[i] = seatNum;
        totalCost += movie->cost;  // Accumulate the cost for each booked seat
    }
    customer->bookedMovie = showtimeIdx;
    customer->bookedShowtime = numTickets;

    printf("Successfully booked %d ticket(s) for %s at %s.\n", numTickets, movie->title, movie->showtimes[showtimeIdx]);
    printf("Total Cost: $%.2f\n", totalCost);  // Display the total cost
}

// Function to cancel specific seats
void cancelSpecificSeats(struct Customer* customer, struct Movie* movie, int showtimeIdx) {
    if (showtimeIdx < 0 || showtimeIdx >= MAX_SHOWTIMES) {
        printf("Invalid showtime selection.\n");
        return;
    }

    if (customer->bookedMovie == -1 || customer->bookedMovie != showtimeIdx) {
        printf("You have no booked seats for this showtime.\n");
        return;
    }

    printf("Your booked seats for %s at %s: ", movie->title, movie->showtimes[showtimeIdx]);

    for (int i = 0; i < customer->bookedShowtime; i++) {
        int seatNum = customer->bookedSeats[i];
        printf("%d ", seatNum);
        movie->seats[showtimeIdx][seatNum - 1] = 1;  // Mark the seat as available
    }

    printf("\n");
    customer->bookedMovie = -1;
    customer->bookedShowtime = 0;
    printf("Successfully canceled your booked seats.\n");
}

// Function to find a customer by username
int findCustomerByUsername(const char* username) {
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].username, username) == 0) {
            return i;
        }
    }
    return -1;  // Customer not found
}

// Function to create a new customer
void createCustomer(const char* username, const char* password) {
    if (customerCount < MAX_CUSTOMERS) {
        strcpy(customers[customerCount].username, username);
        strcpy(customers[customerCount].password, password);
        customers[customerCount].bookedMovie = -1;  // No bookings initially
        customers[customerCount].bookedShowtime = 0;
        customerCount++;
        printf("Account created successfully.\n");
    } else {
        printf("Maximum number of customers reached.\n");
    }
}

int main() {
    struct Movie movies[MAX_MOVIES];
    strcpy(movies[0].title, "Movie A");
    strcpy(movies[1].title, "Movie B");

    strcpy(movies[0].showtimes[0], "10:00 AM");
    strcpy(movies[0].showtimes[1], "2:00 PM");
    strcpy(movies[0].showtimes[2], "6:00 PM");

    strcpy(movies[1].showtimes[0], "11:00 AM");
    strcpy(movies[1].showtimes[1], "3:00 PM");
    strcpy(movies[1].showtimes[2], "7:00 PM");

    // Initialize movie costs
    movies[0].cost = 100.0;  // Cost for Movie A
    movies[1].cost = 150.0;  // Cost for Movie B

    for (int i = 0; i < MAX_MOVIES; i++) {
        for (int j = 0; j < MAX_SHOWTIMES; j++) {
            for (int k = 0; k < MAX_SEATS; k++) {
                movies[i].seats[j][k] = 1; // Initialize all seats as available
            }
        }
    }

    while (1) {
        printf("\n1. Login\n2. Create an Account\n0. Quit\nSelect an option: ");
        int option;
        scanf("%d", &option);

        if (option == 0) {
            break;
        } else if (option == 1) {
            char username[50];
            char password[50];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            int customerIndex = findCustomerByUsername(username);
            if (customerIndex != -1 && strcmp(customers[customerIndex].password, password) == 0) {
                while (1) {
                    printf("\nWelcome, %s!\n", username);
                    printf("1. View Movies\n2. Cancel Booked Seats\n0. Logout\nSelect an option: ");
                    scanf("%d", &option);

                    if (option == 0) {
                        break;
                    } else if (option == 1) {
                        printf("\nMovie Listings:\n");
                        for (int i = 0; i < MAX_MOVIES; i++) {
                            printf("%d. %s\n", i + 1, movies[i].title);
                        }

                        int choice;
                        printf("Select a movie (1, 2, or 0 to go back): ");
                        scanf("%d", &choice);

                        if (choice == 0) {
                            continue;
                        }

                        if (choice >= 1 && choice <= MAX_MOVIES) {
                            int showtimeIdx;
                            displayShowtimes(movies[choice - 1]);
                            printf("Select a showtime (1, 2, 3, or 0 to go back): ");
                            scanf("%d", &showtimeIdx);

                            if (showtimeIdx == 0) {
                                continue;
                            }

                            if (showtimeIdx >= 1 && showtimeIdx <= MAX_SHOWTIMES) {
                                int action;
                                printf("1. Book Tickets\n2. Display Available Seats\n3. Cancel Booked Seats\nSelect an action: ");
                                scanf("%d", &action);

                                if (action == 1) {
                                    int numTickets;
                                    printf("Enter the number of tickets to book: ");
                                    scanf("%d", &numTickets);
                                    bookTicket(&customers[customerIndex], &movies[choice - 1], showtimeIdx - 1, numTickets);
                                } else if (action == 2) {
                                    displaySeats(movies[choice - 1], showtimeIdx - 1);
                                } else if (action == 3) {
                                    cancelSpecificSeats(&customers[customerIndex], &movies[choice - 1], showtimeIdx - 1);
                                } else {
                                    printf("Invalid action. Please select a valid action.\n");
                                }
                            } else {
                                printf("Invalid showtime selection.\n");
                            }
                        } else {
                            printf("Invalid choice. Please select a valid movie.\n");
                        }
                    } else if (option == 2) {
                        if (customers[customerIndex].bookedMovie == -1) {
                            printf("You have no booked seats.\n");
                        } else {
                            printf("Please select a showtime to cancel booked seats.\n");
                            int showtimeIdx;
                            displayShowtimes(movies[customers[customerIndex].bookedMovie]);
                            printf("Select a showtime (1, 2, 3, or 0 to go back): ");
                            scanf("%d", &showtimeIdx);

                            if (showtimeIdx == 0) {
                                continue;
                            }

                            if (showtimeIdx >= 1 && showtimeIdx <= MAX_SHOWTIMES) {
                                cancelSpecificSeats(&customers[customerIndex], &movies[customers[customerIndex].bookedMovie], showtimeIdx - 1);
                            } else {
                                printf("Invalid showtime selection.\n");
                            }
                        }
                    } else {
                        printf("Invalid option. Please select a valid option.\n");
                    }
                }
            } else {
                printf("Invalid login credentials.\n");
            }
        } else if (option == 2) {
            char newUsername[50];
            char newPassword[50];
            printf("Enter a new username: ");
            scanf("%s", newUsername);
            printf("Enter a new password: ");
            scanf("%s", newPassword);

            int existingCustomerIndex = findCustomerByUsername(newUsername);
            if (existingCustomerIndex != -1) {
                printf("Username already exists. Please choose a different username.\n");
            } else {
                createCustomer(newUsername, newPassword);
            }
        } else {
            printf("Invalid option. Please select a valid option.\n");
        }
    }

    return 0;
}
