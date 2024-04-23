#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define BOARDSIZEY 21
#define BOARDSIZEX 10
#define PLAYERSNUMBER 2
#define MAXTOTALSHIPSNUMBER 10
#define MAINBUFFORSIZE 20
#define BUFFORSIZE 20
#define CARSIZE 5
#define BATSIZE 4
#define CRUSIZE 3
#define DESSIZE 2

struct player_t {
    bool isSetFleetUsed = false;
    bool isInitPositionUsed = false;
    bool isPlayerXUsed = false;
    bool isPlayersMove = false; //as above but it can be used in [state] too
    bool playerXwasntUsedYet = true;
    bool placedAllShips = false;
    struct {
        int startY = 0;
        int startX = 0;
        int endY = 21;
        int endX = 10;
    }playersBoard_t;
    struct {
        int id;
        int type;
        int player;
    }temporary_info_t;
    struct {
        int CARamount = 1;
        int BATamount = 2;
        int CRUamount = 3;
        int DESamount = 4;
        int numberOfShipsParts = CARamount * CARSIZE + BATamount * BATSIZE + CRUamount * CRUSIZE + DESamount * DESSIZE;
        int shipParts = 0;
    }shipInfo_t;
    struct {
        int x1[10];
        int y1[10];
        int x2[10];
        int y2[10];
        char destination[10];
        int damage[10];
        int moveCounter[10];
    }ship_t[4];
}player[PLAYERSNUMBER]{};

struct {
    int x = 0;
    int y = 0;
    bool used = false;
}reef_t[BOARDSIZEY * BOARDSIZEX];

void conditionsForBOARDSIZE();
int whichPlayer();
const char* shipsName(int C);
//it resets counter of moves done by ship
void resetCounter();
void saveState();
void printDMG(int C, int dmg);
void printDMGdigits(int dmg, int r);
void setFleet(char P, char** board);
int IDforReefs();
char** creatingTheBoard();
//printing function/it masks ship parts that are used in an extended version of the game
void printTheBasicBoard(char** board);
//placing ship function
void placingSegmentsSelector(int y, int x, int C, char D, char** board);
void placingShip(int y, int x, char D, int id, int C, char** board);
//placing ship function
void placeShip(int y, int x, char D, int id, int C, char** board);
void placeReef(int y, int x, char** board);
//ship function
void makeXinTheShipFunction(int y, int x, char D, int whereXshouldBe, char** board);
void makeDmgInTheShipFunction(char P, int y, int x, char D, int id, int C, int dmg, char** board);
bool isItZero(int dmg, int r);
void shipFunction(char P, int y, int x, char D, int id, int C, int dmg, char** board);
//shooting function
void shootShip(int y, int x, char** board);
void getIDofShipWhichGotHitExtended(int y, int x, char** board);
//winning check function
bool checkIfSomeoneWins();
void checkWhichPlayerWins();

//these supply more detailed information about how function has to work
bool supplementToReefCheckerMovingForward(int i, int C, char** board);
bool supplementToReefCheckerMovingLeft(int i, int C, char** board);
bool supplementToReefCheckerMovingRight(int i, int C, char** board);

//it selects in which direction ship is going to
void movingShipSelector(int i, int C, char direction, char** board);
void movingShipSelectorPlayer(int i, int C, char direction, char** board);
//Error messages when some conditions are not fulfilled
void moveErrorMessages(int code, int i, int C, char D);

void movingShipsForwardPlayer(int i, int C, char** board);
void movingShipsForward(int i, int C, char** board);

void movingShipsLeftPlayer(int i, int C, char** board);
void movingShipsLeft(int i, int C, char** board);

void movingShipsRightPlayer(int i, int C, char** board);
void movingShipsRight(int i, int C, char** board);

void movingShips(int i, int C, char D, char** board);
bool conditionsForMovingShips(int i, int C, char D, char** board);
bool conditionsForMovingShipsForward(int i, int C, char** board);
//these supply more detailed information about how function has to work
bool supplementToCheckingBeingCloseWhileMovingLeft(int i, int C, char** board);
bool supplementToCheckingBeingCloseWhileMovingRight(int i, int C, char** board);
bool conditionForCheckingBeingCloseForForward(int i, int C, char** board);

bool checkIfShipIsGoingApartFromTheBoardForward(int i, int C);
bool checkIfShipIsGoingApartFromTheBoardLeft(int i, int C);
bool checkIfShipIsGoingApartFromTheBoardRight(int i, int C);

bool conditionsForCheckingBeingClose(int y, int x, char D, int i, int C, char** board);
bool checkIfIsntTooCloseToEachOther(int y, int x, int y1, int x1, char** board);

//several conditions mainly for placing ships and moving
bool isInInitialPosition(int y, int x, char D, int C);
bool checkIfIDhasntAlreadyBeenTaken(int id, int C);
bool checkIfShipHasBrokenEngine(int i, int C, char** board);

bool checkIfTheresNotTooManyShipsByGivenClass(int id, int C);
bool checkIfAllShipsHaveBeenPlaced();

bool checkIfTheresAFreeSpace(int y, int x, char D, int C, char** board);
bool checkIfTheresReef(int y, int x, char D, int C, char** board);

//it's used while shooting. it finds information about ship which got shot
int determineShipsID(int y, int x, int whichSide);
int determineShipsType(int y, int x, int id, int whichSide);
int determineShipsPlayer(int y, int x, int id, int C, int whichSide);

void state(char** board);

void functionalities(char** board);

void playerX(char** board);

int main() {
    char** board = creatingTheBoard();
    conditionsForBOARDSIZE();
    for (int y = 0; y < BOARDSIZEY; y++) {
        for (int x = 0; x < BOARDSIZEX; x++) {
            board[y][x] = ' ';
        }
    }
    functionalities(board);
    return 0;
}

//this funciton sets default values unless INIT_POSITION is used
void conditionsForBOARDSIZE() {
    player[0].playersBoard_t = {
        0, //startY
        0, //startX
        BOARDSIZEY / 2 - 1, //endY
        BOARDSIZEX - 1 //endX
    };

    player[1].playersBoard_t = {
        BOARDSIZEY / 2 + 1, //startY
        0, //startX
        BOARDSIZEY - 1, //endY
        BOARDSIZEX - 1 //endX
    };
}
int whichPlayer() {
    if (player[0].isPlayersMove == true) return 0;
    else if (player[1].isPlayersMove == true) return 1;
    else exit(0);
}
//this helps especially error messsages to display ship's name
const char* shipsName(int C) {
    if (C == 0) return "CAR";
    else if (C == 1) return "BAT";
    else if (C == 2) return "CRU";
    else if (C == 3) return "DES";
    else exit(0);
}
void setFleet(char P, char** board) {
    if (P == 'A') {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        player[0].shipInfo_t = { a, b, c, d };
        player[0].isSetFleetUsed = true;
        state(board);
    }
    else if (P == 'B') {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        player[1].shipInfo_t = { a, b, c, d };
        player[1].isSetFleetUsed = true;
        state(board);
    }
    else printf("INVALID OPERATION: THE OTHER PLAYER EXPECTED"); exit(0);
}

char** creatingTheBoard() {
    char** board = new char* [BOARDSIZEY];
    for (int i = 0; i < BOARDSIZEY; i++) { board[i] = new char[BOARDSIZEX]; }
    return board;
}

//it tells how many numbers are in a row. it is used for printing purposes
int decimalSystemInTheTable(int size) {
    int value = size;
    int liczbaCyfr = 0;
    while (value > 0) {
        liczbaCyfr++;
        value = value / 10;
    }
    return liczbaCyfr;
}

