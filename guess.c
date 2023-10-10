#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Here we are define the maximum number of players that can be stored in the file
#define MaxPlayers 5

//Now we create a struct with name and guesses as the characteristics of the player
struct Player {
    char name[50];
    int guesses;
};

/*---------------------------------------------------------------------------------------
readPlayers
This method is used to read the players from the file. It opens the players.txt file
and if the file is NULL, then the number of leaders are 0 and it returns that value.
If the file does have something to read, when the players are less than 5, the code
reads the player's names and the number of guesses they took to get to the correct answer.
 ---------------------------------------------------------------------------------------*/
void readPlayers(struct Player players[], int *numPlayers) {
	//Here we open and read from the file players.txt
    FILE* stats = fopen("leaders.txt", "r");
    //if the file is null, then the number of players is 0
    if (stats == NULL) {
        *numPlayers = 0;
        return;
    }

    *numPlayers = 0;
    while (*numPlayers < MaxPlayers) {
    	if(fscanf(stats, "%s %d", players[*numPlayers].name, &players[*numPlayers].guesses) != 2){
    		break;
    	}
    	(*numPlayers)++;
    }
    fclose(stats);
}

/*---------------------------------------------------------------------------------------
displayPlayers
This method is used to display all the players in the text file. It uses a loop to go
through the file and display the player's name and the number of guesses each player took.
 ---------------------------------------------------------------------------------------*/
void displayPlayers(struct Player players[], int numPlayers) {
    printf("Here are the current leaders:\n");
    for (int i = 0; i < numPlayers; i++) {
        printf("%s made %d guesses\n", players[i].name, players[i].guesses);
    }
}

/*---------------------------------------------------------------------------------------
savePlayers
This method uses the "w" to write into the players.txt file. If the file is null, the the
program will return, "Unable to save players to file". If we do wanna save the player's
details, the we use the for loop to write into the file. After we're done writing, we close
the file.
 ---------------------------------------------------------------------------------------*/
void savePlayers(struct Player leaders[], int numLeaders) {
	//We now open and write in the file.
    FILE *file = fopen("leaders.txt", "w");
    if (file == NULL) {
        printf("Unable to save players to file.\n");
        return;
    }
    //Loop to write data into the file
    for (int i = 0; i < numLeaders; i++) {
        fprintf(file, "%s %d\n", leaders[i].name, leaders[i].guesses);
    }
    fclose(file);
}

/*---------------------------------------------------------------------------------------
main
This method is where the code starts working. First we display a welcome message to the user. Then,
we ask them if they want to quit or start the game. After that, if the player wants to play,
then we ask them for their name. Then the code gives them a number and asks the player to guess.
Then, it asks if you want to play again. If that's a yes, then it prompts the user for their
name and continues the game until the user decides to quit. Then the code also orders the players
to display the leader board. The number of guesses are then ordered in the ascending order.
 ---------------------------------------------------------------------------------------*/
int main() {
    char input;
    struct Player player[MaxPlayers];
    int numPlayers = 0;
    int welcomeText = 0;

    //Reading the data in the file
    readPlayers(player, &numPlayers);

    while (1) {
    	//To make sure that welcome is played in the beginning
        if (!welcomeText) {
            printf("Welcome!");
            welcomeText = 1;
        }

        //Now prompts the user to quit or if they want to continue.
        printf("Press 'q' to quit or any other key to continue:\n");
        scanf(" %c", &input);
        //If the input is q, then we quit the game
        if (input == 'q') {
            savePlayers(player, numPlayers);
            printf("Bye Bye!\n");
            break;
        }

        //If the input is any other key and not q
        struct Player currentPlayer;
        printf("Please enter your name to start: ");
        scanf("%s", currentPlayer.name);
        currentPlayer.guesses = 0;

        //Generating a random integer
        srand(time(0));
        double target = rand() % 91 + 10;
        double guess;
        double roott = sqrt(target);

        //Give the user the number in the question
        printf("%f is the square root of what number? Guess a value between 10 and 100: ", roott);

        //This is where we check if the input the user gave it too high or too low
        while (1) {
            scanf("%lf", &guess);
            //To keep track of the guesses
            currentPlayer.guesses++;

            if (guess < target) {
                printf("Too low, guess again: ");
            }
            else if (guess > target) {
                printf("Too high, guess again: ");
            }
            else {
                printf("You got it, baby!\n");
                printf("You made %d guesses.\n", currentPlayer.guesses);

                //Now we order the players based on their number of guesses in ascending order
                int i;
                for (i = 0; i < numPlayers; i++) {
                	if (currentPlayer.guesses < player[i].guesses) {
                		break;
                	}
                }
                for (int j = numPlayers; j > i; j--) {
                	player[j] = player[j-1];
                }
                player[i] = currentPlayer;
                if (numPlayers < MaxPlayers) {
                	numPlayers++;
                                }
                //Now we display the players, returned from the method.
                displayPlayers(player, numPlayers);
                break;
            }
        }
    }
    return 0;
}
