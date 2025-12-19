#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

using namespace std;

enum typeTile {residentialArea, militarySite, none};

const int BOARDSIZE = 9;
const int NUMOFRES = 30;
const int NUMOFMIL = 4;
const int TOTRESWARN = 15;
const char MILI = 'M';
const char RESI = 'R';
const char NONE = ' ';
const char NUKE = 'N';

struct resTilePlaceHolder
{
    int x;
    int y;
};

struct milTilePlaceHolder
{
    int x;
    int y;
};

struct tile
{
    bool revealedBySpy;
    bool tileContaminated;
    char tileSymbol;
    typeTile type;
    bool occupied;
    bool wasNukedAlready;
};

resTilePlaceHolder resMemArray[NUMOFRES];
milTilePlaceHolder milMemArray[NUMOFMIL];
char displayBoard[BOARDSIZE][BOARDSIZE];
tile actuBoard[BOARDSIZE][BOARDSIZE];

void initializeActuBoard();
void displayCharBoard(bool endgame);
HANDLE hOut;
////beginning of main

int main(int argc, char* argv[])
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SetConsoleTitle("homeyg's Nuclear Wars, v2.5!");
    
    int noOfNukesLeft;
    int reputation;
    bool contGame = true;
    bool gameStillOccuring;
    int spyRuns;
    char move, tutorial;
    int xSelection, ySelection;
    int xPlace, yPlace;
    int j = 0;
    int count = 0;
    int goAgain;
    
    ////main game loop
    while(contGame == true)
    {
        initializeActuBoard();
        gameStillOccuring = true;
        reputation = 30;
        noOfNukesLeft = 4;
        spyRuns = 15;
        
        cout<<"Welcome to Nuclear Wars!"<<endl;
        cout<<"Would you like instructions? (y or n): ";
        cin>>tutorial;
        
        if((tutorial == 'y')||(tutorial == 'Y'))
        {
            cout<<"The object of the game is to hunt down and destroy the "<<NUMOFMIL<<" military\n"
                <<"locations on the game board with nuclear weapons. You get reputation\nby destroying "
                <<"these locations, but you lose reputation by destroying\nresidential "
                <<"or civilian locations. Nuclear weapons, when used, not \nonly destroy the intended tile, but also all the tiles around it\nso you better watch out! "
                <<"At first the only revealed tiles will be "<<TOTRESWARN<<"\n"
                <<"residential tiles. You can uncover more tiles by using one of your \n"
                <<spyRuns<<" spy runs. Each will uncover a tile of your choice. There are\n"
                <<"a total of "<<NUMOFRES<<" residential locations on the board. This knowledge\n"
                <<"can be somewhat helpful. Press a key to recieve more instructions."
                <<"\n"<<endl;
            
            cin.get();
            cin.get();
            system("cls");
            
            cout<<"The keywords to make a move are as follows:\n'p' or 'P' to use a spy run\n"
                <<"'n' or 'N' to use a nuclear weapon"<<endl<<"The board grid is set up like the "
                <<"grid on a computer screen.\nThe Y axis value goes down as you go up the grid and starts with 0 "
                <<"for\nthe first tile. X axis is normal except it starts at 0 for the first tile.\n"
                <<"The game ends when all military sites are destroyed or you run out of weapons.\n"
                <<"Your final reputation is then summed and given to you.\n"
                <<"Military squares are represented by a 'M'.\n"
                <<"Residential squares are represented by an 'R'.\n"
                <<"Squares that have been nuked are represented by an 'N'.\n\n"
                <<"Have fun!";
            
            cin.get();
            
        }
                
        j = 0;
        for(int i = 0; i<BOARDSIZE; i++)
        {
            for(int j = 0; j<BOARDSIZE; j++)
            {
                displayBoard[i][j] = NONE;
            }
        }

        while(j<TOTRESWARN)
        {
            displayBoard[resMemArray[j].y][resMemArray[j].x] = RESI;
            actuBoard[resMemArray[j].y][resMemArray[j].x].revealedBySpy = true;
            j++;
        }

        while(gameStillOccuring)
        {
            count = 0;
            system("cls");
            displayCharBoard(false);

            cout<<"You have "<<spyRuns<<" spy runs left."<<endl;
            cout<<"You have "<<noOfNukesLeft<<" nukes left."<<endl;
            cout<<"Make move: ";

            cin>>move;
            

            switch(move)
            {
                case 'p':
                case 'P':
                {
                    if(spyRuns != 0)
                    {
                        /////////////////////////////////////////////ERROR LOOP FOR SPY RUNS STARTS HERE/////
                        cout<<"Select coordinates of spy run."<<endl;
                        cout<<"X = ";
                        cin>>xSelection;
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Please select correct coordinates of spy run."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
                            
                        }
                        
                        while((xSelection >= BOARDSIZE)||(xSelection < 0))
                        {  
                            cout<<"Please select correct coordinates of spy run."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
        
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"X = ";
                                cin>>xSelection;
                            }
                        }
                        
                        cout<<"Y = ";
                        cin>>ySelection;
                        ///////////////////
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Select correct coordinates of spy run."<<endl;
                            //cout<<"X = ";
                            //cin>>xSelection;
                            cout<<"Y = ";
                            cin>>ySelection;
                        }
                        
                        while((ySelection >= BOARDSIZE)||(ySelection < 0))
                        {  
                            cout<<"Please select correct coordinates of spy run."<<endl;
                            cout<<"Y = ";
                            cin>>ySelection;
        
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"Y = ";
                                cin>>ySelection;
                            }
                        }
                        
                        while(actuBoard[ySelection][xSelection].revealedBySpy == true)
                        {
                            cout<<"Please select correct coordinates of spy run."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"X = ";
                                cin>>xSelection;
                            }
    
                            while((xSelection >= BOARDSIZE)||(xSelection < 0))
                            {  
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"X = ";
                                cin>>xSelection;
        
                                while(!cin)
                                {
                                    cin.clear();
                                    cin.ignore();
                                    cout<<"Please select correct coordinates of spy run."<<endl;
                                    cout<<"X = ";
                                    cin>>xSelection;
                                }
                            }
                            cout<<"Y = ";
                            cin>>ySelection;
                        
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"Y = ";
                                cin>>ySelection;
                            }
    
                            while((ySelection >= BOARDSIZE)||(ySelection < 0))
                            {  
                                cout<<"Please select correct coordinates of spy run."<<endl;
                                cout<<"Y = ";
                                cin>>ySelection;
        
                                while(!cin)
                                {
                                    cin.clear();
                                    cin.ignore();
                                    cout<<"Please select correct coordinates of spy run."<<endl;
                                    cout<<"Y = ";
                                    cin>>ySelection;
                                }
                            }
                        }
                        /////ERROR LOOP FOR SPY RUNS ENDS HERE/////
                        /////////////////////////////////////////////////////////////////////////////////
                        if(actuBoard[ySelection][xSelection].type == residentialArea)
                        {
                            displayBoard[ySelection][xSelection] = RESI;
                        }
                        else if(actuBoard[ySelection][xSelection].type == militarySite)
                        {
                            displayBoard[ySelection][xSelection] = MILI;
                        }
                        else
                        {
                            displayBoard[ySelection][xSelection] = 'O';
                        }
                        actuBoard[ySelection][xSelection].revealedBySpy = true;
                        spyRuns--;
                    }
                    else
                    {
                        cout<<"Out of spy runs."<<endl;
                        cin.get();
                        cin.get();
                    }
                    break;
                }
                /////////////////CASE NUCLEAR/////////////////////////////////////////////////////////
                case 'n':
                case 'N':
                {
                   /////////ERROR WHILE LOOP FOR NUKES STARTS HERE/////////
                    cout<<"Please select coordinates to nuke."<<endl;
                    cout<<"X = ";
                    cin>>xSelection;
                    
                    while(!cin)
                    {
                        cin.clear();
                        cin.ignore();
                        cout<<"Please select correct coordinates to nuke."<<endl;
                        cout<<"X = ";
                        cin>>xSelection;
                    }
    
                    while((xSelection >= (BOARDSIZE-1))||(xSelection <= 0))
                    {  
                        cout<<"Please select correct coordinates to nuke."<<endl;
                        cout<<"X = ";
                        cin>>xSelection;
        
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
                        }
                    }
                    
                    cout<<"Y = ";
                    cin>>ySelection;
                    
                    while(!cin)
                    {
                        cin.clear();
                        cin.ignore();
                        cout<<"Please select correct coordinates to nuke."<<endl;
                        cout<<"Y = ";
                        cin>>ySelection;
                    }
                    //////////////////////
                    while((ySelection >= (BOARDSIZE-1))||(ySelection <= 0))
                    {  
                        cout<<"Please select correct coordinates to nuke."<<endl;
                        cout<<"Y = ";
                        cin>>ySelection;
        
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"Y = ";
                            cin>>ySelection;
                        }
                    }
                    /////////////
                    while(actuBoard[ySelection][xSelection].wasNukedAlready == true)
                    {
                        cout<<"Please select correct coordinates to nuke."<<endl;
                        cout<<"X = ";
                        cin>>xSelection;
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
                        }
    
                        while((xSelection >= (BOARDSIZE-1))||(xSelection <= 0))
                        {  
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"X = ";
                            cin>>xSelection;
        
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates to nuke."<<endl;
                                cout<<"X = ";
                                cin>>xSelection;
                            }
                        }
                        cout<<"Y = ";
                        cin>>ySelection;
                        
                        while(!cin)
                        {
                            cin.clear();
                            cin.ignore();
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"Y = ";
                            cin>>ySelection;
                        }
    
                        while((ySelection >= (BOARDSIZE-1))||(ySelection <= 0))
                        {  
                            cout<<"Please select correct coordinates to nuke."<<endl;
                            cout<<"Y = ";
                            cin>>ySelection;
        
                            while(!cin)
                            {
                                cin.clear();
                                cin.ignore();
                                cout<<"Please select correct coordinates to nuke."<<endl;
                                cout<<"Y = ";
                                cin>>ySelection;
                            }
                        }
                    }
                    /////////////END CASE NUCLEAR/////////////////////////////////////////////////////////
                    
                    //////////ERROR WHILE LOOP FOR NUKES ENDS HERE/////////
                    actuBoard[ySelection][xSelection].tileContaminated = true;
                    actuBoard[ySelection][xSelection+1].tileContaminated = true;
                    actuBoard[ySelection-1][xSelection+1].tileContaminated = true;
                    actuBoard[ySelection-1][xSelection].tileContaminated = true;
                    actuBoard[ySelection][xSelection-1].tileContaminated = true;
                    actuBoard[ySelection-1][xSelection-1].tileContaminated = true;
                    actuBoard[ySelection+1][xSelection].tileContaminated = true;
                    actuBoard[ySelection+1][xSelection+1].tileContaminated = true;
                    actuBoard[ySelection+1][xSelection-1].tileContaminated = true;
                        
                    displayBoard[ySelection][xSelection] = NUKE;
                    actuBoard[ySelection][xSelection].wasNukedAlready = true;
                    noOfNukesLeft--;
                    
                    if(noOfNukesLeft==0)
                    {
                        gameStillOccuring = false;
                    }
                    break;
                }
                default:
                {
                    cout<<"That's not a proper command."<<endl;
                    cin.get();
                    cin.get();
                    break;
                }
                
            }
            j = 0;
            while(j<NUMOFMIL)
            {
                yPlace = milMemArray[j].y;
                xPlace = milMemArray[j].x;
                if(actuBoard[yPlace][xPlace].tileContaminated == true)
                {
                    count++;
                }
                j++;
            }
            
            if(count == 4)
            {
                gameStillOccuring = false;
            }
            count = 0;
        }
        
        for(int i = 0; i<NUMOFRES; i++)
        {
            yPlace = resMemArray[i].y;
            xPlace = resMemArray[i].x;
            
            if(actuBoard[yPlace][xPlace].tileContaminated == true)
            {
                reputation -= 2;
            }
        }
        
        for(int i = 0; i<NUMOFMIL; i++)
        {
            yPlace = milMemArray[i].y;
            xPlace = milMemArray[i].x;
            
            if(actuBoard[yPlace][xPlace].tileContaminated == true)
            {
                reputation += 8;
            }
        }
        system("cls");
        displayCharBoard(false);
            
        cout<<"Game over! Your final reputation is "<<reputation<<"!"<<endl;
        cout<<"Press enter to reveal the board."<<endl;
        cin.ignore();
        cin.get();
        system("cls");
        for(int i = 0; i<BOARDSIZE; i++)
        {
            for(int j = 0; j<BOARDSIZE; j++)
            {
                switch(actuBoard[i][j].type)
                {
                    case residentialArea:
                        displayBoard[i][j] = RESI;
                        break;
                    case militarySite:
                        displayBoard[i][j] = MILI;
                        break;
                    case none:
                        displayBoard[i][j] = NONE;
                }
            }
        }
        

        displayCharBoard(true);
        cout<<"Would you like to play another round? (1-yes, 2-no)"<<endl;
        cin>>goAgain;
        
        if(goAgain == 1)
        {
            contGame = true;
        }
        else if(goAgain == 2)
        {
            contGame = false;
        }
        system("cls");
    }

    cout<<"Thanks for playing."<<endl;
    cin.get();
    cin.get();
    return 0;
}