//printing function with visible columns/rows
void printTheBoard(char** board) {
    int xd = decimalSystemInTheTable(BOARDSIZEX);
    for (int r = 0; r < xd; r++) { cout << " "; }
    for (int e = 0; e < BOARDSIZEX; e++) { cout << e; }
    cout << endl;
    for (int w = 0; w < BOARDSIZEY; w++) {
        cout << w / 10;
        cout << w - w / 10 * 10;
        for (int j = 0; j < BOARDSIZEX; j++) {
            cout << board[w][j];
        }
        cout << endl;
    }
    printf("PARTS REMAINING:: A : %d B : %d\n", player[0].shipInfo_t.shipParts, player[1].shipInfo_t.shipParts);
    state(board);
}
//printing function/it replaces ship parts to pluses
void printTheBasicBoard(char** board) {
    for (int i = 0; i < BOARDSIZEY; i++) {
        for (int j = 0; j < BOARDSIZEX; j++) {
            if (board[i][j] == '@' || board[i][j] == '!' || board[i][j] == '%') { cout << '+'; }
            else cout << board[i][j];
        }
        cout << endl;
    }
    printf("PARTS REMAINING:: A : %d B : %d\n", player[0].shipInfo_t.shipParts, player[1].shipInfo_t.shipParts);
    state(board);
}
//placing individual characters
void placingSegmentsSelector(int y, int x, int C, char D, char** board) {
    int i = 0, j = 0, h = 0, k = 0;
    int* hptr;
    int* kptr;
    hptr = &h;
    kptr = &k;

    if (D == 'N') { h = 1; }
    else if (D == 'S') { h = -1; }
    else if (D == 'W') { k = 1; }
    else if (D == 'E') { k = -1; }

    if (C >= 0 && C <= 3) {
        board[y + i][x + j] = '@';
        i += *hptr; j += *kptr;

        if (C <= 2) {
            board[y + i][x + j] = '!';
            i += *hptr; j += *kptr;
        }

        if (C <= 1) {
            board[y + i][x + j] = '+';
            i += *hptr; j += *kptr;
        }

        if (C <= 0) {
            board[y + i][x + j] = '+';
            i += *hptr; j += *kptr;
        }

        board[y + i][x + j] = '%';
    }
}
//placing ship function
void placingShip(int y, int x, char D, int id, int C, char** board) {
    int playersNum = whichPlayer();
    if (player[playersNum].isPlayersMove == true) {
        player[playersNum].ship_t[C].x1[id] = x;
        player[playersNum].ship_t[C].y1[id] = y;
        player[playersNum].ship_t[C].destination[id] = D;
        player[playersNum].ship_t[C].damage[id] = 111111; // every type of ship gets this number and thanks to appropriate function adequate '1's would be hidden.
        player[playersNum].shipInfo_t.shipParts += (CARSIZE - C);
    }
    bool allShips = checkIfAllShipsHaveBeenPlaced();
    if (allShips == true) {
        player[0].placedAllShips = true;
        player[1].placedAllShips = true;
    }
    placingSegmentsSelector(y, x, C, D, board);
    if (D == 'N') {
        player[playersNum].ship_t[C].y2[id] = y - 1 + CARSIZE - C;
        player[playersNum].ship_t[C].x2[id] = x;
    }
    else if (D == 'W') {
        player[playersNum].ship_t[C].y2[id] = y;
        player[playersNum].ship_t[C].x2[id] = x - 1 + CARSIZE - C;
    }
    else if (D == 'S') {
        player[playersNum].ship_t[C].y2[id] = y + 1 - CARSIZE + C;
        player[playersNum].ship_t[C].x2[id] = x;
    }
    else if (D == 'E') {
        player[playersNum].ship_t[C].y2[id] = y;
        player[playersNum].ship_t[C].x2[id] = x + 1 - CARSIZE + C;
    }
}
//placing ship conditions
void placeShip(int y, int x, char D, int id, int C, char** board) {
    bool initial = isInInitialPosition(y, x, D, C);
    bool IDisntTaken = checkIfIDhasntAlreadyBeenTaken(id, C);
    bool notTooMany = checkIfTheresNotTooManyShipsByGivenClass(id, C);
    bool theresReef = checkIfTheresReef(y, x, D, C, board);
    bool notTooCloseToOthers = false;
    for (int r = 0; r < CARSIZE - C; r++) {
        if (D == 'N') if (checkIfIsntTooCloseToEachOther(y + r, x, y, x, board) == false) break;
        if (D == 'E') if (checkIfIsntTooCloseToEachOther(y, x + r, y, x, board) == false) break;
        if (D == 'S') if (checkIfIsntTooCloseToEachOther(y - r, x, y, x, board) == false) break;
        if (D == 'W') if (checkIfIsntTooCloseToEachOther(y, x - r, y, x, board) == false) break;
        notTooCloseToOthers = true;
    }
    if (notTooCloseToOthers == true && initial == true && IDisntTaken == true && notTooMany == true && theresReef == false) { placingShip(y, x, D, id, C, board); }
    else if (initial == false) { printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": NOT IN STARTING POSITION", y, x, D, id, shipsName(C)); exit(0); }
    else if (IDisntTaken == false) { printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": SHIP ALREADY PRESENT", y, x, D, id, shipsName(C)); exit(0); }
    else if (notTooMany == false) { printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": ALL SHIPS OF THE CLASS ALREADY SET", y, x, D, id, shipsName(C)); exit(0); }
    else if (theresReef == true) { printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": PLACING SHIP ON REEF", y, x, D, id, shipsName(C)); exit(0); }
    else if (notTooCloseToOthers == false) { printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": PLACING SHIP TOO CLOSE TO OTHER SHIP", y, x, D, id, shipsName(C)); exit(0); }
}

void printDMG(int C, int dmg) {
    int r = 0;
    while (r < CARSIZE - C) {
        printDMGdigits(dmg, r);
        r++;
    }
    cout << "\n";
}
void printDMGdigits(int dmg, int r) {
    int i = 0;
    for (i = CARSIZE - r - 1; i > 0; i--) { dmg = dmg / 10; }
    if (dmg % 10 == 0) cout << 0;
    else cout << 1;
}

void shipFunction(char P, int y, int x, char D, int id, int C, int dmg, char** board) {
    if (P == 'A') player[0].isPlayersMove = true;
    else if (P == 'B') player[1].isPlayersMove = true;
    bool IDisntTaken = checkIfIDhasntAlreadyBeenTaken(id, C);
    bool notTooMany = checkIfTheresNotTooManyShipsByGivenClass(id, C);
    bool theresReef = checkIfTheresReef(y, x, D, C, board);
    bool notTooCloseToOthers = false;
    for (int r = 0; r < CARSIZE - C; r++) {
        if (D == 'N') if (checkIfIsntTooCloseToEachOther(y + r, x, y, x, board) == false) break;
        if (D == 'E') if (checkIfIsntTooCloseToEachOther(y, x + r, y, x, board) == false) break;
        if (D == 'S') if (checkIfIsntTooCloseToEachOther(y - r, x, y, x, board) == false) break;
        if (D == 'W') if (checkIfIsntTooCloseToEachOther(y, x - r, y, x, board) == false) break;
        notTooCloseToOthers = true;
    }
    if (notTooCloseToOthers == true && IDisntTaken == true && theresReef == false && notTooMany == true) {
        placingShip(y, x, D, id, C, board);
        player[0].isPlayersMove = false;
        player[1].isPlayersMove = false;
        makeDmgInTheShipFunction(P, y, x, D, id, C, dmg, board);
    }
    else if (IDisntTaken == false) {
        printf("INVALID OPERATION \"SHIP %c %d %d %c %d %s %d\": SHIP ALREADY PRESENT", P, y, x, D, id, shipsName(C), dmg); exit(0);
    }
    else if (notTooMany == false) {
        printf("INVALID OPERATION \"SHIP %c %d %d %c %d %s %d\": ALL SHIPS OF THE CLASS ALREADY SET", P, y, x, D, id, shipsName(C), dmg); exit(0);
    }
    else if (theresReef == true) {
        printf("INVALID OPERATION \"SHIP %c %d %d %c %d %s %d\": PLACING SHIP ON REEF", P, y, x, D, id, shipsName(C), dmg); exit(0);
    }
    else if (notTooCloseToOthers == false) {
        printf("INVALID OPERATION \"SHIP %c %d %d %c %d %s %d\": PLACING SHIP TOO CLOSE TO OTHER SHIP", P, y, x, D, id, shipsName(C), dmg); exit(0);
    }
}
//damage 
void makeDmgInTheShipFunction(char P, int y, int x, char D, int id, int C, int dmg, char** board) {
    int playersNum = 0;
    if (P == 'A') { playersNum = 0; }
    else if (P == 'B') { playersNum = 1; }
    int r = 0;
    //these four conditions fullfil damage number to 6 digit number (one hidden for players and rest visible)
    if (dmg < 100) { dmg = dmg * 1000 + 100000; }
    else if (dmg < 1000) { dmg = dmg * 100 + 100000; }
    else if (dmg < 10000) { dmg = dmg * 10 + 100000; }
    else dmg = dmg + 100000;
    player[playersNum].ship_t[C].damage[id] = dmg;
    while (r < CARSIZE - C) {
        if (isItZero(dmg, r) == true) { makeXinTheShipFunction(y, x, D, r, board); player[playersNum].shipInfo_t.shipParts--; }
        r++;
    }
}
bool isItZero(int dmg, int r) {
    int i = 0;
    for (i = CARSIZE - r - 1; i > 0; i--) { dmg = dmg / 10; }
    if (dmg % 10 == 0)  return true;
    else return false;
}
void makeXinTheShipFunction(int y, int x, char D, int whereXshouldBe, char** board) {
    if (D == 'N') { board[y + whereXshouldBe][x] = 'x'; }
    else if (D == 'E') { board[y][x - whereXshouldBe] = 'x'; }
    else if (D == 'S') { board[y - whereXshouldBe][x] = 'x'; }
    else if (D == 'W') { board[y][x + whereXshouldBe] = 'x'; }
    player[0].isPlayersMove = false;
    player[1].isPlayersMove = false;
}

void placeReef(int y, int x, char** board) {
    if (player[0].isPlayerXUsed == false && player[1].isPlayerXUsed == false) {
        if (x >= 0 && x < BOARDSIZEX && y >= 0 && y < BOARDSIZEY) {
            int idReef;
            idReef = IDforReefs();
            reef_t[idReef].y = y;
            reef_t[idReef].x = x;
            reef_t[idReef].used = true;
            board[y][x] = '#';
        }
        else { printf("INVALID OPERATION \"REEF %d %d\": REEF IS NOT PLACED ON BOARD", y, x); exit(0); }
    }
    else exit(0);
}

//shooting functions

//calculates and saves info about ship's damage
void calculateWhereShipGotShot(int y, int x, int e, int r, int w) {
    char D = player[w].ship_t[r].destination[e];
    int distanceBetweenShipsHeadAndShot = 5;
    if (D == 'W') { distanceBetweenShipsHeadAndShot = x - player[w].ship_t[r].x1[e]; }
    else if (D == 'E') { distanceBetweenShipsHeadAndShot = player[w].ship_t[r].x1[e] - x; }
    else if (D == 'S') { distanceBetweenShipsHeadAndShot = player[w].ship_t[r].y1[e] - y; }
    else if (D == 'N') { distanceBetweenShipsHeadAndShot = y - player[w].ship_t[r].y1[e]; }

    if (distanceBetweenShipsHeadAndShot == 0) { player[w].ship_t[r].damage[e] -= 10000; }
    else if (distanceBetweenShipsHeadAndShot == 1) { player[w].ship_t[r].damage[e] -= 1000; }
    else if (distanceBetweenShipsHeadAndShot == 2) { player[w].ship_t[r].damage[e] -= 100; }
    else if (distanceBetweenShipsHeadAndShot == 3) { player[w].ship_t[r].damage[e] -= 10; }
    else if (distanceBetweenShipsHeadAndShot == 4) { player[w].ship_t[r].damage[e] -= 1; }

}
void shootShip(int y, int x, char** board) {
    int* playerptr;
    int whichPlayer;
    playerptr = &whichPlayer;
    if (player[0].isPlayersMove == true) whichPlayer = 0;
    else if (player[1].isPlayersMove == true) whichPlayer = 1;
    if (player[0].placedAllShips == true && player[1].placedAllShips == true) {
        if (y >= 0 && y < BOARDSIZEY && y != BOARDSIZEY / 2 && x >= 0 && x < BOARDSIZEX) {
            if (board[y][x] == '+' || board[y][x] == '@' || board[y][x] == '%' || board[y][x] == '!') {
                getIDofShipWhichGotHitExtended(y, x, board);
                int id = player[*playerptr].temporary_info_t.id;
                int type = player[*playerptr].temporary_info_t.type;
                int playerInt = player[*playerptr].temporary_info_t.player;
                calculateWhereShipGotShot(y, x, id, type, playerInt);
                board[y][x] = 'x';
                (player[playerInt].shipInfo_t.shipParts)--;
                if (checkIfSomeoneWins() == true) { checkWhichPlayerWins(); }
            }
            else;
        }
        else {
            printf("INVALID OPERATION \"SHOOT %d %d\": FIELD DOES NOT EXIST", y, x);
            exit(0);
        }
    }
    else {
        printf("INVALID OPERATION \"SHOOT %d %d\": NOT ALL SHIPS PLACED", y, x);
        exit(0);
    }
}

void resetCounter() {
    if (player[0].isPlayersMove == true) {
        for (int e = 0; e < 4; e++) {
            for (int r = 0; r < MAXTOTALSHIPSNUMBER; r++) { player[0].ship_t[e].moveCounter[r] = 0; }
        }
    }
    else if (player[1].isPlayersMove == true) {
        for (int e = 0; e < 4; e++) { for (int r = 0; r < MAXTOTALSHIPSNUMBER; r++) { player[1].ship_t[e].moveCounter[r] = 0; } }
    }
    else exit(0);
}
void saveState() {
    int dmg = 0;
    char playerName = 'A';
    int reefs = 0;
    printf("[state]\n");
    printf("BOARD_SIZE %d %d\n", BOARDSIZEY, BOARDSIZEX);
    printf("NEXT_PLAYER ");
    if (player[1].isPlayersMove == true) cout << "A\n";
    else cout << "B\n";
    for (int a = 0; a < PLAYERSNUMBER; a++) {
        if (a == 0) { playerName = 'A'; }
        else if (a == 1) { playerName = 'B'; }
        printf("INIT_POSITION %c %d %d %d %d\n", playerName, player[a].playersBoard_t.startY, player[a].playersBoard_t.startX, player[a].playersBoard_t.endY, player[a].playersBoard_t.endX);
        printf("SET_FLEET %c %d %d %d %d\n", playerName, player[a].shipInfo_t.CARamount, player[a].shipInfo_t.BATamount, player[a].shipInfo_t.CRUamount, player[a].shipInfo_t.DESamount);
        for (int r = 0; r < player[a].shipInfo_t.CARamount; r++) {
            printf("SHIP %c %d %d %c %d CAR ", playerName, player[a].ship_t[0].y1[r], player[a].ship_t[0].x1[r], player[a].ship_t[0].destination[r], r);
            dmg = (player[a].ship_t[0].damage[r] - 100000);
            printDMG(0, dmg);
        }
        for (int r = 0; r < player[a].shipInfo_t.BATamount; r++) {
            printf("SHIP %c %d %d %c %d BAT ", playerName, player[a].ship_t[1].y1[r], player[a].ship_t[1].x1[r], player[a].ship_t[1].destination[r], r);
            dmg = (player[a].ship_t[1].damage[r] - 100000);
            printDMG(1, dmg);
        }
        for (int r = 0; r < player[a].shipInfo_t.CRUamount; r++) {
            printf("SHIP %c %d %d %c %d CRU ", playerName, player[a].ship_t[2].y1[r], player[a].ship_t[2].x1[r], player[a].ship_t[2].destination[r], r);
            dmg = (player[a].ship_t[2].damage[r] - 100000);
            printDMG(2, dmg);
        }
        for (int r = 0; r < player[a].shipInfo_t.DESamount; r++) {
            printf("SHIP %c %d %d %c %d DES ", playerName, player[a].ship_t[3].y1[r], player[a].ship_t[3].x1[r], player[a].ship_t[3].destination[r], r);
            dmg = (player[a].ship_t[3].damage[r] - 100000);
            printDMG(3, dmg);
        }
    }
    reefs = IDforReefs();
    for (int w = 0; w < reefs; w++) {
        printf("REEF %d %d\n", reef_t[w].y, reef_t[w].x);
    }
    cout << "[state]\n";
}

bool checkIfSomeoneWins() {
    if ((player[0].shipInfo_t.shipParts == 0 || player[1].shipInfo_t.shipParts == 0)
        && (player[0].placedAllShips == true && player[1].placedAllShips))
        return true; else return false;
}
void checkWhichPlayerWins() {
    if (player[0].shipInfo_t.shipParts == 0) { printf("B won"); exit(0); }
    else if (player[1].shipInfo_t.shipParts == 0) { printf("A won"); exit(0); }
}