////end of main

void initializeActuBoard()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int)seconds);
    int randX;
    int randY;

    for(int i = 0; i<BOARDSIZE; i++)
    {
        for(int j = 0; j<BOARDSIZE; j++)
        {
            actuBoard[i][j].revealedBySpy = false;
        }
    }
    
    for(int i = 0; i<BOARDSIZE; i++)
    {
        for(int j = 0; j<BOARDSIZE; j++)
        {
            actuBoard[i][j].wasNukedAlready = false;
            actuBoard[i][j].tileContaminated = false;
        }
    }
    
    for(int i = 0; i<BOARDSIZE; i++)
    {
        for(int j = 0; j<BOARDSIZE; j++)
        {
            actuBoard[i][j].type = none;
        }
    }

    for(int i = 0; i<BOARDSIZE; i++)
    {
        for(int j = 0; j<BOARDSIZE; j++)
        {
            actuBoard[i][j].occupied = false;
        }
    }

    int i = 0;

    while(i<NUMOFRES)
    {
        randX = 1+rand()%9;
        randY = 1+rand()%9;
        randX = randX-1;
        randY = randY-1;

        if(actuBoard[randY][randX].occupied == false)
        {
            actuBoard[randY][randX].type = residentialArea;
            resMemArray[i].x = randX;
            resMemArray[i].y = randY;
            actuBoard[randY][randX].occupied = true;
            i++;
        }
        else if(actuBoard[randY][randX].occupied != false)
        {
            continue;
        }

    }

    i = 0;

    while(i<NUMOFMIL)
    {
        randX = 1+rand()%9;
        randY = 1+rand()%9;
        randX = randX-1;
        randY = randY-1;

        if(actuBoard[randY][randX].occupied == false)
        {
            actuBoard[randY][randX].type = militarySite;
            milMemArray[i].x = randX;
            milMemArray[i].y = randY;
            actuBoard[randY][randX].occupied = true;
            i++;
        }
        else if(actuBoard[randY][randX].occupied != false)
        {
            continue;
        }

    }
}

void displayCharBoard(bool endgame)
{
    cout<<"[ ]";
    for(int i = 0; i<BOARDSIZE; i++)
    {
        cout<<"["<<i<<"]";
    }
    cout<<endl;
    for(int i = 0; i<BOARDSIZE; i++)
    {
        cout<<"["<<i<<"]";
        for(int j = 0; j<BOARDSIZE; j++)
        {
            if((displayBoard[i][j] == NUKE)&&(endgame == false))
            {
                SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }
            else if((displayBoard[i][j] == MILI)&&(endgame == false))
            {
                SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
            else if((displayBoard[i][j] == RESI)&&(endgame == false))
            {
                SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
            else if((endgame == true)&&(displayBoard[i][j]==MILI)&&(actuBoard[i][j].tileContaminated == true))
            {
                SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }
            else if((endgame == true)&&(displayBoard[i][j]==RESI)&&(actuBoard[i][j].tileContaminated == true))
            {
                SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }
            
            cout<<"["<<displayBoard[i][j]<<"]";
            SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        cout<<endl;
    }
}