//checks whether it's possible to put a ship in an area of ship's size
bool supplementToCheckFreeSpaceForMovingForward(int y, int x, char D, char** board) {
    int C = 4; //we need to check only one square in front of the ship so 1=5-4;
    if (checkIfTheresAFreeSpace(y, x, D, C, board) == true) return true; else return false;
}
bool checkIfTheresAFreeSpace(int y, int x, char D, int C, char** board) {
    int temp = 0;
    int* tempptr;
    tempptr = &temp;
    for (int j = 0; j < CARSIZE - C; j++) {
        if (D == 'N') { if (board[y + j][x] == ' ') temp++; }
        else if (D == 'W') { if (board[y][x + j] == ' ') temp++; }
        else if (D == 'S') { if (board[y - j][x] == ' ') temp++; }
        else if (D == 'E') { if (board[y][x - j] == ' ') temp++; }
    }
    if (*tempptr == CARSIZE - C) return true; else return false;
}
bool supplementToReefCheckerMovingForward(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            if (checkIfTheresReef(y--, x, D, C, board) == true) return true; else return false;
        }
        else if (D == 'E') {
            if (checkIfTheresReef(y, x++, D, C, board) == true) return true; else return false;
        }
        else if (D == 'S') {
            if (checkIfTheresReef(y++, x, D, C, board) == true) return true; else return false;
        }
        else if (D == 'W') {
            if (checkIfTheresReef(y, x--, D, C, board) == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}
bool supplementToReefCheckerMovingLeft(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            D = 'W';
            if (checkIfTheresReef(y - 1, x - CARSIZE + C, D, C, board) == true) return true; else return false;
        }
        else if (D == 'E') {
            D = 'N';
            if (checkIfTheresReef(y - CARSIZE + C, x + 1, D, C, board) == true) return true; else return false;
        }
        else if (D == 'S') {
            D = 'E';
            if (checkIfTheresReef(y + 1, x + CARSIZE - C, D, C, board) == true) return true; else return false;
        }
        else if (D == 'W') {
            D = 'S';
            if (checkIfTheresReef(y + CARSIZE - C, x - 1, D, C, board) == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}
bool supplementToReefCheckerMovingRight(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            D = 'E';
            if (checkIfTheresReef(y - 1, x + CARSIZE - C, D, C, board) == true) return true; else return false;
        }
        else if (D == 'E') {
            D = 'S';
            if (checkIfTheresReef(y + CARSIZE - C, x + 1, D, C, board) == true) return true; else return false;
        }
        else if (D == 'S') {
            D = 'W';
            if (checkIfTheresReef(y + 1, x - CARSIZE + C, D, C, board) == true) return true; else return false;
        }
        else if (D == 'W') {
            D = 'N';
            if (checkIfTheresReef(y - CARSIZE + C, x - 1, D, C, board) == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}
bool checkIfTheresReef(int y, int x, char D, int C, char** board) {
    for (int j = 0; j < CARSIZE - C; j++) {
        if (D == 'N') { if (board[y + j][x] == '#') return true; }
        else if (D == 'W') { if (board[y][x + j] == '#') return true; }
        else if (D == 'S') { if (board[y - j][x] == '#') return true; }
        else if (D == 'E') { if (board[y][x - j] == '#') return true; }
    }
    return false;
}
bool checkIfShipHasBrokenEngine(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (D == 'N') { if (board[y + CARSIZE - C - 1][x] == 'x') return true; else return false; }
    else if (D == 'S') { if (board[y - CARSIZE + C + 1][x] == 'x') return true; else return false; }
    else if (D == 'E') { if (board[y][x - CARSIZE + C + 1] == 'x') return true; else return false; }
    else if (D == 'W') { if (board[y][x + CARSIZE - C - 1] == 'x') return true; else return false; }
    else return false;
}

bool checkIfShipIsGoingApartFromTheBoardForward(int i, int C) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') { if (y - 1 < 0) return true; else return false; }
        else if (D == 'E') { if (x + 1 > BOARDSIZEX) return true; else return false; }
        else if (D == 'S') { if (y + 1 > BOARDSIZEY)  return true; else return false; }
        else if (D == 'W') {
            if (x - 1 < 0) return true;
            else return false;
        }
        else return true;
    }
    else return true;
}
bool checkIfShipIsGoingApartFromTheBoardLeft(int i, int C) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (checkIfShipIsGoingApartFromTheBoardForward(i, C) == false) {
        if (player[playersNum].isPlayersMove == true) {
            if (D == 'N') { if (x - CARSIZE + C + 1 < 0) return true; else return false; }
            else if (D == 'E') { if (y - CARSIZE + C + 1 > BOARDSIZEY) return true; else return false; }
            else if (D == 'S') { if (x + CARSIZE - C - 1 > BOARDSIZEX) return true; else return false; }
            else if (D == 'W') { if (y + CARSIZE - C - 1 < 0) return true; else return false; }
            else return true;
        }
        else return false;
    }
    else return false;
}
bool checkIfShipIsGoingApartFromTheBoardRight(int i, int C) {
    if (checkIfShipIsGoingApartFromTheBoardForward(i, C) == false) {
        int playersNum = whichPlayer();
        int x = player[playersNum].ship_t[C].x1[i];
        int y = player[playersNum].ship_t[C].y1[i];
        char D = player[playersNum].ship_t[C].destination[i];

        if (player[playersNum].isPlayersMove == true) {
            if (D == 'N') { if (x + CARSIZE - C - 1 < 0) return true; else return false; }
            else if (D == 'E') { if (y - CARSIZE + C + 1 > BOARDSIZEY) return true; else return false; }
            else if (D == 'S') { if (x - CARSIZE + C + 1 > BOARDSIZEX) return true; else return false; }
            else if (D == 'W') { if (y + CARSIZE - C - 1 < 0) return true; else return false; }
            else return true;
        }
        else return false;
    }
    else return false;
}

//several conditions
bool isInInitialPosition(int y, int x, char D, int C) {
    int playersNum = whichPlayer();
    for (int j = 0; j < (CARSIZE - C); j++) {
        if (player[playersNum].isPlayersMove == true) {

            if (D == 'N') {
                if (y >= player[playersNum].playersBoard_t.startY && y + j <= player[playersNum].playersBoard_t.endY && x >= player[playersNum].playersBoard_t.startX && x <= player[playersNum].playersBoard_t.endX) {
                    if (j == CARSIZE - C - 1) return true;
                }
            }
            else if (D == 'W') {
                if (y >= player[playersNum].playersBoard_t.startY && y <= player[playersNum].playersBoard_t.endY && x >= player[playersNum].playersBoard_t.startX && x + j <= player[playersNum].playersBoard_t.endX) {
                    if (j == CARSIZE - C - 1) return true;
                }
            }
            else if (D == 'S') {
                if (y - j >= player[playersNum].playersBoard_t.startY && y <= player[playersNum].playersBoard_t.endY && x >= player[playersNum].playersBoard_t.startX && x <= player[playersNum].playersBoard_t.endX) {
                    if (j == CARSIZE - C - 1) return true;
                }
            }
            else if (D == 'E') {
                if (y >= player[playersNum].playersBoard_t.startY && y <= player[playersNum].playersBoard_t.endY && x - j >= player[playersNum].playersBoard_t.startX && x <= player[playersNum].playersBoard_t.endX) {
                    if (j == CARSIZE - C - 1) return true;
                }
            }
        }
    }
    return false;
}
bool checkIfTheresNotTooManyShipsByGivenClass(int id, int C) {
    int playersNum = whichPlayer();
    if (player[playersNum].isPlayersMove == true) {
        if (C == 0) { if (id <= player[playersNum].shipInfo_t.CARamount - 1) return true; else return false; }
        else if (C == 1) { if (id <= player[playersNum].shipInfo_t.BATamount - 1) return true; else return false; }
        else if (C == 2) { if (id <= player[playersNum].shipInfo_t.CRUamount - 1) return true; else return false; }
        else if (C == 3) { if (id <= player[playersNum].shipInfo_t.DESamount - 1) return true; else return false; }
        else return false;
    }
    else return false;
}
bool checkIfIDhasntAlreadyBeenTaken(int id, int C) {
    int playersNum = whichPlayer();
    if (player[playersNum].isPlayersMove == true) {
        if (player[playersNum].ship_t[C].destination[id] == 'N'
            || player[playersNum].ship_t[C].destination[id] == 'S'
            || player[playersNum].ship_t[C].destination[id] == 'E'
            || player[playersNum].ship_t[C].destination[id] == 'W')
            return false; else return true;
    }
    else return false;
}
int IDforReefs() {
    for (int i = 0; i < BOARDSIZEY * BOARDSIZEX; i++) {
        if (reef_t[i].used == false) {
            return i;
        }
    }
    return 0;
}
bool checkIfAllShipsHaveBeenPlaced() {
    if (player[0].shipInfo_t.numberOfShipsParts == player[0].shipInfo_t.shipParts && player[1].shipInfo_t.numberOfShipsParts == player[1].shipInfo_t.shipParts) return true;
    else return false;
}

//this gives information about ID of a ship which gets shot
void getIDofShipWhichGotHitExtended(int y, int x, char** board) {
    int playersNum = whichPlayer();
    int ID, type, playerInt;
    int YY = 0, XX = 0, side = 1;
    if (board[y][x] == '@') {
    }
    else if (board[y][x] == '!') {
        if (board[y - 1][x] == '@') YY = -1;
        else if (board[y + 1][x] == '@') YY = 1;
        else if (board[y][x - 1] == '@') XX = -1;
        else if (board[y][x + 1] == '@') XX = 1;
        else if (board[y - 1][x] == 'x') {
            if (y - 2 >= 0 && (board[y - 2][x] == '+' || board[y - 2][x] == '%' || board[y - 2][x] == 'x')) YY = 1;
            else { if (determineShipsID(y - 1, x, 1) == -1) { YY = -1; side = 2; } else YY = -1; }
        }
        else if (board[y + 1][x] == 'x') {
            if (y + 2 <= BOARDSIZEY && (board[y + 2][x] == '+' || board[y + 2][x] == '%' || board[y + 2][x] == 'x')) YY = -1;
            else { if (determineShipsID(y + 1, x, 1) == -1) { YY = 1; side = 2; } else YY = 1; }
        }
        else if (board[y][x - 1] == 'x') {
            if (x - 2 >= 0 && (board[y][x - 2] == '+' || board[y][x - 2] == '%' || board[y][x - 2] == 'x'))  XX = 1;
            else { if (determineShipsID(y, x - 1, 1) == -1) { XX = -1; side = 2; } else XX = -1; }
        }
        else if (board[y][x + 1] == 'x') {
            if (x + 2 <= BOARDSIZEX && (board[y][x + 2] == '+' || board[y][x + 2] == '%' || board[y][x + 2] == 'x')) XX = -1;
            else { if (determineShipsID(y, x + 1, 1) == -1) { XX = 1; side = 2; } else XX = 1; }
        }
    }
    else if (board[y][x] == '+') {
        if (board[y - 1][x] == '!') YY = -2;
        else if (board[y + 1][x] == '!') YY = 2;
        else if (board[y][x - 1] == '!') XX = -2;
        else if (board[y][x + 1] == '!') XX = 2;
        else if (board[y - 1][x] == '%') { YY = -1; side = 2; }
        else if (board[y + 1][x] == '%') { YY = 1; side = 2; }
        else if (board[y][x - 1] == '%') { XX = -1; side = 2; }
        else if (board[y][x + 1] == '%') { XX = 1; side = 2; }
        else if (board[y - 1][x] == 'x') {
            if (board[y - 2][x] == 'x' || board[y - 2][x] == '@') {
                if (determineShipsID(y - 2, x, 1) == -1) { YY = -2; side = 2; }
                else YY = -2;
            }
            else { if (determineShipsID(y - 1, x, 1) == -1) { YY = -1; side = 2; } else YY = -1; }
        }
        else if (board[y + 1][x] == 'x') {
            if (board[y + 2][x] == 'x' || board[y - 2][x] == '@') {
                if (determineShipsID(y + 2, x, 1) == -1) { YY = 2; side = 2; }
                else YY = 2;
            }
            else { if (determineShipsID(y + 1, x, 1) == -1) { YY = 1; side = 2; } else YY = 1; }
        }
        else if (board[y][x - 1] == 'x') {
            if (board[y][x - 2] == 'x' || board[y - 2][x] == '@') {
                if (determineShipsID(y, x - 2, 1) == -1) { XX = -2; side = 2; }
                else XX = -2;
            }
            else { if (determineShipsID(y, x - 1, 1) == -1) { XX = -1; side = 2; } else XX = -1; }
        }
        else if (board[y][x + 1] == 'x') {
            if (board[y][x + 2] == 'x' || board[y - 2][x] == '@') {
                if (determineShipsID(y, x + 2, 1) == -1) { XX = 2; side = 2; }
                else XX = 2;
            }
            else { if (determineShipsID(y, x + 1, 1) == -1) { XX = 1; side = 2; } else XX = 1; }
        }
    }
    else if (board[y][x] == '%') side = 2;

    ID = determineShipsID(y + YY, x + XX, side);
    type = determineShipsType(y + YY, x + XX, ID, side);
    playerInt = determineShipsPlayer(y + YY, x + XX, ID, type, side);
    player[playersNum].temporary_info_t.id = ID;
    player[playersNum].temporary_info_t.type = type;
    player[playersNum].temporary_info_t.player = playerInt;
}

// int whichSide in this function is responsible for checking ID of the ship from the given by this int side. int takes 1 or 2.
int determineShipsID(int y, int x, int whichSide) {
    for (int r = 0; r < 4; r++) {
        for (int e = 0; e < 10; e++) {
            for (int w = 0; w < 2; w++) {
                if (whichSide == 1) {
                    if (player[w].ship_t[r].x1[e] == x && player[w].ship_t[r].y1[e] == y) return e;
                }
                else if (whichSide == 2) {
                    if (player[w].ship_t[r].x2[e] == x && player[w].ship_t[r].y2[e] == y) return e;
                }
            }
        }
    }
    return -1;
}
int determineShipsType(int y, int x, int id, int whichSide) {
    for (int r = 0; r < 4; r++) {
        for (int w = 0; w < 2; w++) {
            if (whichSide == 1) {
                if (player[w].ship_t[r].x1[id] == x && player[w].ship_t[r].y1[id] == y) return r;
            }
            else if (whichSide == 2) {
                if (player[w].ship_t[r].x2[id] == x && player[w].ship_t[r].y2[id] == y) return r;
            }
        }
    }
    return -1;
}
int determineShipsPlayer(int y, int x, int id, int C, int whichSide) {
    for (int w = 0; w < 2; w++) {
        if (whichSide == 1) {
            if (player[w].ship_t[C].x1[id] == x && player[w].ship_t[C].y1[id] == y)  return w;
        }
        else if (whichSide == 2) {
            if (player[w].ship_t[C].x2[id] == x && player[w].ship_t[C].y2[id] == y) return w;
        }
    }
    return -1;
}

//Menus
void state(char** board) {
    char buf[BUFFORSIZE];
    while (1) {
        scanf("%19s", buf);
        if (feof(stdin) != 0) break;
        if (strcmp(buf, "[state]") == 0) functionalities(board);
        else if (strcmp(buf, "PRINT") == 0) {
            int print;
            cin >> print;
            if (print == 0) { printTheBasicBoard(board); }
            if (print == 1) { printTheBoard(board); }
        }
        else if (strcmp(buf, "SET_FLEET") == 0) {
            if (player[0].playerXwasntUsedYet = true && player[1].playerXwasntUsedYet == true) {
                char P;
                cin >> P;
                setFleet(P, board);
            }
            else exit(0);
        }
        else if (strcmp(buf, "NEXT_PLAYER") == 0) {
            if (player[0].playerXwasntUsedYet = true && player[1].playerXwasntUsedYet == true) {
                char P;
                cin >> P;
                if (P == 'A') {
                    player[0].isPlayerXUsed = false;
                    player[0].isPlayersMove = true;
                    player[1].isPlayersMove = false;
                    state(board);
                }
                else if (P == 'B') {
                    player[1].isPlayerXUsed = false;
                    player[1].isPlayersMove = true;
                    player[0].isPlayersMove = false;
                    state(board);
                }
                else printf("INVALID OPERATION: THE OTHER PLAYER EXPECTED"); exit(0);
            }
        }
        else if (strcmp(buf, "BOARD_SIZE") == 0) {
            ;
        }
        else if (strcmp(buf, "INIT_POSITION") == 0) {
            if (player[0].playerXwasntUsedYet = true && player[1].playerXwasntUsedYet == true) {
                int startX, startY, endX, endY;
                char P;
                cin >> P >> startY >> startX >> endY >> endX;
                if (P == 'A') {
                    player[0].playersBoard_t = { startY, startX, endY, endX };
                    player[0].isInitPositionUsed = true;
                }
                else if (P == 'B') {
                    player[1].playersBoard_t = { startY, startX, endY, endX };
                    player[1].isInitPositionUsed = true;
                }
            }
        }
        else if (strcmp(buf, "REEF") == 0) {
            int y, x;
            cin >> y >> x;
            placeReef(y, x, board);
        }
        else if (strcmp(buf, "SHIP") == 0) {
            char P, D, C[4];
            int y, x, id, dmg;
            cin >> P >> y >> x >> D >> id;
            scanf("%3s", C);
            cin >> dmg;
            if (strcmp(C, "CAR") == 0) {
                shipFunction(P, y, x, D, id, 0, dmg, board);
            }
            else if (strcmp(C, "BAT") == 0) {
                shipFunction(P, y, x, D, id, 1, dmg, board);
            }
            else if (strcmp(C, "CRU") == 0) {
                shipFunction(P, y, x, D, id, 2, dmg, board);
            }
            else if (strcmp(C, "DES") == 0) {
                shipFunction(P, y, x, D, id, 3, dmg, board);
            }
        }
        else if (strcmp(buf, "EXTENDED_SHIPS") == 0) {
            ;
        }
        else if (strcmp(buf, "SAVE") == 0) saveState();
        else printf("UNKNOWN COMMAND: >%s< \n", buf);
    }
}
void functionalities(char** board) {
    char buf[MAINBUFFORSIZE];
    while (1) {
        scanf("%19s", buf);
        if (feof(stdin) != 0)break;
        if (strcmp(buf, "[state]") == 0) state(board);
        else if (strcmp(buf, "[playerA]") == 0)
        {
            if (player[0].isPlayerXUsed == false) {
                player[0].isPlayersMove = true;
                player[1].isPlayersMove = false;
                if (player[0].playerXwasntUsedYet == true) {
                    player[0].playerXwasntUsedYet = false;
                    player[1].playerXwasntUsedYet = false;
                }
                playerX(board);
            }
            else { printf("INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED"); exit(0); }
        }
        else if (strcmp(buf, "[playerB]") == 0)
        {
            if (player[1].isPlayerXUsed == false) {
                player[0].isPlayersMove = false;
                player[1].isPlayersMove = true;
                if (player[1].playerXwasntUsedYet == true) {
                    player[0].playerXwasntUsedYet = false;
                    player[1].playerXwasntUsedYet = false;
                }
                playerX(board);
            }
            else { printf("INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED"); exit(0); }
        }
        else printf("UNKNOWN COMMAND: >%s< \n", buf);
    }
}
void playerX(char** board) {
    char buf[MAINBUFFORSIZE];
    while (1) {
        scanf("%19s", buf);
        if (feof(stdin) != 0)break;
        else if (strcmp(buf, "[playerA]") == 0)
        {
            if (player[0].isPlayerXUsed == false) {
                resetCounter();
                player[0].isPlayerXUsed = true;
                player[1].isPlayerXUsed = false;
                player[0].isPlayersMove = true;
                player[1].isPlayersMove = false;
                functionalities(board);
            }
        }
        else if (strcmp(buf, "[playerB]") == 0)
        {
            if (player[1].isPlayerXUsed == false) {
                resetCounter();
                player[0].isPlayerXUsed = false;
                player[1].isPlayerXUsed = true;
                player[0].isPlayersMove = false;
                player[1].isPlayersMove = true;
                functionalities(board);
            }
        }
        else if (strcmp(buf, "PLACE_SHIP") == 0)
        {
            int x, y, id;
            char D, C[4];
            cin >> x >> y >> D >> id;
            scanf("%3s", C);
            if (strcmp(C, "CAR") == 0) placeShip(x, y, D, id, 0, board);
            else if (strcmp(C, "BAT") == 0) placeShip(x, y, D, id, 1, board);
            else if (strcmp(C, "CRU") == 0) placeShip(x, y, D, id, 2, board);
            else if (strcmp(C, "DES") == 0) placeShip(x, y, D, id, 3, board);
        }
        else if (strcmp(buf, "SHOOT") == 0)
        {
            int y, x;
            cin >> y >> x;
            shootShip(y, x, board);
        }
        else if (strcmp(buf, "MOVE") == 0)
        {
            int i, typeC = 0;
            char C[4], direction;
            cin >> i;
            scanf("%3s", C);
            if (strcmp(C, "CAR") == 0) typeC = 0;
            else if (strcmp(C, "BAT") == 0) typeC = 1;
            else if (strcmp(C, "CRU") == 0) typeC = 2;
            else if (strcmp(C, "DES") == 0) typeC = 3;
            cin >> direction;
            movingShipSelector(i, typeC, direction, board);
        }
    }
}

//Move function:

//this function checks if one piece of the considered ship is near another ships
bool checkIfIsntTooCloseToEachOther(int y, int x, int y1, int x1, char** board) {
    int xStart, yStart, xEnd, yEnd;
    if (x == 0) xStart = 0; else xStart = -1;
    if (y == 0) yStart = 0; else yStart = -1;
    if (x == BOARDSIZEY) xEnd = 0; else xEnd = 1;
    if (y == BOARDSIZEX) yEnd = 0; else yEnd = 1;

    for (int i = yStart; i <= yEnd; i++)
    {
        for (int j = xStart; j <= xEnd; j++)
        {
            int xprim = x + j;
            int yprim = y + i;
            if ((xprim >= 0) && (xprim < BOARDSIZEX) && (yprim >= 0) && (yprim < BOARDSIZEY))
            {
                if (i == 0 && j == 0 && yprim == y && xprim == x) {}
                else {
                    if (board[yprim][xprim] == '@' || board[yprim][xprim] == '!' || board[yprim][xprim] == '%' || board[yprim][xprim] == '+' || board[yprim][xprim] == 'x')
                    {
                        if (yprim == y1 && xprim == x1) {}
                        else return false;
                    }
                }
            }
        }
    }
    return true;
}
bool conditionsForCheckingBeingClose(int y, int x, char D, int i, int C, char** board) {
    int playersNum = whichPlayer();
    int y1 = player[playersNum].ship_t[C].y1[i];
    int x1 = player[playersNum].ship_t[C].x1[i];

    int fulfills = 0;
    for (int r = 0; r < CARSIZE - C; r++) {
        if (D == 'N') {
            bool notClose = checkIfIsntTooCloseToEachOther(y + r, x, y1, x1, board);
            if (notClose == true) { fulfills++; if (fulfills == CARSIZE - C) return true; }
            else return false;
        }
        else if (D == 'W') {
            bool notClose = checkIfIsntTooCloseToEachOther(y, x + r, y1, x1, board);
            if (notClose == true) { fulfills++; if (fulfills == CARSIZE - C) return true; }
            else return false;
        }
        else if (D == 'S') {
            bool notClose = checkIfIsntTooCloseToEachOther(y - r, x, y1, x1, board);
            if (notClose == true) { fulfills++; if (fulfills == CARSIZE - C) return true; }
            else return false;
        }
        else if (D == 'E') {
            bool notClose = checkIfIsntTooCloseToEachOther(y, x - r, y1, x1, board);
            if (notClose == true) { fulfills++; if (fulfills == CARSIZE - C) return true; }
            else return false;
        }
        else return false;
    }
    return false;
}
// and this one links requirements for the entire ship
bool conditionForCheckingBeingCloseForForward(int i, int C, char** board) {
    int playersNum = whichPlayer();

    if (player[playersNum].isPlayersMove == true) {
        int y = player[playersNum].ship_t[C].y1[i];
        int x = player[playersNum].ship_t[C].x1[i];
        char D = player[playersNum].ship_t[C].destination[i];
        if (D == 'N') {
            bool notClose = checkIfIsntTooCloseToEachOther(y - 1, x, y, x, board);
            if (notClose == true) return true; else return false;
        }
        else if (D == 'E') {
            bool notClose = checkIfIsntTooCloseToEachOther(y, x + 1, y, x, board);
            if (notClose == true) return true; else return false;
        }
        else if (D == 'S') {
            bool notClose = checkIfIsntTooCloseToEachOther(y + 1, x, y, x, board);
            if (notClose == true) return true; else return false;
        }
        else if (D == 'W') {
            bool notClose = checkIfIsntTooCloseToEachOther(y, x - 1, y, x, board);
            if (notClose == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}
bool supplementToCheckingBeingCloseWhileMovingLeft(int i, int C, char** board) {
    int playersNum = whichPlayer();
    if (player[playersNum].isPlayersMove == true) {
        int y = player[playersNum].ship_t[C].y1[i];
        int x = player[playersNum].ship_t[C].x1[i];
        char D = player[playersNum].ship_t[C].destination[i];
        if (D == 'N') {
            D = 'W';
            if (conditionsForCheckingBeingClose(y - 1, x - CARSIZE + C, D, i, C, board) == true) return true; else return false;
        }
        else if (D == 'E') {
            D = 'N';
            if (conditionsForCheckingBeingClose(y - CARSIZE + C, x + 1, D, i, C, board) == true) return true; else return false;
        }
        else if (D == 'S') {
            D = 'E';
            if (conditionsForCheckingBeingClose(y + 1, x + CARSIZE - C, D, i, C, board) == true) return true; else return false;
        }
        else if (D == 'W') {
            D = 'S';
            if (conditionsForCheckingBeingClose(y + CARSIZE - C, x - 1, D, i, C, board) == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}
bool supplementToCheckingBeingCloseWhileMovingRight(int i, int C, char** board) {
    int playersNum = whichPlayer();
    if (player[playersNum].isPlayersMove == true) {
        int y = player[playersNum].ship_t[C].y1[i];
        int x = player[playersNum].ship_t[C].x1[i];
        char D = player[playersNum].ship_t[C].destination[i];
        if (D == 'N') {
            D = 'E';
            if (conditionsForCheckingBeingClose(y - 1, x + CARSIZE - C, D, i, C, board) == true)  return true; else return false;
        }
        else if (D == 'E') {
            D = 'S';
            if (conditionsForCheckingBeingClose(y + CARSIZE - C, x + 1, D, i, C, board) == true) return true; else return false;
        }
        else if (D == 'S') {
            D = 'W';
            if (conditionsForCheckingBeingClose(y + 1, x - CARSIZE + C, D, i, C, board) == true) return true; else return false;
        }
        else if (D == 'W') {
            D = 'N';
            if (conditionsForCheckingBeingClose(y - CARSIZE + C, x - 1, D, i, C, board) == true) return true; else return false;
        }
        else return false;
    }
    else return false;
}

// two next functions check if ship can make right move i.e. field is empty
bool conditionsForMovingShipsForward(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    bool can = false;
    if (D == 'N') { can = supplementToCheckFreeSpaceForMovingForward(y - 1, x, D, board); }
    else if (D == 'W') { can = supplementToCheckFreeSpaceForMovingForward(y, x - 1, D, board); }
    else if (D == 'S') { can = supplementToCheckFreeSpaceForMovingForward(y + 1, x, D, board); }
    else if (D == 'E') { can = supplementToCheckFreeSpaceForMovingForward(y, x + 1, D, board); }

    if (can == true) return true; else return false;
}
bool conditionsForMovingShips(int i, int C, char D, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char Dir = player[playersNum].ship_t[C].destination[i];
    int u = 0;

    bool can = false;
    if (Dir == 'N') {
        if (D == 'R') { Dir = 'E'; u = x + CARSIZE - C; }
        else if (D == 'L') { Dir = 'W'; u = x - CARSIZE + C; }
        can = checkIfTheresAFreeSpace(y - 1, u, Dir, C, board);
    }
    else if (Dir == 'W') {
        if (D == 'R') { Dir = 'N'; u = y - CARSIZE + C; }
        else if (D == 'L') { Dir = 'S'; u = y + CARSIZE - C; }
        can = checkIfTheresAFreeSpace(u, x - 1, Dir, C, board);
    }
    else if (Dir == 'S') {
        if (D == 'R') { Dir = 'W'; u = x - CARSIZE + C; }
        else if (D == 'L') { Dir = 'E'; u = x + CARSIZE - C; }
        can = checkIfTheresAFreeSpace(y + 1, u, Dir, C, board);
    }
    else if (Dir == 'E') {
        if (D == 'R') { Dir = 'S'; u = y + CARSIZE - C; }
        else if (D == 'L') { Dir = 'N'; u = y - CARSIZE + C; }
        can = checkIfTheresAFreeSpace(u, x + 1, Dir, C, board);
    }
    if (can == true) return true; else return false;
}
//it helps to redirect move function
void movingShipSelector(int i, int C, char direction, char** board) {
    if (direction == 'F') { movingShipsForward(i, C, board); }
    else if (direction == 'L') { movingShipsLeft(i, C, board); }
    else if (direction == 'R') { movingShipsRight(i, C, board); }
}
void movingShipSelectorPlayer(int i, int C, char direction, char** board) {
    if (direction == 'F') { movingShipsForwardPlayer(i, C, board); }
    else if (direction == 'L') { movingShipsLeftPlayer(i, C, board); }
    else if (direction == 'R') { movingShipsRightPlayer(i, C, board); }
}
//Error messages
void moveErrorMessages(int code, int i, int C, char D) {
    if (code == 0) { printf("INVALID OPERATION \"MOVE %d %s %c\": PLACING SHIP ON REEF", i, shipsName(C), D); }
    else if (code == 1) { printf("INVALID OPERATION \"MOVE %d %s %c\": PLACING SHIP TOO CLOSE TO OTHER SHIP", i, shipsName(C), D); }
    else if (code == 2) { printf("INVALID OPERATION \"MOVE %d %s %c\": SHIP WENT FROM BOARD", i, shipsName(C), D); }
    else if (code == 3) { printf("INVALID OPERATION \"MOVE %d %s %c\": SHIP CANNOT MOVE", i, shipsName(C), D); }
    else if (code == 4) { printf("INVALID OPERATION \"MOVE %d %s %c\": SHIP MOVED ALREADY", i, shipsName(C), D); }
    exit(0);
}
//set of functions and conditions required to move ship forward
void movingShipsForwardPlayer(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y - 1 + j][x] = board[y + j][x]; }
            board[y - 1 + CARSIZE - C][x] = ' ';
            player[playersNum].ship_t[C].y1[i]--;
            player[playersNum].ship_t[C].y2[i]--;
        }
        else if (D == 'W') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y][x - 1 + j] = board[y][x + j]; }
            board[y][x - 1 + CARSIZE - C] = ' ';
            player[playersNum].ship_t[C].x1[i]--;
            player[playersNum].ship_t[C].x2[i]--;
        }
        else if (D == 'S') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y + 1 - j][x] = board[y - j][x]; }
            board[y + 1 - CARSIZE + C][x] = ' ';
            player[playersNum].ship_t[C].y1[i]++;
            player[playersNum].ship_t[C].y2[i]++;
        }
        else if (D == 'E') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y][x + 1 - j] = board[y][x - j]; }
            board[y][x + 1 - CARSIZE + C] = ' ';
            player[playersNum].ship_t[C].x1[i]++;
            player[playersNum].ship_t[C].x2[i]++;
        }
    }
}
void movingShipsForward(int i, int C, char** board) {
    char D = 'F';
    int code = 5;
    bool theresReef = supplementToReefCheckerMovingForward(i, C, board);
    bool shipIsntNear = conditionForCheckingBeingCloseForForward(i, C, board);
    bool isShipGoingApart = checkIfShipIsGoingApartFromTheBoardForward(i, C);
    bool isEngineBroken = checkIfShipHasBrokenEngine(i, C, board);
    if (theresReef == false && isShipGoingApart == false && shipIsntNear == true && isEngineBroken == false) {

        if (player[0].isPlayersMove == true) {
            if (C == 0 && player[0].ship_t[0].moveCounter[i] < 2) {
                movingShipsForwardPlayer(i, C, board);
                player[0].ship_t[C].moveCounter[i]++;
                playerX(board);
            }
            else if (player[0].ship_t[C].moveCounter[i] < 3) {
                movingShipsForwardPlayer(i, C, board);
                player[0].ship_t[C].moveCounter[i]++;
                playerX(board);
            }
            else { code = 4; moveErrorMessages(code, i, C, D); }
        }
        else if (player[1].isPlayersMove == true) {
            if (C == 0 && player[1].ship_t[0].moveCounter[i] < 2) {
                movingShipsForwardPlayer(i, C, board);
                player[1].ship_t[C].moveCounter[i]++;
                playerX(board);
            }
            else if (player[1].ship_t[C].moveCounter[i] < 3) {
                movingShipsForwardPlayer(i, C, board);
                player[1].ship_t[C].moveCounter[i]++;
                playerX(board);
            }
            else { code = 4; moveErrorMessages(code, i, C, D); }
        }
    }
    else if (theresReef == true) { code = 0; moveErrorMessages(code, i, C, D); }
    else if (shipIsntNear == false) { code = 1; moveErrorMessages(code, i, C, D); }
    else if (isShipGoingApart == true) { code = 2; moveErrorMessages(code, i, C, D); }
    else if (isEngineBroken == true) { code = 3; moveErrorMessages(code, i, C, D); }
    else exit(0);
}
//as above but for moving left
void movingShipsLeftPlayer(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y - 1][x - CARSIZE + C + 1 + j] = board[y + j][x]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y + k][x] = ' '; }
            player[playersNum].ship_t[C].y1[i]--;
            player[playersNum].ship_t[C].x1[i] -= CARSIZE - C - 1;
            player[playersNum].ship_t[C].y2[i] -= CARSIZE - C;
            player[playersNum].ship_t[C].destination[i] = 'W';
            // the same player[playersNum].ship_t[C].x2[i];
        }
        else if (D == 'W') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y + CARSIZE - C - 1 - j][x - 1] = board[y][x + j]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y][x + k] = ' '; }
            player[playersNum].ship_t[C].y1[i] += CARSIZE - C - 1;
            player[playersNum].ship_t[C].x1[i]--;
            // the same player[playersNum].ship_t[C].y2[i];
            player[playersNum].ship_t[C].x2[i] -= CARSIZE - C;
            player[playersNum].ship_t[C].destination[i] = 'S';
        }
        else if (D == 'S') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y + 1][x + CARSIZE - C - 1 - j] = board[y - j][x]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y - k][x] = ' '; }
            player[playersNum].ship_t[C].y1[i]++;
            player[playersNum].ship_t[C].x1[i] += CARSIZE - C - 1;
            player[playersNum].ship_t[C].y2[i] += CARSIZE - C;
            // the same player[playersNum].ship_t[C].x2[i];
            player[playersNum].ship_t[C].destination[i] = 'E';
        }
        else if (D == 'E') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y - CARSIZE + C + 1 + j][x + 1] = board[y][x - j]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y][x - k] = ' '; }
            player[playersNum].ship_t[C].y1[i] -= CARSIZE - C - 1;
            player[playersNum].ship_t[C].x1[i]++;
            // the same player[playersNum].ship_t[C].y2[i];
            player[playersNum].ship_t[C].x2[i] += CARSIZE - C;
            player[playersNum].ship_t[C].destination[i] = 'N';
        }
    }
}
void movingShipsLeft(int i, int C, char** board) {
    movingShips(i, C, 'L', board);
}
//as above but for moving right
void movingShipsRightPlayer(int i, int C, char** board) {
    int playersNum = whichPlayer();
    int x = player[playersNum].ship_t[C].x1[i];
    int y = player[playersNum].ship_t[C].y1[i];
    char D = player[playersNum].ship_t[C].destination[i];

    if (player[playersNum].isPlayersMove == true) {
        if (D == 'N') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y - 1][x + CARSIZE - C - 1 - j] = board[y + j][x]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y + k][x] = ' '; }
            player[playersNum].ship_t[C].y1[i]--;
            player[playersNum].ship_t[C].x1[i] += CARSIZE - C - 1;
            player[playersNum].ship_t[C].y2[i] -= CARSIZE - C;
            // the same player[*playerptr].ship_t[C].x2[i];
            player[playersNum].ship_t[C].destination[i] = 'E';

        }
        else if (D == 'W') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y - CARSIZE + C + 1 + j][x - 1] = board[y][x + j]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y][x + k] = ' '; }
            player[playersNum].ship_t[C].y1[i] -= CARSIZE - C - 1;
            player[playersNum].ship_t[C].x1[i]--;
            // the same player[playersNum].ship_t[C].y2[i];
            player[playersNum].ship_t[C].x2[i] -= CARSIZE - C;
            player[playersNum].ship_t[C].destination[i] = 'N';
        }
        else if (D == 'S') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y + 1][x - CARSIZE + C + 1 + j] = board[y - j][x]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y - k][x] = ' '; }
            player[playersNum].ship_t[C].y1[i]++;
            player[playersNum].ship_t[C].x1[i] -= CARSIZE - C - 1;
            player[playersNum].ship_t[C].y2[i] += CARSIZE - C;
            // the same player[playersNum].ship_t[C].x2[i];
            player[playersNum].ship_t[C].destination[i] = 'W';
        }
        else if (D == 'E') {
            for (int j = 0; j < CARSIZE - C; j++) { board[y + CARSIZE - C - 1 - j][x + 1] = board[y][x - j]; }
            for (int k = 0; k < CARSIZE - C; k++) { board[y][x - k] = ' '; }
            player[playersNum].ship_t[C].y1[i] += CARSIZE - C - 1;
            player[playersNum].ship_t[C].x1[i]++;
            // the same player[playersNum].ship_t[C].y2[i];
            player[playersNum].ship_t[C].x2[i] += CARSIZE - C;
            player[playersNum].ship_t[C].destination[i] = 'S';
        }
    }

}
void movingShipsRight(int i, int C, char** board) {
    movingShips(i, C, 'R', board);
}
void movingShips(int i, int C, char D, char** board) {
    bool theresReef = 0;
    if (D == 'L') theresReef = supplementToReefCheckerMovingLeft(i, C, board);
    if (D == 'R') theresReef = supplementToReefCheckerMovingRight(i, C, board);
    bool shipIsntNear = 0;
    if (D == 'L') shipIsntNear = supplementToCheckingBeingCloseWhileMovingLeft(i, C, board);
    if (D == 'R') shipIsntNear = supplementToCheckingBeingCloseWhileMovingRight(i, C, board);
    bool isShipGoingApart = 0;
    if (D == 'L') isShipGoingApart = checkIfShipIsGoingApartFromTheBoardLeft(i, C);
    if (D == 'R') isShipGoingApart = checkIfShipIsGoingApartFromTheBoardRight(i, C);
    bool isEngineBroken = checkIfShipHasBrokenEngine(i, C, board);

    if (theresReef == false && isShipGoingApart == false && shipIsntNear == true && isEngineBroken == false) {

        if (player[0].isPlayersMove == true) {
            if (C == 0 && player[0].ship_t[0].moveCounter[i] < 2) {
                movingShipSelectorPlayer(i, C, D, board);
                player[0].ship_t[C].moveCounter[i]++;
            }
            else if (player[0].ship_t[C].moveCounter[i] < 3) {
                movingShipSelectorPlayer(i, C, D, board);
                player[0].ship_t[C].moveCounter[i]++;
            }
            else { int code = 4; moveErrorMessages(code, i, C, D); }
        }
        else if (player[1].isPlayersMove == true) {
            if (C == 0 && player[1].ship_t[0].moveCounter[i] < 2) {
                movingShipSelectorPlayer(i, C, D, board);
                player[1].ship_t[C].moveCounter[i]++;
            }
            else if (player[1].ship_t[C].moveCounter[i] < 3) {
                movingShipSelectorPlayer(i, C, D, board);
                player[1].ship_t[C].moveCounter[i]++;
            }
            else { int code = 4; moveErrorMessages(code, i, C, D); }
        }
    }
    else if (theresReef == true) { int code = 0; moveErrorMessages(code, i, C, D); }
    else if (shipIsntNear == false) { int code = 1; moveErrorMessages(code, i, C, D); }
    else if (isShipGoingApart == true) { int code = 2; moveErrorMessages(code, i, C, D); }
    else if (isEngineBroken == true) { int code = 3; moveErrorMessages(code, i, C, D); }
    else { exit(0); }
}